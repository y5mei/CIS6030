# library(R6)
# library(pryr)
library(igraph)
import::from(R6, R6Class)
import::from(collections, PriorityQueue)
import::from(kit, topn)
import::from(rlang, is_missing)
library(r2r)

# https://adv-r.hadley.nz/r6.html
UF <- R6Class("UF",
              public = list(
                parents = NA,
                size = NA,
                initialize = function (N) {
                  self$parents <- 1:N
                  self$size <- rep(1, N)
                },
                find = function(x) {
                  while (x != self$parents[x]) {
                    x <- self$parents[x]
                    self$parents[x] = self$parents[self$parents[x]]
                  }
                  return (x)
                },
                union = function (child, parent){
                  this <- self$find(child)
                  that <- self$find(parent)

                  if (self$size[this]<self$size[that]){
                    self$parents[this] <- that
                    self$size[that] <- self$size[that] + self$size[this]
                  }else{
                    self$parents[that] <- this
                    self$size[this] <- self$size[that] + self$size[this]
                  }
                }
              ))

TreeNode <- R6Class("TreeNode",
                    public = list(
                      left = NA,
                      right = NULL,
                      value = NULL,
                      stability = 0,
                      idx = NA,
                      num_child = 1,
                      initialize = function (idx, left=NULL, right=NULL, value=NA){
                        if (missing(idx)){
                          idx = NA
                        }
                        self$left <- left
                        self$right <- right
                        self$value <- value
                        self$idx <- idx
                      },
                      getIdx = function(){
                        return (self$idx)
                      }
                    ))

