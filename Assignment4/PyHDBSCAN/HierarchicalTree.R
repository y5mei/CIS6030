source("UnionFind.R")
import::from(rlang, is_missing)
import::from(collections, PriorityQueue)
library(r2r)
library(dequer)
library(igraph)
import::from(DescTools, Small) # Todo: change this to TopN

TreeNode <- R6Class("TreeNode",
                    public = list(
                      left = NULL,
                      right = NULL,
                      value = NULL, # this is the distance value where two nodes merge together
                      stability = 0, # the leaf node has stability 0, so that the leaf nodes can always propagate stability to their parents and add themselves as a propagated descendent in the  partent
                      score = 0, # todo: the max between node's stability and the sum of its two child in condensed tree
                      lambda_birth = 0, # the lambda_birth for the root node is 0, for the leaf nodes are 1/dist where they merge into a cluster
                      lambda_all_children_leave_cluster = 0,
                      idx = NA, # this is very important, for leafe nodes, idx = row number, for internal node, if idx = -1, this is the first internal node in merge matrix
                      num_child = 1,
                      is_in_condensed_tree = FALSE,
                      contains = NA,
                      initialize = function(idx, left = NULL, right = NULL, value = NA) {
                        if (missing(idx)) {
                          idx <- NA
                        }
                        self$left <- left
                        self$right <- right
                        self$value <- value
                        self$idx <- idx
                      },
                      getIdx = function() {
                        return(self$idx)
                      },
                      isLeafNode = function() {
                        return(is.null(self$left) & is.null(self$right))
                      }
                    ))


