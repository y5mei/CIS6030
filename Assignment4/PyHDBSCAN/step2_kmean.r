library(ggplot2)
library(tidyverse)
library(cluster)
library(factoextra)

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
# kmean plots
######################################################################################

iris <- iris_dataset
multishapes <- multishapes_dataset[-c(1)] # drop the index col

pdf("KMean_Plot.pdf")
# par(mfrow=c(2,2))
irisCluster <- kmeans(iris[,1:4], center=3, nstart=50)
fviz_cluster(irisCluster, data = iris[,1:2], shape = 19, geom = "point", main = "K-Mean for Iris")
fviz_cluster(irisCluster, data = iris[,3:4], shape = 19, geom = "point", main = "K-Mean for Iris")
fviz_cluster(irisCluster, data = iris[,c(1,3)], shape = 19, geom = "point", main = "K-Mean for Iris")
fviz_cluster(irisCluster, data = iris[,c(2,4)], shape = 19, geom = "point", main = "K-Mean for Iris")

# fviz_cluster(irisCluster[, 2:4], shape = 19,geom = "point", main = "K-Mean for Iris")

# confusion matrix
# table(irisCluster$cluster, iris$Species)

multishapesCluster <- kmeans(multishapes[, 1:2], center=5, nstart=50)
fviz_cluster(multishapesCluster, data = multishapes[,1:2], shape = 19, geom = "point", main = "K-Mean for multishapes")

dev.off()
system('open KMean_Plot.pdf')
print("==========================================================================================================")
print(">> KMEAN step is finsihed, the dataset has been read from HDBSAN, and each time read a database, the K-MEAN plot")
print("   is saved as a PDF file with name KMean_Plot.pdf. ")
print("==========================================================================================================")
q()