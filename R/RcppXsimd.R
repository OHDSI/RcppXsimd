#' RcppXsimd: Rcpp wrapper to Xsimd
#' 
#' The RcppXsimd package wrappers the header-only C++ Xsimd library that 
#' provides parallelized math implementations using SIMD
#' 
#' @docType package
#' @name RcppXsimd
#' @useDynLib RcppXsimd, .registration = TRUE
#' @importFrom Rcpp sourceCpp
NULL

#' Determine if CPU supports SSE SIMD instructions
#' 
#' @return Boolean
#' 
#' @examples 
#' \dontrun{
#' 
#' if (supportsSSE()) {
#'   Sys.setenv(PKG_CPPFLAGS = getSSEFlags())
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
#' }
#' 
#' @export
supportsSSE <- function() {
  getSimdFeatures()$HW_SSE
}

#' Determine if CPU supports AVX SIMD instructions
#' 
#' @return Boolean
#' 
#' @examples 
#' \dontrun{
#' 
#' if (supportsAVX()) {
#'   Sys.setenv(PKG_CPPFLAGS = getAVXFlags())
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
#' }
#' 
#' @export
supportsAVX <- function() {
  features <- getSimdFeatures()
  features$HW_AVX && features$OS_AVX
}

#' Determine if CPU supports AVX512 SIMD instructions
#' 
#' @return Boolean
#' 
#' @examples 
#' \dontrun{
#' 
#' if (supportsAVX512()) {
#'   Sys.setenv(PKG_CPPFLAGS = getAVX512Flags())
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
#' }
#' @export
supportsAVX512 <- function() {
  features <- getSimdFeatures()
  features$HW_AVX512_F && features$HW_AVX512_BW
}

#' Concatenate supported SSE compiler flags for system CPU
#' 
#' @return String for compiler flags
#' 
#' @export
getSSEFlags <- function() {
  sseCodesWithFlags <- c("HW_SSE", "HW_SSE2", "HW_SSE3", "HW_SSSE3", "HW_SSE4.1", "HW_SSE4.2")
  .makeFlags(getSimdFeatures(), sseCodesWithFlags)
}

#' Concatenate supported AVX compiler flags for system CPU
#' 
#' @return String for compiler flags
#' 
#' @export
getAVXFlags <- function() {
  avxCodesWithFlags <- c("HW_AVX", "HW_AVX2")
  paste(getSSEFlags(), .makeFlags(getSimdFeatures(), avxCodesWithFlags), collapse = " ")
}

#' Concatenate supported AVX512 compiler flags for system CPU
#' 
#' @return String for compiler flags
#' 
#' @export
getAVX512Flags <- function() {
  avx512CodesWithFlags <- c( "HW_AVX512_F", "HW_AVX512_PF", "HW_AVX512_ER", "HW_AVX512_CD", "HW_AVX512_VL", "HW_AVX512_BW", "HW_AVX512_DQ")
  paste(getAVXFlags(), .makeFlags(getSimdFeatures(), avx512CodesWithFlags), collapse = " ")
}

.makeFlags <- function(features, codes) {
  flags <- unlist(features[codes])
  flags <- names(flags[which(flags)])
  paste(sub("_", "", tolower(sub("HW_", "-m", flags))), collapse = " ")    
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