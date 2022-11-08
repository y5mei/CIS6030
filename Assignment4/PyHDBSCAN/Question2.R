# # Title     : TODO
# # Objective : TODO
# # Created by: Chris
# # Created on: 2022/11/2
# library(factoextra)
# library(ggplot2)
# library(igraph)
# import::from(kit, topn)
# library(R6)
# library(pryr)
#
# a<-100
# dataset <- multishapes
# p <- ggplot(dataset, aes(x, y)) + geom_point(aes(col=shape), size=2)
# plot(p)
#
# # Calculate the mutual reachability distance for each data points
# # the mutual reachbility graph is a 1100x1100 matrix, where the major diag is the core_k() distance
# k <- 3
# xy <- dataset[, c(1,2)]
# euclidean_dist <- as.matrix(dist(xy))
# core_dist <- apply(euclidean_dist, 1, function(x) topn(x, minPts, decreasing = FALSE)[minPts])
#
# mutual_dist <- matrix(, nrow = 1100, ncol = 1100)
# for(row in 1:nrow(mutual_dist)) {
#     for(col in 1:ncol(mutual_dist)) {
#         if (row == col){
#             mutual_dist[row, col] <- 0
#         }else{
#              mutual_dist[row, col] <- max(euclidean_dist[row, col], core_dist[row], core_dist[col])
#         }
#     }
# }
#
# # # Build the minimum spanning tree
# g <- mutual_dist
# l <- c(0,35,45,40,33,35,0,15,26,30,45,15,0,25,32,40,26,25,0,10, 33,30,32,10,0)
# g <- matrix(l, nrow = 5)
# g[upper.tri(g)] <- 0
# G <- as.undirected(graph.adjacency(g, weighted = TRUE))
# # plot(G, edge.label=round(E(G)$weight, 3))
# g_mst <- mst(G)
# plot(g_mst, edge.label=round(E(g_mst)$weight, 3))
# g_mtrix <- as_adjacency_matrix(g_mst, type = "lower", attr="weight")
#
# m <- matrix(g_mtrix, nrow = 5)
# m[upper.tri(m)] <- 0
# print("Sample Matrix:")
# print(m)
#
# # # stores indexes of max value
# # max = which(m == min(m[m>0]), arr.ind=TRUE)
# # print(paste("Maximum value: ",m[max]))
# # print(max[1,1])
# # print(max[1,2])