HierarchicalTree <- R6Class("HierarchicalTree",
                            public = list(
                              hashmap = NA,
                              uf = NA,
                              root = NA,
                              n = NA,
                              classification = NA,
                              myClusters = hashset(),
                              initialize = function(N){
                                # initialize the leaf node hashmap bins
                                self$n <- N
                                self$classification <- replicate(N, 1)
                                self$hashmap <- list()
                                for (i in 1:N){
                                  self$hashmap <- append(self$hashmap, TreeNode$new(idx = i))
                                }
                                # initialize the union find obj
                                self$uf <- UF$new(N)
                              },
                              build = function(left, right, value){
                                left_treeNode <- self$hashmap[[self$uf$find(left)]]
                                right_treeNode <- self$hashmap[[self$uf$find(right)]]
                                # put the node with more child on the left
                                if (left_treeNode$num_child < right_treeNode$num_child){
                                  temp <- left_treeNode
                                  left_treeNode <- right_treeNode
                                  right_treeNode <- temp
                                }
                                # if (!is.na(left_treeNode$value)){
                                #   if (is.na(right_treeNode$value)){
                                #     temp <- left_treeNode
                                #     left_treeNode <- right_treeNode
                                #     right_treeNode <- temp
                                #   }else{
                                #     if (right_treeNode$value < left_treeNode$value){
                                #       temp <- left_treeNode
                                #       left_treeNode <- right_treeNode
                                #       right_treeNode <- temp
                                #     }
                                #   }
                                # }
                                new_node <- TreeNode$new(left = left_treeNode, right = right_treeNode, value = value)
                                new_node$num_child <- (left_treeNode$num_child + right_treeNode$num_child)

                                self$uf$union(left, right)
                                new_root_idx <- self$uf$find(left)
                                self$hashmap[[new_root_idx]] <- new_node
                                self$root <- new_node
                              },
                              # top-down tranversal the self.root to conduct a condense to the hierarchy based on the min cluster size (MCS)
                              # if one of the two children is smaller than the MCS, we remove the child
                              # if two of the children both smaller than the MCS, we do not go any future (this is not from the course node, we basically keep all the data points in this case)
                              # if two of the children both greater than the MCS, we recursively visit both of the children
                              # basically, condense if a operation that tranversal the tree, and replace the child that is smaller than MCS to NA
                              condense = function(minPts){
                                helper <- function(treeNode){
                                  if (treeNode$left$num_child >= minPts & treeNode$right$num_child >= minPts){
                                    helper(treeNode$left)
                                    helper(treeNode$right)
                                  }else if (treeNode$left$num_child < minPts & treeNode$right$num_child < minPts){
                                    return()
                                  }else{
                                    if (treeNode$left$num_child < minPts){
                                      treeNode$num_child <-  treeNode$num_child - treeNode$left$num_child # this is very fishy, might need to delete this, basically, I am counting how many valid points belongs to this node
                                      treeNode$left <- NULL
                                      helper(treeNode$right)
                                    }else{
                                      treeNode$num_child <- treeNode$num_child - treeNode$right$num_child
                                      treeNode$right <- NULL
                                      helper(treeNode$left)
                                    }
                                  }
                                }
                                helper(self$root)
                              },
                              # need to transveral the condensed tree
                              # 1. Select all the leave nodes as clusters (this should be a set?)
                              # 2. Post order tranversal all the internal nodes:
                              #     2.1 Calculate the stability of the node
                              #     2.2 Calculate the sum of the stability of its two children
                              # 3. If the node's stability <  stability of two child: change the node's stability into the sum, do not change the cluster (the two children remain in the cluster)
                              #    If the node's stability >= stability of two child: select the node as a cluster and unselect all its descendants
                              extract = function(){
                                # tranversal the tree to get all the valid leaf nodes
                                # cluster_set <- hashset()

                                remove_clusters_from_sets<-function(treeNode){
                                  if (is.null(treeNode)){
                                    return()
                                  }
                                  if (is.null(treeNode$right) & is.null(treeNode$left)){
                                    print("Removing a cluster ")
                                    print(treeNode)
                                    print("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^")
                                    delete(self$myClusters, treeNode)
                                    return()
                                    }
                                  if (!is.null(treeNode$left)){

                                    remove_clusters_from_sets(treeNode$left)
                                  }
                                  if (!is.null(treeNode$right)){
                                    remove_clusters_from_sets(treeNode$right)
                                  }
                                  print("=========== ALSO NEED TO DELETE THE INTERNAL NODE ================")
                                  print(treeNode)
                                  print("=========== Before delete, the lenght of myCLUSTER IS ================")
                                  cat(length(self$myClusters), "\n")

                                  delete(self$myClusters, treeNode)
                                  print("After delete the internal node: ")
                                  print(keys(self$myClusters))
                                  print("=========== ======================== ================")
                                  print("=========== ======================== ================")
                                }

                                post_order <- function(treeNode, parentValue, func){
                                  if (is.null(treeNode)){
                                    return()
                                  }
                                  if (is.null(treeNode$right) & is.null(treeNode$left)){
                                      func(treeNode, parentValue)
                                      return()
                                    }
                                  if (!is.null(treeNode$left)){
                                    post_order(treeNode$left, treeNode$value, func)
                                  }
                                  if (!is.null(treeNode$right)){
                                    post_order(treeNode$right, treeNode$value, func)
                                  }
                                  # Break at tree's root level, else, post order
                                  if (parentValue == -100){
                                    return ()
                                  }else{
                                    func(treeNode, parentValue)
                                  }
                                }

                                my_print_num_child <- function(treeNode){
                                  print(treeNode$num_child)
                                  print(treeNode$value)
                                }

                                insert_cluseter_set <- function(treeNode, parentValue){
                                  # put all the leave nodes as clusters, and keep their stability to be 0
                                  if (is.null(treeNode$right) & is.null(treeNode$left)){
                                    cat("Add leafve node: ", treeNode$idx, "\n")
                                    print("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~")
                                     insert(self$myClusters, treeNode)
                                    }
                                  else{
                                    basic_stability <- treeNode$num_child * (1/treeNode$value - 1/parentValue)
                                    child_sum_stability <- 0
                                    if (!is.null(treeNode$left)){
                                      child_sum_stability <- child_sum_stability + treeNode$left$stability
                                    }
                                    if (!is.null(treeNode$right)){
                                      child_sum_stability <- child_sum_stability + treeNode$right$stability
                                    }
                                    if (basic_stability < child_sum_stability){
                                      treeNode$stability <- child_sum_stability
                                      # print(treeNode)
                                      # cat("The stabiltiy is ", treeNode$stability, "\n")
                                    }else{
                                      # todo: Not sure should I remove just the left and right child or I need to recursivly remove everything below?
                                      if (!is.null(treeNode$left)){
                                        remove_clusters_from_sets(treeNode$left)
                                      # delete(cluster_set, treeNode$left)
                                      cat("After remove the tree ndoes, there are ", length(self$myClusters), "\n")
                                        print("these treenodes are: ")
                                        print(keys(self$myClusters))
                                        print("%%%%%%%%%%%%%%%%%%%%%%%%%lll%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%")
                                      }
                                      if (!is.null(treeNode$right)){
                                       remove_clusters_from_sets(treeNode$right)
                                      # delete(cluster_set, treeNode$right)
                                        cat("After remove the tree ndoes, there are ", length(self$myClusters), "\n")
                                        print("these treenodes are: ")
                                        print(keys(self$myClusters))
                                        print("%%%%%%%%%%%%%%%%%%%%%%rrr%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%")
                                      }
                                      cat("Add internal node with stability: ", basic_stability, "\n")
                                      print(treeNode)
                                      print("~~~~~~~~~~~~~~~~~~~~XXXXXXX~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~")
                                      insert(self$myClusters, treeNode)
                                      treeNode$stability <- basic_stability
                                      # print(treeNode)
                                      # cat("Remove children, the stabiltiy is ", treeNode$stability, "\n")
                                    }
                                  }
                                  # process all the internal nodes in post-order
                                }
                                post_order(self$root, -100, insert_cluseter_set)
                                # print(length(cluster_set)) # display the number of clusters
                                # print(keys(cluster_set)[1]) # display all the clusters

                                # generate_cluster_vectors from the hashset, and update the classification field
                                self$generate_cluster_vector(self$myClusters)
                              },

                              generate_cluster_vector = function(cluster_set){
                                set_classification_helper <- function(treeNode, class_cnter){
                                    if (is.null(treeNode$left) & is.null(treeNode$right)){
                                      cat("Set leafnode : ", treeNode$idx, " to class ", class_cnter, "\n")
                                      self$classification[treeNode$idx] <- class_cnter
                                      return()
                                    }

                                    if (!is.null(treeNode$left)){
                                      set_classification_helper(treeNode$left, class_cnter)
                                    }

                                    if (!is.null(treeNode$right)){
                                       set_classification_helper(treeNode$right, class_cnter)
                                    }
                                  }

                                # print(keys(self$myClusters))
                                # classification <- replicate(self$n, 0) # the outliers are classified as class zero
                                for (class_cnter in 1:length(cluster_set)){
                                  currentClassNode <- keys(cluster_set)[[class_cnter]]
                                  # get all the leaf nodes under this node, and set the class to be class_cnter
                                  set_classification_helper(currentClassNode, class_cnter+1)
                                  print("=================================================")
                                  print(self$classification)
                                  print("=================================================")
                                }
                              }
                            ))

