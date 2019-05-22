## Resubmission following email from Martina Schmirl on May 22 stating: "The Date field is over a
* Done. See DESCRIPTION.

## New package information
This package imports the header-only `Xsimd` library that provides C++ SIMD
implementations of mathematical operations.  While there is very little executable
R code directly in this package, it is an important dependency of several high-performance
statistical packages that I am developing and plan to release via CRAN.

## Test environments
* local OS X install, R 3.5.1
* ubuntu 14.04 (on travis-ci), R 3.5.1, gcc 7.0
* win-builder (devel, release, oldrel)

## R CMD check results
* There were no ERRORs and WARNINGs.
* There are 2 NOTEs:
  - New submission
  - Possible misspellings in DESCRIPTION (of which, I believe, there are none)

## Downstream dependencies
There are currently no downstream dependencies.
