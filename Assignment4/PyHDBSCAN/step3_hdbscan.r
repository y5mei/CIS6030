foldername<-'.'
library(r2r, lib.loc=foldername)
source("HierarchicalTree.R")

######################################################################################
# read file back from HFDS as datasets
######################################################################################
result <- system("docker exec namenode hdfs dfs -cat /iris.csv", intern = TRUE)
writeLines(result, "temp_iris.csv")
iris_dataset <- read.csv("temp_iris.csv", header=FALSE)
colnames(iris_dataset) <- c("Sepal.Length", "Sepal.Width", "Petal.Length","Petal.Width", "Species")

result <- system("docker exec namenode hdfs dfs -cat /multishapes.csv", intern = TRUE)
writeLines(result, "temp_multishapes.csv")
multishapes_dataset <- read.csv("temp_multishapes.csv", header=TRUE)
# print(head(multishapes_dataset, n=8))

file.remove("temp_multishapes.csv")
file.remove("temp_iris.csv")

######################################################################################
# HDBSCAN plots
######################################################################################

iris <- iris_dataset
multishapes <- multishapes_dataset[-c(1)] # drop the index col

pdf("Hdbscan_Plot.pdf")

table <- iris
MCS = 5
par(mfrow=c(2,2))
ht <- HierarchicalTree$new(table, min_cluster_size = MCS) # 80/20 looks good
plot(table[,1], table[,2], col = ht$cluster + 1, pch = 20, cex = 1.9, xlab=colnames(iris)[1], ylab=colnames(iris)[2],
main =paste("My HDBSCAN Implement for IRIS\n MCS = ", ht$minPts , " minPts = ", ht$minPtsForCoreDist), cex.main=1.1)

plot(table[,3], table[,4], col = ht$cluster + 1, pch = 20, cex = 1.9, xlab=colnames(iris)[3], ylab=colnames(iris)[4],
main =paste("My HDBSCAN Implement for IRIS\n MCS = ", ht$minPts , " minPts = ", ht$minPtsForCoreDist), cex.main=1.1)

plot(table[,1], table[,3], col = ht$cluster + 1, pch = 20, cex = 1.9, xlab=colnames(iris)[1], ylab=colnames(iris)[3],
main =paste("My HDBSCAN Implement for IRIS\n MCS = ", ht$minPts , " minPts = ", ht$minPtsForCoreDist), cex.main=1.1)

plot(table[,2], table[,4], col = ht$cluster + 1, pch = 20, cex = 1.9, xlab=colnames(iris)[2], ylab=colnames(iris)[4],
main =paste("My HDBSCAN Implement for IRIS\n MCS = ", ht$minPts , " minPts = ", ht$minPtsForCoreDist), cex.main=1.1)


table <- multishapes
par(mfrow=c(2,2))

MCS <- 15
ht <- HierarchicalTree$new(table, min_cluster_size = MCS, min_points = 5) # 80/20 looks good
plot(table[,1], table[,2], col = ht$cluster + 1, pch = 20, cex = 1.9, 
main =paste("My HDBSCAN Implement for Multishapes\n MCS = ", ht$minPts , " minPts = ", ht$minPtsForCoreDist), cex.main=1.1)

ht <- HierarchicalTree$new(table, min_cluster_size = MCS, min_points = 25) # 80/20 looks good
plot(table[,1], table[,2], col = ht$cluster + 1, pch = 20, cex = 1.9, 
main =paste("My HDBSCAN Implement for Multishapes\n MCS = ", ht$minPts , " minPts = ", ht$minPtsForCoreDist), cex.main=1.1)

ht <- HierarchicalTree$new(table, min_cluster_size = MCS, min_points = 40) # 80/20 looks good
plot(table[,1], table[,2], col = ht$cluster + 1, pch = 20, cex = 1.9, 
main =paste("My HDBSCAN Implement for Multishapes\n MCS = ", ht$minPts , " minPts = ", ht$minPtsForCoreDist), cex.main=1.1)

ht <- HierarchicalTree$new(table, min_cluster_size = MCS, min_points = 50) # 80/20 looks good
plot(table[,1], table[,2], col = ht$cluster + 1, pch = 20, cex = 1.9, 
main =paste("My HDBSCAN Implement for Multishapes\n MCS = ", ht$minPts , " minPts = ", ht$minPtsForCoreDist), cex.main=1.1)


MCS <- 49
ht <- HierarchicalTree$new(table, min_cluster_size = MCS, min_points = 5) # 80/20 looks good
plot(table[,1], table[,2], col = ht$cluster + 1, pch = 20, cex = 1.9, 
main =paste("My HDBSCAN Implement for Multishapes\n MCS = ", ht$minPts , " minPts = ", ht$minPtsForCoreDist), cex.main=1.1)

ht <- HierarchicalTree$new(table, min_cluster_size = MCS, min_points = 25) # 80/20 looks good
plot(table[,1], table[,2], col = ht$cluster + 1, pch = 20, cex = 1.9, 
main =paste("My HDBSCAN Implement for Multishapes\n MCS = ", ht$minPts , " minPts = ", ht$minPtsForCoreDist), cex.main=1.1)

ht <- HierarchicalTree$new(table, min_cluster_size = MCS, min_points = 40) # 80/20 looks good
plot(table[,1], table[,2], col = ht$cluster + 1, pch = 20, cex = 1.9, 
main =paste("My HDBSCAN Implement for Multishapes\n MCS = ", ht$minPts , " minPts = ", ht$minPtsForCoreDist), cex.main=1.1)

ht <- HierarchicalTree$new(table, min_cluster_size = MCS, min_points = 50) # 80/20 looks good
plot(table[,1], table[,2], col = ht$cluster + 1, pch = 20, cex = 1.9, 
main =paste("My HDBSCAN Implement for Multishapes\n MCS = ", ht$minPts , " minPts = ", ht$minPtsForCoreDist), cex.main=1.1)


# par(mfrow=c(1,1))
# table <- multishapes[-c(1000:1050), ]
# ht <- HierarchicalTree$new(table, min_cluster_size = 10, min_points = 60) # 80/20 looks good
# plot(table[,1], table[,2], col = ht$cluster + 1, pch = 20, cex = 1.9, 
# main =paste("My HDBSCAN Implement for Multishapes Without Noise\n MCS = ", ht$minPts , " minPts = ", ht$minPtsForCoreDist), cex.main=1.1)

dev.off()
system('open Hdbscan_Plot.pdf')
print("==========================================================================================================")
print(">> HDBSCAN step is finsihed, the dataset has been read from HDBSAN, and each time read a database, the K-MEAN plot")
print("   is saved as a PDF file with name Hdbscan_Plot.pdf. ")
print("==========================================================================================================")
q()