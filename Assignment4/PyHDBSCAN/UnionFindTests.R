source("UnionFind.R")
library(testthat)
library("dbscan")
import::from(factoextra, multishapes)

test_that("constructor works", {
  uf <- UF$new(5)
  expect_equal(uf$size, c(1,1,1,1,1))
  expect_equal(uf$parents, c(1,2,3,4,5))
})

test_that("union-find works", {
  # example from https://www.youtube.com/watch?v=9J-TKZgAr88
  uf <- UF$new(10)
  uf$union(5,4)
  uf$union(4,9)
  uf$union(7,6)
  uf$union(10, 5)
  uf$union(3,2)

  expect_equal(uf$find(9), uf$find(10))
  expect_false(uf$find(1) == uf$find(8))

  uf$union(6,1)
  uf$union(8,3)
  uf$union(7,2)
  uf$union(2,1)

  expect_equal(uf$find(1), uf$find(8))

})

test_that("tree node works", {
  t1 <- TreeNode$new(1)
  t2 <- TreeNode$new(2)
  t3 <- TreeNode$new(left = t1, right = t2, value = 100)
  expect_equal(t3$left, t1)
  expect_equal(t3$right, t2)
})

test_that("HierarchicalTree works for creating leaf nodes", {
  htree <- HierarchicalTree$new(5)
  expect_equal(htree$hashmap[[5]]$idx, 5)
  expect_equal(htree$hashmap[[4]]$idx, 4)
  expect_equal(htree$hashmap[[3]]$idx, 3)
  expect_equal(htree$hashmap[[2]]$idx, 2)
  expect_equal(htree$hashmap[[1]]$idx, 1)

  htree$build(4, 5, 10)
  expect_equal(htree$root$value, 10)
  htree$build(2, 3, 15)
  expect_equal(htree$root$value, 15)
  htree$build(3, 4, 25)
  expect_equal(htree$root$value, 25)
  expect_equal(htree$root$left$value, 15)
  expect_equal(htree$root$right$value, 10)
  htree$build(1, 5, 33)
  expect_equal(htree$root$value, 33)
  expect_equal(htree$root$left$value, 25)
  expect_equal(htree$root$right$value, NA)

  # make sure the union find's hash map is correct
  x <- c(htree$uf$find(1),htree$uf$find(2),htree$uf$find(3),htree$uf$find(4),htree$uf$find(5))
  expect_setequal(x, htree$uf$find(1))
})

test_that("HierarchicalTree works for condense based on MCS",{
  htree <- HierarchicalTree$new(5)
  htree$build(4, 5, 10)
  htree$build(2, 3, 15)
  htree$build(3, 4, 25)
  htree$build(1, 5, 33)

  htree$condense(minPts = 2)
  expect_equal(htree$root$num_child, 4)
  expect_equal(htree$root$left$num_child, 4)
  expect_equal(htree$root$right, NULL)
  expect_equal(htree$root$left$left$num_child, 2)
  expect_equal(htree$root$left$right$num_child, 2)
  expect_equal(htree$root$left$right$left$num_child, 1)
  expect_equal(htree$root$left$right$right$num_child, 1)
  expect_equal(htree$root$left$left$left$num_child, 1)
  expect_equal(htree$root$left$left$right$num_child, 1)

  # after condense, we can check the extrac with lambda
  htree$extract()

})

test_that("HDBSCAN works for direct supply mutrial reachability matrix",{
  l <- c(0,35,45,40,33,35,0,15,26,30,45,15,0,25,32,40,26,25,0,10, 33,30,32,10,0)
  g <- matrix(l, nrow = 5)
  hdbscan <- HDBSCAN$new(minPts=2, g = g)
  htree <- hdbscan$hTree
  expect_equal(htree$root$value, 33)
  expect_equal(htree$root$left$value, 25)
  expect_equal(htree$root$right, NULL)
  expect_equal(htree$root$left$left$value, 10)
  expect_equal(htree$root$left$right$value, 15)

  # check the num of children are correct
  expect_equal(htree$root$num_child, 4)
  expect_equal(htree$root$left$num_child, 4)
  expect_equal(htree$root$left$left$num_child, 2)
  expect_equal(htree$root$left$right$num_child, 2)
})

test_that("HDBSCAN works for providing a table",{
  #  import::from(factoextra, multishapes)
  #  data("multishapes")
  #  table <- multishapes
  # # newCol <- 1:1100
  # # table["predict"] <- newCol # append a new col
  # hdbscan <- HDBSCAN$new(table = table, minPts=5)
  # table <- hdbscan$table
  # plot(table[,1], table[, 2], col = table[, 3], pch = 19, cex = 0.8)
})

# data("moons")
# plot(moons, pch=20)
# cl <- hdbscan(moons, minPts = 19)
# plot(moons, col=cl$cluster+1, pch=19)




# Need to have a method that convert a Table of n rows more than 2 cols into a Multra Distance Matrix
   import::from(factoextra, multishapes)
   data("multishapes")
   table <- multishapes[1:15,]
#   # newCol <- 1:1100
#   # table["predict"] <- newCol # append a new col
  hdbscan <- HDBSCAN$new(table = table, minPts=5)
  # table <- hdbscan$table
#   plot(table[,1], table[, 2], col = table[, 3], pch = 19, cex = 0.8)