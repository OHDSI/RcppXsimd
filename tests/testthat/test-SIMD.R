library(RcppXsimd)

context("test-SIMD.R")

test_that("SSE", {
  if (supportsSSE()) {
    Sys.setenv(PKG_CPPFLAGS = getSSEFlags())
    Rcpp::sourceCpp(verbose = TRUE, code='
      // [[Rcpp::plugins(cpp14)]]
      // [[Rcpp::depends(RcppXsimd)]]
                    
      #include <Rcpp.h>
      #include "xsimd/xsimd.hpp"
                    
      // [[Rcpp::export]] 
      void demoSSE42() {
        xsimd::batch<double, 2> a(1.0);
        xsimd::batch<double, 2> b(1.0);
        Rcpp::Rcout << a << " + " << b << " = " << (a + b) << std::endl;
    }')
    demoSSE42()
  } else {
    message("SSE4.2 is not supported")
  }
  
})

test_that("AVX", {
  if (supportsAVX()) {
    Sys.setenv(PKG_CPPFLAGS = getAVXFlags())
    Rcpp::sourceCpp(verbose = TRUE, code='
    // [[Rcpp::plugins(cpp14)]]
    // [[Rcpp::depends(RcppXsimd)]]
                    
    #include <Rcpp.h>
    #include "xsimd/xsimd.hpp"
                    
    // [[Rcpp::export]] 
    void demoAVX() {
      xsimd::batch<double, 4> a(1.0);
      xsimd::batch<double, 4> b(1.0);
      Rcpp::Rcout << a << " + " << b << " = " << (a + b) << std::endl;
    }')
    demoAVX()
  } else {
    message("AVX is not supported")
  }  
})

test_that("AVX512", {
  if (supportsAVX512()) {
    Sys.setenv(PKG_CPPFLAGS = getAVX512Flags())
    Rcpp::sourceCpp(verbose = TRUE, code='
    // [[Rcpp::plugins(cpp14)]]
    // [[Rcpp::depends(RcppXsimd)]]
                    
    #include <Rcpp.h>
    #include "xsimd/xsimd.hpp"
                    
    // [[Rcpp::export]] 
    void demoAVX512() {
      xsimd::batch<double, 8> a(1.0);
      xsimd::batch<double, 8> b(1.0);
      Rcpp::Rcout << a << " + " << b << " = " << (a + b) << std::endl;
    }')
    demoAVX512()
  } else {
    message("AVX512 is not supported")
  }  
})