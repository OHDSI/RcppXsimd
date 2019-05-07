RcppXsimd
=======

[![CRAN_Status_Badge](http://www.r-pkg.org/badges/version/RcppXsimd)](https://CRAN.R-project.org/package=RcppXsimd)

Overview
============

This library provides an `Rcpp` wrapper around [Xsimd](https://github.com/QuantStack/xsimd) that is a modern, portable `C++14` header-only library for SIMD intrinsics and parallelized, optimized math implementations.

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

[![Build Status](https://travis-ci.org/OHDSI/RcppXsimd.svg?branch=master)](https://travis-ci.org/OHDSI/RcppXsimd)

[![Build status](https://ci.appveyor.com/api/projects/status/h6ucnf08yk08fbkk?svg=true)](https://ci.appveyor.com/project/andrewjholbrook/rcppxsimd)

Acknowledgements
================
- This project is supported in part through the National Science Foundation grant DMS 1264153.
