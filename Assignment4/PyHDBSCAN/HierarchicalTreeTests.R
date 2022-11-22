source("HierarchicalTree.R")
library(testthat)
library("dbscan")
library(factoextra)

# test_that("constructor works", {
#
# })

data(multishapes)
data(moons)
data("DS3")

################ overlook
# plot(multishapes[,1], multishapes[, 2],
#     col = multishapes[, 3], pch = 19, cex = 0.8, main = "Overlook of the multishapes dataset")
# legend("topright", legend=c(1,2,3,4,5,6),
#        col=c(1,2,3,4,5,6), pch=19, cex=0.8)

################ dbscan
# table<- multishapes[, 1:2]
# dbscan_res <- dbscan(table, eps = 0.15, minPts = 5)
# plot(table, col=dbscan_res$cluster+1, main="DBSCAN just for reference")

################ hdbscan
# cl <- dbscan::hdbscan(table, minPts = MCS)
# plot(table, col = cl$cluster + 1, pch = 20, cex = 1.9, main ="Standard Lib HDBSCAN Benchmarker")

moons<- multishapes[, 1:2]
DATA_SIZE <- 1100


# moons <- moons[, 1:2]
# DATA_SIZE <- 100

# moons <- DS3[,1:2]
# DATA_SIZE <- 1000


MCS <- 5
Labels <- 1:DATA_SIZE
table <- moons[1:DATA_SIZE,]


par(mfrow=c(1,2))


# print(nearest)

# cl2 <- hdbscan(table, minPts = 25)
# plot(table, col=cl2$cluster+1,
#        pch=ifelse(cl2$cluster == 0, 8, 1), # Mark noise as star
#        cex=ifelse(cl2$cluster == 0, 0.5, 0.75), # Decrease size of noise
#        xlab=NA, ylab=NA, main =paste("`dbscan` lib in RCran"), cex.main=1.5)
#   # colors <- sapply(1:length(cl2$cluster),
#   #                  function(i) adjustcolor(palette()[(cl2$cluster+1)[i]], alpha.f = cl2$membership_prob[i]))
#   points(table, col=colors, pch=20)
#
# cl2 <- HierarchicalTree$new(table, min_cluster_size = 25)
# plot(table, col=cl2$cluster+1,
#        pch=ifelse(cl2$cluster == 0, 8, 1), # Mark noise as star
#        cex=ifelse(cl2$cluster == 0, 0.5, 0.75), # Decrease size of noise
#        xlab=NA, ylab=NA, main =paste("`My HDBSCAN Implement"), cex.main=1.5)
#   # colors <- sapply(1:length(cl2$cluster),
#   #                  function(i) adjustcolor(palette()[(cl2$cluster+1)[i]], alpha.f = cl2$membership_prob[i]))
#   points(table, col=colors, pch=20)


# dbscan_res <- dbscan(table, eps = 0.15, minPts = 5)
# plot(table, col=dbscan_res$cluster+1, main="DBSCAN just for reference")

########################################## hdbscan lib and my implement ####################
# cl <- dbscan::hdbscan(table, minPts = MCS)
# plot(table, col = cl$cluster + 1, pch=ifelse(cl2$cluster == 0, 8, 20), cex.main=1.5,# Mark noise as star,
#            cex = 1.9, main =" dbscan Lib's hdbscan method as Benchmarker")
#
# ht <- HierarchicalTree$new(table, min_cluster_size = MCS) # 80/20 looks good
# plot(table, col = ht$cluster + 1, pch=ifelse(cl2$cluster == 0, 8, 20), # Mark noise as star,
#            cex = 1.9, main =paste("My HDBSCAN Implement\n MCS = ", ht$minPts , " minPts = ", ht$minPtsForCoreDist), cex.main=1.5)

par(mfrow=c(2,3))
MCS <- 5
ht <- HierarchicalTree$new(table, min_cluster_size = MCS, min_points = 5) # 80/20 looks good
plot(table, col = ht$cluster + 1, pch = 20, cex = 1.9, main =paste("My HDBSCAN Implement\n MCS = ", ht$minPts , " minPts = ", ht$minPtsForCoreDist), cex.main=1.5)

MCS <- 15
ht <- HierarchicalTree$new(table, min_cluster_size = MCS, min_points = 5) # 80/20 looks good
plot(table, col = ht$cluster + 1, pch = 20, cex = 1.9, main =paste("My HDBSCAN Implement\n MCS = ", ht$minPts , " minPts = ", ht$minPtsForCoreDist), cex.main=1.5)

MCS <- 50
ht <- HierarchicalTree$new(table, min_cluster_size = MCS, min_points = 5) # 80/20 looks good
plot(table, col = ht$cluster + 1, pch = 20, cex = 1.9, main =paste("My HDBSCAN Implement\n MCS = ", ht$minPts , " minPts = ", ht$minPtsForCoreDist), cex.main=1.5)

MCS <- 50
ht <- HierarchicalTree$new(table, min_cluster_size = MCS, min_points = 25) # 80/20 looks good
plot(table, col = ht$cluster + 1, pch = 20, cex = 1.9, main =paste("My HDBSCAN Implement\n MCS = ", ht$minPts , " minPts = ", ht$minPtsForCoreDist), cex.main=1.5)

MCS <- 50
ht <- HierarchicalTree$new(table, min_cluster_size = MCS, min_points = 40) # 80/20 looks good
plot(table, col = ht$cluster + 1, pch = 20, cex = 1.9, main =paste("My HDBSCAN Implement\n MCS = ", ht$minPts , " minPts = ", ht$minPtsForCoreDist), cex.main=1.5)

MCS <- 50
ht <- HierarchicalTree$new(table, min_cluster_size = MCS, min_points = 50) # 80/20 looks good
plot(table, col = ht$cluster + 1, pch = 20, cex = 1.9, main =paste("My HDBSCAN Implement\n MCS = ", ht$minPts , " minPts = ", ht$minPtsForCoreDist), cex.main=1.5)