HierarchicalTree <- R6Class("HierarchicalTree",
                            public = list(
                              # The map from root # of each leafe node (int) -> current internal node (TreeNode),
                              # say after union(5, 6), union(6, 2), then
                              # leafe node 5's UF current root is 2,
                              # hashmap[2] stores the internal "root"
                              hashmap = NA,
                              uf = NA,
                              root = NA,
                              condensed_root = NA,
                              all_nodes_hashmap = hashmap(), # keys are negative ints for internal nodes, positive for leaf nodes
                              n = NA,
                              cluster = NA,
                              internal_node_cnter = 0,

                              table = NA,
                              minPts = NA,
                              merge = NA, # this is a table
                              post_order_treenodes = c(),

                              initialize = function(table, minPts) {
                                # initialize the leaf node hashmap bins
                                self$n <- nrow(table)
                                self$cluster <- replicate(self$n, 0)
                                self$hashmap <- list()
                                for (i in seq_len(nrow(table))) {
                                  new_node <- TreeNode$new(idx = i)
                                  self$hashmap <- append(self$hashmap, new_node)
                                  new_node$num_child <- 1
                                  self$all_nodes_hashmap[[i]] <- new_node # save this internal node into the hashmap to be used in condense step
                                  self$cluster[[i]] <- i # Select all the leave nodes as clusters and process internal nodes in post-order later
                                  new_node$contains <- c(i)
                                }
                                # initialize the union find obj
                                self$uf <- UF$new(self$n)

                                # setup the merge table
                                self$merge <- data.frame(matrix(ncol = 5, nrow = 0))
                                colnames(self$merge) <- c("child1", "child2", "parent", "height", "lambda")

                                self$minPts <- minPts
                                if (!is_missing(table)) {
                                  self$table <- table
                                  # plot(table[,1], table[, 2], col = table[, 3], pch = 19, cex = 0.8)
                                  # calculate the mutual_dist graph matrix, and overwrite g with it
                                  xy <- table[, c(1, 2)]
                                  euclidean_dist <- as.matrix(dist(xy))
                                  core_dist <- apply(euclidean_dist, 1, function(x) Small(x, minPts, unique = FALSE)[minPts])

                                  mutual_dist <- matrix(, nrow = nrow(table), ncol = nrow(table))
                                  for (row in seq_len(nrow(mutual_dist))) {
                                    for (col in seq_len(ncol(mutual_dist))) {
                                      if (row == col) {
                                        mutual_dist[row, col] <- 0
                                      }else {
                                        mutual_dist[row, col] <- max(euclidean_dist[row, col], core_dist[row], core_dist[col])
                                      }
                                    }
                                  }

                                  g <- mutual_dist
                                }

                                # convert a matrix to be a mst, this need to be a lower triagnal matrix to avoid the weight of edges
                                # to be calculated twice
                                g[upper.tri(g)] <- 0
                                G <- as.undirected(graph.adjacency(g, weighted = TRUE))
                                g_mst <- mst(G)

                                # clean the mst, convert it back to a lower triagnal matrix
                                g_mtrix <- as_adjacency_matrix(g_mst, type = "lower", attr = "weight")
                                m <- matrix(g_mtrix, nrow = dim(g_mtrix)[1])
                                m[upper.tri(m)] <- 0

                                # build the heap
                                q <- PriorityQueue()
                                for (row in seq_len(nrow(m))) {
                                  for (col in 1:row) {
                                    if (m[row, col] != 0) {
                                      q$push(c(row, col, m[row, col]), priority = -m[row, col])
                                    }
                                  }
                                }

                                # loop through the heap (from min to max) to build the HTree
                                while (q$size()) {
                                  elements <- q$pop()
                                  left <- elements[1]
                                  right <- elements[2]
                                  value <- elements[3]
                                  self$build(left, right, value)
                                }

                                self$condense()
                                self$extract()
                              },

                              # a helper funciton to construct a htree via union find, the root is saved into self$root
                              # the merge matrix is also saved self$merge
                              build = function(left, right, value) {
                                left_treeNode <- self$hashmap[[self$uf$find(left)]]
                                right_treeNode <- self$hashmap[[self$uf$find(right)]]

                                # put the node with more child to the left
                                if (left_treeNode$num_child < right_treeNode$num_child) {
                                  temp <- left_treeNode
                                  left_treeNode <- right_treeNode
                                  right_treeNode <- temp
                                }

                                self$internal_node_cnter <- self$internal_node_cnter - 1 # decreased the internal node cnter by 1
                                # create new internal node, and save the lambda_birth for left and right treenode
                                new_node <- TreeNode$new(left = left_treeNode, right = right_treeNode, value = value, idx = self$internal_node_cnter)
                                left_treeNode$lambda_birth <- 1 / value
                                right_treeNode$lambda_birth <- 1 / value

                                # # if left or right is leaf node, also save the lambda_birth value as lambda_all_children_leave_cluster value
                                # if (left_treeNode$isLeafNode()) {
                                #   left_treeNode$lambda_all_children_leave_cluster <- 1 / value
                                # }
                                # if (right_treeNode$isLeafNode()) {
                                #   right_treeNode$lambda_all_children_leave_cluster <- 1 / value
                                # }

                                # Append left.idx, right.idx, height, new_node.idx, 1/height save to merge table
                                left_idx <- left_treeNode$idx
                                right_idx <- right_treeNode$idx
                                lambda <- 1 / value
                                self$merge[-self$internal_node_cnter,] <- c(-left_idx, -right_idx, value, -new_node$idx, lambda)
                                new_node$num_child <- (left_treeNode$num_child + right_treeNode$num_child)

                                self$uf$union(left, right)
                                new_root_idx <- self$uf$find(left)
                                self$hashmap[[new_root_idx]] <- new_node # save the internal node into the fake "hashmap" for union find purpose
                                self$all_nodes_hashmap[[new_node$idx]] <- new_node # save this internal node into the hashmap to be used in condense step
                                self$root <- new_node
                                self$root$lambda_birth <- self$root$left$lambda_birth # todo: this is an appoximation
                              },

                              # condense a hierarchy tree based on MCS, this condensed tree only store the idx (negative numbers) for the non-trimed internal node
                              # if you need to actually get the internal nodes, you need to get each of the internal nodes from self$all_nodes_hashmap[idx]
                              condense = function() {
                                # level order transversal the htree to get the condensed nodes
                                dummy <- self$root
                                dummy$is_in_condensed_tree <- TRUE
                                d <- deque()
                                push(d, self$root)
                                while (length(d) > 0) {
                                  node <- popback(d)
                                  if (!is.null(node$left) & node$left$num_child >= self$minPts) {
                                    node$left$is_in_condensed_tree <- TRUE
                                    push(d, node$left)
                                  }
                                  if (!is.null(node$right) & node$right$num_child >= self$minPts) {
                                    node$right$is_in_condensed_tree <- TRUE
                                    push(d, node$right)
                                  }
                                }

                              },

                              post_order = function(treeNode, parentValue, func) {
                                if (is.null(treeNode)) {
                                  return()
                                }
                                if (is.null(treeNode$right) & is.null(treeNode$left)) {
                                  func(treeNode, parentValue)
                                  return()
                                }
                                if (!is.null(treeNode$left)) {
                                  self$post_order(treeNode$left, treeNode$value, func)
                                }
                                if (!is.null(treeNode$right)) {
                                  self$post_order(treeNode$right, treeNode$value, func)
                                }
                                # Break at tree's root level, else, post order
                                if (parentValue == -10000) {
                                  return()
                                }else {
                                  func(treeNode, parentValue)
                                }
                              },

                              cache_treenodes = function(node, someValue){
                                self$post_order_treenodes <- c(self$post_order_treenodes, node)
                              },

                              iterative_post_order = function(func){
                                for(treeNode in self$post_order_treenodes){
                                  func(treeNode, someValue)
                                }
                              },

                              calculate_lambda_for_all_children_left_cluster = function(node, someValue) {
                                if (node$isLeafNode()) {
                                  node$lambda_all_children_leave_cluster <- node$lambda_birth
                                  return()
                                }
                                if (node$left$is_in_condensed_tree & node$right$is_in_condensed_tree) {
                                  # calcualte the stability of each node (only internal nodes) after condense,
                                  # if a node's left and right are both not in the condensed tree, sta = sta_left + sta_right
                                  node$lambda_all_children_leave_cluster <- node$left$lambda_birth * node$num_child
                                }else {
                                  left_result <- node$left$lambda_all_children_leave_cluster
                                  right_result <- node$right$lambda_all_children_leave_cluster
                                  node$lambda_all_children_leave_cluster <- left_result + right_result
                                }
                                # also need to merge all the leaf nodes from the subtrees
                                node$contains <- c(node$left$contains, node$right$contains)

                              },

                              calculate_stability = function(node, someValue) {
                                if (node$isLeafNode()) {
                                  node$stability <- 0
                                }else {
                                  node$stability <- node$lambda_all_children_leave_cluster - node$num_child * node$lambda_birth
                                }
                              },

                              # set_cluster_num_for_all_leaf_nodes_below = function(node, clusterNum) {
                              #   if (!node$isLeafNode()) {
                              #     return()
                              #   }
                              #   if (self$cluster[[node$idx]] == 0) {
                              #     # do thing with the noise leaf nodes
                              #     return()
                              #   }
                              #   # set the non-outlier leaf node belongs to a cluster
                              #   self$cluster[[node$idx]] <- clusterNum
                              #
                              # },

                              # now we need to calculate score, only the condensed nodes have valid score defined,
                              # score is the max of (node's stability, sum of the stability of its two children)
                              # start from all the leaf nodes are seperate clusters,
                              # if node's stability >= sum of children, then set the node as one cluster, and unselect the two children???
                              calculate_score = function(node, someValue) {
                                if (node$isLeafNode()) {
                                  node$score <- 0
                                }else {
                                  node$score <- max(node$stability, node$left$score + node$right$score)

                                  if (node$stability >= node$left$score + node$right$score) {
                                    # if node is more stable, cluster all the leaf nodes in left, right child into the same cluster
                                    # todo: post-order tranverse from the current node, if any leaf node's cluster is not zero, merge them to the same cluster
                                    # self$post_order(node, node$idx, set_cluster_num_for_all_leaf_nodes_below)
                                    for (i in node$contains) {
                                      self$cluster[[i]] <- node$idx
                                    }
                                  }else {
                                    outliers <- c()
                                    if (!node$left$is_in_condensed_tree) {
                                      outliers <- c(outliers, node$left$contains)
                                    }
                                    if (!node$right$is_in_condensed_tree) {
                                      outliers <- c(outliers, node$right$contains)
                                    }
                                    for (j in outliers) {
                                      self$cluster[[j]] <- 0
                                    }
                                  }
                                  # todo: if current node has left or right children that are not in the clusting tree, set them as outlier and set class to be zero
                                }
                              },

                              extract = function() {
                                # calculate the sum of lambda that all children left cluster for each nodes
                                self$post_order(self$root, 100, self$cache_treenodes)
                                # self$post_order(self$root, 100, self$calculate_lambda_for_all_children_left_cluster)
                                # self$post_order(self$root, 100, self$calculate_stability)
                                # self$post_order(self$root, 100, self$calculate_score)
                                self$iterative_post_order(self$calculate_lambda_for_all_children_left_cluster)
                                self$iterative_post_order(self$calculate_stability)
                                self$iterative_post_order(self$calculate_score)

                                # change the cluster num from negative to positive
                                self$cluster <- self$cluster * (-1)
                                final_cluster <- hashmap()
                                cnter <- 1
                                for (i in 1:length(self$cluster)) {
                                  if (self$cluster[[i]] == 0) {

                                  }else {
                                    if (!has_key(final_cluster, self$cluster[[i]])) {
                                      insert(final_cluster, self$cluster[[i]], cnter)
                                      self$cluster[[i]] <- cnter
                                      cnter <- cnter + 1
                                    }else {
                                      self$cluster[[i]] <- query(final_cluster, self$cluster[[i]])
                                    }
                                  }
                                }
                              }
                            ))