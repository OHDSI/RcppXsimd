#' @useDynLib RcppXsimd, .registration = TRUE
#' @importFrom Rcpp sourceCpp
NULL

#' Determine if CPU supports SSE42 SIMD instructions
#' 
#' @return Boolean
#' 
#' @export
supportsSSE42 <- function() {
  getSimdFeatures()$HW_SSE42
}

#' Determine if CPU supports AVX SIMD instructions
#' 
#' @return Boolean
#' 
#' @export
supportsAVX <- function() {
  getSimdFeatures()$HW_AVX
}

#' Determine if CPU supports AVX512 SIMD instructions
#' 
#' @return Boolean
#' 
#' @export
supportsAVX512 <- function() {
  getSimdFeatures()$HW_AVX512_F
}

.onUnload <- function (libpath) {
  library.dynam.unload("RcppXsimd", libpath)
}