import::from(R6, R6Class)
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
