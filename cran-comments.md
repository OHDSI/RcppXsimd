## Resubmission following email from Uwe Ligges on May 17 asking:

* "we see you have declared this is licensed under the MIT license, but parts of your
package have different license statements, pelase make sure you can relicense under MIT
and if permission is needed that you have permission from all copyright holders."

  - Response: I have received conflicting information from UCLA Legal.  To simplify
  matters, I have changed the license to BSD_3_clause to match the Xsimd license.
  FeatureDetector is licensed under CC0 1.0 Universal that falls under BSD_3_clause usage.

* "Can you add a link ... to the Xsimd library in your Description field"

  - Response: Done. See DESCRIPTION

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
