RcppXsimd
=======

[![CRAN_Status_Badge](http://www.r-pkg.org/badges/version/RcppXsimd)](https://CRAN.R-project.org/package=RcppXsimd)

Overview
============

This library provides an `Rcpp` wrapper around [Xsimd](https://github.com/QuantStack/xsimd) that is a modern, portable `C++14` header-only library for SIMD intrinsics and parallelized, optimized math implementations.

Examples
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

Technology
============
Cyclops in an R package, with most functionality implemented in C++. Cyclops uses cyclic coordinate descent to optimize the likelihood function, which makes use of the sparse nature of the data.

System Requirements
===================
Requires R (version 3.1.0 or higher). Installation on Windows requires [RTools]( https://CRAN.R-project.org/bin/windows/Rtools/) (`devtools >= 1.12` required for RTools34, otherwise RTools33 works fine).

Dependencies
============
 * There are no dependencies.

Getting Started
===============
1. On Windows, make sure [RTools](https://CRAN.R-project.org/bin/windows/Rtools/) is installed.
2. In R, use the following commands to download and install Cyclops:

  ```r
  install.packages("devtools")
  library(devtools)
  install_github("ohdsi/Cyclops")
  ```

3. To perform a Cyclops model fit, use the following commands in R:

  ```r
  library(Cyclops)
  cyclopsData <- createCyclopsDataFrame(formula)
  cyclopsFit <- fitCyclopsModel(cyclopsData)
  ```

Getting Involved
================
* Package manual: [Cyclops manual](https://raw.githubusercontent.com/OHDSI/Cyclops/master/extras/Cyclops.pdf)
* Developer questions/comments/feedback: <a href="http://forums.ohdsi.org/c/developers">OHDSI Forum</a>
* We use the <a href="../../issues">GitHub issue tracker</a> for all bugs/issues/enhancements

License
=======
Cyclops is licensed under Apache License 2.0.   Cyclops contains the TinyThread libray.

The TinyThread library is licensed under the [zlib/libpng](https://opensource.org/licenses/Zlib) license as described [here](http://tinythreadpp.bitsnbites.eu).


Development status
===========

[![Build Status](https://travis-ci.org/OHDSI/RcppXsimd.svg?branch=master)](https://travis-ci.org/OHDSI/RcppXsimd)

Acknowledgements
================
- This project is supported in part through the National Science Foundation grant DMS 1264153.
