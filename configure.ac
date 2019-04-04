# Process this file with autoconf to produce a configure script.
AC_INIT(RcppXsimd, 0.1, msuchard@ucla.edu)

# find R home and set CC/CFLAGS
: ${R_HOME=`R RHOME`}
if test -z "${R_HOME}"; then
  echo "could not determine R_HOME"
  exit 1
fi

# platform check
RHOST=`"${R_HOME}/bin/Rscript" -e 'cat(Sys.info()[["sysname"]])'`

# Check for which host we are on and setup a few things
# specifically based on the host

SIMD_FLAGS="-mfma -msse4.2 -mavx"

AC_SUBST(SIMD_FLAGS)

AC_CONFIG_FILES([src/Makevars])
AC_OUTPUT