HDBSCAN<- R6Class("HDBSCAN",
                            public = list(
                              minPts = NA,
                              hTree = NA,
                              table = NA,
                              predict = NA,
                              # input is a mutual reachability distance graph matrix, g, does not need to be a tri matrix, but must be a symetry matrix at least.
                              initialize = function(table, minPts, g){
                                # table has higher priority, if table is provided, generate a g from the table
                                # table can be skipped, and user can only provide g matrix for unit test purpose
                                self$minPts <- minPts

                                if (!is_missing(table)){
                                  self$table <- table
                                  # plot(table[,1], table[, 2], col = table[, 3], pch = 19, cex = 0.8)
                                  # calculate the mutual_dist graph matrix, and overwrite g with it
                                  xy <- table[, c(1,2)]
                                  euclidean_dist <- as.matrix(dist(xy))
                                  core_dist <- apply(euclidean_dist, 1, function(x) x[topn(x, minPts, decreasing = FALSE)[minPts]])

                                  mutual_dist <- matrix(, nrow = nrow(table), ncol = nrow(table))
                                  for(row in 1:nrow(mutual_dist)) {
                                      for(col in 1:ncol(mutual_dist)) {
                                          if (row == col){
                                              mutual_dist[row, col] <- 0
                                          }else{
                                               mutual_dist[row, col] <- max(euclidean_dist[row, col], core_dist[row], core_dist[col])
                                          }
                                      }
                                  }

                                  g <- mutual_dist
                                }

                                q <- PriorityQueue()
                                # convert a matrix to be a mst, this need to be a lower triagnal matrix to avoid the weight of edges
                                # to be calculated twice
                                g[upper.tri(g)] <- 0
                                G <- as.undirected(graph.adjacency(g, weighted = TRUE))
                                g_mst <- mst(G)


                                # clean the mst, convert it back to a lower triagnal matrix
                                g_mtrix <- as_adjacency_matrix(g_mst, type = "lower", attr="weight")
                                m <- matrix(g_mtrix, nrow = dim(g_mtrix)[1])
                                m[upper.tri(m)] <- 0

                                # build the heap
                                for(row in 1:nrow(m)) {
                                    for(col in 1:row) {
                                      if (m[row, col] != 0){
                                        q$push(c(row, col, m[row, col]), priority = -m[row, col])
                                      }
                                    }
                                  }

                                # loop through the heap (from min to max) to build the HTree
                                htree <- HierarchicalTree$new(nrow(m))

                                while (q$size()){
                                  elements <- q$pop()
                                  left <- elements[1]
                                  right <- elements[2]
                                  value <- elements[3]
                                  htree$build(left, right, value)
                                }
                                self$hTree <- htree

                                # check htree's leaf nodes' idx
                                printLeaveNodeIdx <- function(x){
                                  if (is.null(x$left) &  is.null(x$right)){
                                    print("-=-=-=-=-=-=-=")
                                    print(x$idx)
                                    return()
                                  }
                                  if (!is.null(x$left)){
                                    printLeaveNodeIdx(x$left)
                                  }
                                  if(!is.null(x$right)){
                                    printLeaveNodeIdx(x$right)
                                  }
                                }

                                # Todo: remove this print leave node idx method
                                # printLeaveNodeIdx(htree$root)
                                # condense the tree
                                htree$condense(minPts)
                                htree$extract()
                                # Todo: remove this print leave node idx method
                                printLeaveNodeIdx(htree$root)

                                self$predict <- htree$classification # append a new col
                                self$hTree <- htree
                                # then extract flat clusters from the condensed tree
                              }
                            ))

# q <- PriorityQueue()
# q$push(c(1,2,3),priority = 1)
# q$push(c(2,3,4), priority = 2)
# q$push(c(-2,3,4), priority = 3)
# print("=============")
# while (q$size()){
#   print("----")
#   print(q$pop()) # urgent
# }
# Set of character keys, case insensitive.

# hashset methods: https://search.r-project.org/CRAN/refmans/r2r/html/hashtable_methods.html
# s <- hashset("A", "B", "C", key_preproc = tolower)
# insert(s, "pig")
# s[["pig"]]

# s <- hashset()
# insert(s, 100)
# insert(s, "a")
# s[[100]]
# s[["a"]]
# keys(s)
# length(s)