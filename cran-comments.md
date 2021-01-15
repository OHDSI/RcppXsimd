## Package patch following email from Brian Ripley noting compiler error with `gcc 11`
* Fixed compilations error under `gcc11` (missing #include <limits>)
* Added `inst/CITATION` file

## Test environments
* local OS X install (Intel), R 4.0.3
* local Apple M1 (Big Sur), R 4.0.3
* ubuntu 14.04 (on travis-ci), R 3.5.1, gcc 7.0
* win-builder (devel, release, oldrel)
* solaris 10, R-patched (experimental)

## R CMD check results
* There were no ERRORs, WARNINGs or NOTEs on most systems.
* On windows, we see:
		Compilation used the following non-portable flag(s):
		'-mavx' '-mfma' '-msse4.2'
  which is as expected.

## Downstream dependencies
There are currently no downstream dependencies.
