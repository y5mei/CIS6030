source("UnionFind.R")
library(testthat)
library("dbscan")
# import::from(factoextra, multishapes)

# test_that("constructor works", {
#   uf <- UF$new(5)
#   expect_equal(uf$size, c(1,1,1,1,1))
#   expect_equal(uf$parents, c(1,2,3,4,5))
# })
#
# test_that("union-find works", {
#   # example from https://www.youtube.com/watch?v=9J-TKZgAr88
#   uf <- UF$new(10)
#   uf$union(5,4)
#   uf$union(4,9)
#   uf$union(7,6)
#   uf$union(10, 5)
#   uf$union(3,2)
#
#   expect_equal(uf$find(9), uf$find(10))
#   expect_equal(uf$find(1) == uf$find(8), FALSE)
#
#   uf$union(6,1)
#   uf$union(8,3)
#   uf$union(7,2)
#   uf$union(2,1)
#
#   expect_equal(uf$find(1), uf$find(8))
#
# })
#
# test_that("tree node works", {
#   t1 <- TreeNode$new(1)
#   t2 <- TreeNode$new(2)
#   t3 <- TreeNode$new(left = t1, right = t2, value = 100)
#   expect_equal(t3$left, t1)
#   expect_equal(t3$right, t2)
# })
#
# test_that("HierarchicalTree works for creating leaf nodes", {
#   htree <- HierarchicalTree$new(5)
#   expect_equal(htree$hashmap[[5]]$idx, 5)
#   expect_equal(htree$hashmap[[4]]$idx, 4)
#   expect_equal(htree$hashmap[[3]]$idx, 3)
#   expect_equal(htree$hashmap[[2]]$idx, 2)
#   expect_equal(htree$hashmap[[1]]$idx, 1)
#
#   htree$build(4, 5, 10)
#   expect_equal(htree$root$value, 10)
#   htree$build(2, 3, 15)
#   expect_equal(htree$root$value, 15)
#   htree$build(3, 4, 25)
#   expect_equal(htree$root$value, 25)
#   expect_equal(htree$root$left$value, 15)
#   expect_equal(htree$root$right$value, 10)
#   htree$build(1, 5, 33)
#   expect_equal(htree$root$value, 33)
#   expect_equal(htree$root$left$value, 25)
#   expect_equal(htree$root$right$value, NA)
#
#   # make sure the union find's hash map is correct
#   x <- c(htree$uf$find(1),htree$uf$find(2),htree$uf$find(3),htree$uf$find(4),htree$uf$find(5))
#   expect_setequal(x, htree$uf$find(1))
# })
#
# test_that("HierarchicalTree works for condense based on MCS",{
#   htree <- HierarchicalTree$new(5)
#   htree$build(4, 5, 10)
#   htree$build(2, 3, 15)
#   htree$build(3, 4, 25)
#   htree$build(1, 5, 33)
#
#   htree$condense(minPts = 2)
#   expect_equal(htree$root$num_child, 4)
#   expect_equal(htree$root$left$num_child, 4)
#   expect_equal(htree$root$right, NULL)
#   expect_equal(htree$root$left$left$num_child, 2)
#   expect_equal(htree$root$left$right$num_child, 2)
#   expect_equal(htree$root$left$right$left$num_child, 1)
#   expect_equal(htree$root$left$right$right$num_child, 1)
#   expect_equal(htree$root$left$left$left$num_child, 1)
#   expect_equal(htree$root$left$left$right$num_child, 1)
#
#   # after condense, we can check the extrac with lambda
#   htree$extract()
#
# })
#
# test_that("HDBSCAN works for direct supply mutrial reachability matrix",{
#   l <- c(0,35,45,40,33,35,0,15,26,30,45,15,0,25,32,40,26,25,0,10, 33,30,32,10,0)
#   g <- matrix(l, nrow = 5)
#   hdbscan <- HDBSCAN$new(minPts=2, g = g)
#   htree <- hdbscan$hTree
#   expect_equal(htree$root$value, 33)
#   expect_equal(htree$root$left$value, 25)
#   expect_equal(htree$root$right, NULL)
#   expect_equal(htree$root$left$left$value, 10)
#   expect_equal(htree$root$left$right$value, 15)
#
#   # check the num of children are correct
#   expect_equal(htree$root$num_child, 4)
#   expect_equal(htree$root$left$num_child, 4)
#   expect_equal(htree$root$left$left$num_child, 2)
#   expect_equal(htree$root$left$right$num_child, 2)
# })

# test_that("HDBSCAN works for providing a table",{
#   data("moons")
#   table <- moons
#   hdbscan <- HDBSCAN$new(table = table, minPts=99)
#   table <- hdbscan$table
#   plot(table[,1], table[, 2], col = hdbscan$predict, pch = 19, cex = 0.8)
# })

MINPTS <- 3

data("moons")
plot(moons, pch=20)
moons <- moons[1:7,]
label <- 1:80
cl <- hdbscan(moons, minPts = MINPTS, gen_simplified_tree = TRUE, gen_hdbscan_tree = TRUE)
true_core_dist <- kNNdist(moons, k=MINPTS-1)

plot(cl$hdbscan_tree, main="HDBSCAN* HierarchyMayson")
plot(moons, col=cl$cluster+1, pch=19)
text(moons[,1], moons[,2] + 0.05, labels=rownames(moons), font=6)

plot(cl$hc, main="HDBSCAN* Hierarchy")
print(cl$hc$merge)
# plot(cl,show_flat = T)

# # TODO: Need to do a bench mark here!!!
# # data("moons")
table <- moons
hdbscan <- HDBSCAN$new(table = table, minPts=MINPTS)
table <- hdbscan$table
plot(table[,1], table[, 2], col =hdbscan$predict, pch = 19, cex = 0.8)
text(table[,1], table[,2] + 0.05, labels=rownames(table), font=6)
euclidean_dist <- as.matrix(dist(table))
core_dist <- apply(euclidean_dist, 1, function(x) x[topn(x, MINPTS, decreasing = FALSE)[MINPTS]])
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
                                q <- PriorityQueue()
                                # convert a matrix to be a mst, this need to be a lower triagnal matrix to avoid the weight of edges
                                # to be calculated twice
                                g[upper.tri(g)] <- 0
                                G <- as.undirected(graph.adjacency(g, weighted = TRUE))
                                g_mst <- mst(G)
                                g_mtrix <- as_adjacency_matrix(g_mst, type = "lower", attr="weight")
                                m <- matrix(g_mtrix, nrow = dim(g_mtrix)[1])
                                m[upper.tri(m)] <- 0
#
# write.table(moons, "sample.csv", row.names = FALSE, sep = ",")
write.table(euclidean_dist, "dist.csv", row.names = FALSE, sep = ",")
write.table(m, "mst_matrix.csv", row.names = FALSE, sep = ",")


# Need to have a method that convert a Table of n rows more than 2 cols into a Multra Distance Matrix
#    import::from(factoextra, multishapes)
#    data("multishapes")
#    table <- multishapes[1:50,]
# #   # newCol <- 1:1100
# #   # table["predict"] <- newCol # append a new col
#   hdbscan <- HDBSCAN$new(table = table, minPts=5)
#   table <- hdbscan$table
#   plot(table[,1], table[, 2], col = table[, 4], pch = 19, cex = 0.8)