#' @useDynLib RcppXsimd, .registration = TRUE
#' @importFrom Rcpp sourceCpp
NULL

#' Determine if CPU supports SSE4.2 SIMD instructions
#' 
#' @return Boolean
#' 
#' @examples 
#' if (supportsSSE42()) {
#'   Sys.setenv(PKG_CPPFLAGS = "-msse4.2")
#'   Rcpp::sourceCpp(verbose = TRUE, code='
#'     // [[Rcpp::plugins(cpp14)]]
#'     // [[Rcpp::depends(RcppXsimd)]]
#'                
#'     #include <Rcpp.h>
#'     #include "xsimd/xsimd.hpp"
#'                
#'     // [[Rcpp::export]] 
#'     void demoSSE42() {
#'       xsimd::batch<double, 2> a(1.0);
#'       xsimd::batch<double, 2> b(1.0);
#'       Rcpp::Rcout << a << " + " << b << " = " << (a + b) << std::endl;
#'     }')
#'   demoSSE42()
#' } else {
#'   message("SSE4.2 is not supported")
#' }
#' 
#' @export
supportsSSE42 <- function() {
  getSimdFeatures()$HW_SSE42
}

#' Determine if CPU supports AVX SIMD instructions
#' 
#' @return Boolean
#' 
#' @examples 
#' if (supportsAVX()) {
#'   Sys.setenv(PKG_CPPFLAGS = "-mavx")
#'   Rcpp::sourceCpp(verbose = TRUE, code='
#'     // [[Rcpp::plugins(cpp14)]]
#'     // [[Rcpp::depends(RcppXsimd)]]
#'                
#'     #include <Rcpp.h>
#'     #include "xsimd/xsimd.hpp"
#'                
#'     // [[Rcpp::export]] 
#'     void demoAVX() {
#'       xsimd::batch<double, 4> a(1.0);
#'       xsimd::batch<double, 4> b(1.0);
#'       Rcpp::Rcout << a << " + " << b << " = " << (a + b) << std::endl;
#'     }')
#'   demoAVX()
#' } else {
#'   message("AVX is not supported")
#' }
#' 
#' @export
supportsAVX <- function() {
  getSimdFeatures()$HW_AVX
}

#' Determine if CPU supports AVX512 SIMD instructions
#' 
#' @return Boolean
#' 
#' @examples 
#' if (supportsAVX512()) {
#'   Sys.setenv(PKG_CPPFLAGS = "-mavx512f")
#'   Rcpp::sourceCpp(verbose = TRUE, code='
#'     // [[Rcpp::plugins(cpp14)]]
#'     // [[Rcpp::depends(RcppXsimd)]]
#'                
#'     #include <Rcpp.h>
#'     #include "xsimd/xsimd.hpp"
#'                
#'     // [[Rcpp::export]] 
#'     void demoAVX512() {
#'       xsimd::batch<double, 8> a(1.0);
#'       xsimd::batch<double, 8> b(1.0);
#'       Rcpp::Rcout << a << " + " << b << " = " << (a + b) << std::endl;
#'     }')
#'   demoAVX512()
#' } else {
#'   message("AVX512 is not supported")
#' }
#' 
#' @export
supportsAVX512 <- function() {
  getSimdFeatures()$HW_AVX512_F
}

# printPreprocessorOptions <- function() {
#   
#   getCode <- function(major, minor, patch) {
#     format(
#       ((((major) %% 100) * 10000000) + (((minor) %% 100) * 100000) + ((patch) %% 100000)),
#       scientific = FALSE
#     )
#   }
#   
#   cat(paste(
#     paste("SSE   ", getCode(1,0,0)),
#     paste("SSE2  ", getCode(2,0,0)),
#     paste("SSE3  ", getCode(3,0,0)),
#     paste("SSE42 ", getCode(4,2,0)),
#     paste("AVX   ", getCode(5,0,0)),
#     paste("AVX512", getCode(6,0,0)),
#     sep = "\n"
#   ))
# }

.onUnload <- function (libpath) {
  library.dynam.unload("RcppXsimd", libpath)
}