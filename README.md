RcppXsimd
=======

[![Build Status](https://github.com/ohdsi/RcppXsimd/actions/workflows/R_CMD_check_Hades.yaml/badge.svg)](https://github.com/OHDSI/RcppXsimd/actions?query=workflow%3AR-CMD-check)
[![CRAN_Status_Badge](https://www.r-pkg.org/badges/version/RcppXsimd)](https://CRAN.R-project.org/package=RcppXsimd)
[![CRAN_Status_Badge](https://cranlogs.r-pkg.org/badges/RcppXsimd)](https://cran.r-project.org/package=RcppXsimd)

Overview
============

This library provides an `Rcpp` wrapper around [Xsimd](https://github.com/xtensor-stack/xsimd) that is a modern, portable `C++14` header-only library for SIMD intrinsics and parallelized, optimized math implementations.

Example
========

```r
library(RcppXsimd)
?supportsAVX

if (supportsAVX()) {
  Sys.setenv(PKG_CPPFLAGS = "-mavx")
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
```

Development status
===========

RcppXsimd is being developed in R Studio.

Acknowledgements
================
- This project is supported in part through the National Science Foundation grant DMS 1264153.
