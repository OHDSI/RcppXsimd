## Package patch following email from Brian Ripley noting compiler error on Solaris
* Fixed.  Solaris C++ compiler defines type `char` == `int8_t` which led to recursively called constructors.  Such constructors are now #ifdef'd out for Solaris compilation
* Checked on: Oracle Solaris 10, x86, 32 bit, R-patched (experimental)

## Test environments
* local OS X install, R 3.5.1
* ubuntu 14.04 (on travis-ci), R 3.5.1, gcc 7.0
* win-builder (devel, release, oldrel)
* solaris 10, R-patched (experimental)

## R CMD check results
* There were no ERRORs and WARNINGs.
* There are 2 NOTEs:
  - New submission
  - Possible misspellings in DESCRIPTION (of which, I believe, there are none)

## Downstream dependencies
There are currently no downstream dependencies.
