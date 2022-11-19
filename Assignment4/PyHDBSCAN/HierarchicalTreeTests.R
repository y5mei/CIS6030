source("HierarchicalTree.R")
library(testthat)
library("dbscan")
library(factoextra)

# test_that("constructor works", {
#
# })

data(multishapes)
# plot(multishapes[,1], multishapes[, 2],
#     col = multishapes[, 3], pch = 19, cex = 0.8)
moons<- multishapes[, 1:2]
DATA_SIZE <- 1100
MCS <- 40
Labels <- 1:DATA_SIZE

table <- moons[1:DATA_SIZE,]
cl <- dbscan::hdbscan(table, minPts = MCS)
plot(table, col = cl$cluster + 1, pch = 20, cex = 1.9, main ="Standard Lib Benchmarker")
dbscan_res <- dbscan(table, eps = 0.15, minPts = 5)
plot(table, col=dbscan_res$cluster+1, main="DBSCAN just for reference")

# plot(cl$hc)

# ht <- HierarchicalTree$new(table, minPts = MCS)
# plot(table, col = ht$cluster + 1, pch = 20, cex = 1.9, main = "My Implementation")
# print(length(ht$post_order_treenodes))