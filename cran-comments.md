## Package patch following email from Brian Ripley noting compiler error on Apple M1 hardware.
* Fixed.  Package now supports NEON (128bit wide) SIMD instructions provided via Xsimd
* Also modified testing on Windows x64

## Test environments
* local OS X install (Intel), R 4.0.3
* local Apple M1 (Big Sur), R 4.0.3
* ubuntu 14.04 (on travis-ci), R 3.5.1, gcc 7.0
* win-builder (devel, release, oldrel)
* solaris 10, R-patched (experimental)

## R CMD check results
* There were no ERRORs, WARNINGs or NOTEs.

## Downstream dependencies
There are currently no downstream dependencies.
