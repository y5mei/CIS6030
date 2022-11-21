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