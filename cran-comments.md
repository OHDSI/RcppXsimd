## Package patch following email from Brian Ripley noting compiler deprecation of `c++14`
* Removed compilations under under `c++14`
* Modernized `DESCRIPTION` file

## Test environments
* local macOS install (arm64), R 4.4
* rhub (multiple architectures), R 4.5
* win-builder (devel, release, oldrel)

## R CMD check results
* There were no ERRORs, WARNINGs or NOTEs on most systems.
* On windows, we see:
		Compilation used the following non-portable flag(s):
		'-mavx' '-mfma' '-msse4.2'
  which is as expected.

## Downstream dependencies
There are currently no downstream dependencies.
