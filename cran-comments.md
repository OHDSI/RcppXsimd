## Package patch following email from Brian Ripley noting warnings with some compilers
* removed deprecated use of `std::allocator<void>` in favor of `<class T> std::allocator<T>`

## Test environments
* local macOS install (arm64), R 4.4
* rhub (multiple architectures), R 4.5
* win-builder (devel, release, oldrel)

## R CMD check results
* There were no ERRORs, WARNINGs or NOTEs on all systems.

## Downstream dependencies
There are currently no downstream dependencies.
