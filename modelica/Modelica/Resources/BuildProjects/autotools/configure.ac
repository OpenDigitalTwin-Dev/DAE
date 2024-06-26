dnl Init
AC_PREREQ([2.63])
AC_INIT([Modelica Standard Library Tables],[4.1.0],[https://github.com/modelica/ModelicaStandardLibrary],[libmodelicastandardtables],[https://modelica.org])

AM_INIT_AUTOMAKE([subdir-objects])
AC_CONFIG_MACRO_DIR([m4])

AC_LANG([C])
AC_PROG_CC
AC_PROG_CPP
AC_PROG_MAKE_SET
LT_INIT([disable-static])
AC_SUBST(LIBZLIB)
AC_SUBST(LIBHDF5)
AC_SUBST(LIBMATH)
dnl Check for HDF5, etc

LIBS_BEFORE="$LIBS"
LIBS=""
AC_SEARCH_LIBS([atan2], [m], [LIBMATH="$LIBS"], [
  AC_MSG_ERROR([unable to find the atan2() function])
])
LIBS="$LIBS_BEFORE"

AC_DEFUN([C_FLAG_CHECK_AND_ADD],
[dnl
  AC_MSG_CHECKING([if $CC supports $1])
  AC_LANG_PUSH([C])
  ac_saved_cflags="$CFLAGS"
  CFLAGS="-Werror $1"
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([])],
    [AC_MSG_RESULT([yes]) ; CFLAGS="$ac_saved_cflags $1"],
    [AC_MSG_RESULT([no]) ; CFLAGS="$ac_saved_cflags"]
  )
  AC_LANG_POP([C])
])

C_FLAG_CHECK_AND_ADD([-fno-delete-null-pointer-checks])
C_FLAG_CHECK_AND_ADD([-Werror=implicit-function-declaration])

AC_SEARCH_LIBS([floor],[m])

LIBS_BEFORE="$LIBS"
ZLIB="Yes"
STATIC_ZLIB="No"
DUMMY_USERTAB=""
AC_ARG_ENABLE(hdf5,
  [  --enable-hdf5[=yes]          Adds support for HDF5 and MAT7.3 formats],[test "xno" = x"$enableval" && HDF5="No"],[HDF5="Yes"])
AC_ARG_ENABLE(zlib,
  [  --enable-zlib[=yes]          Adds support for the MAT7 format],[test "xno" = x"$enableval" && ZLIB="No"],[ZLIB="Yes"])
AC_ARG_ENABLE(shared-tables,
  [  --enable-shared-tables[=yes] Adds support for shared tables, reducing file I/O and reducing memory footprint],[test "xyes" = x"$enableval" && SHARED_TABLES="Yes"],[SHARED_TABLES="Yes"])
AC_ARG_ENABLE(static-zlib, [  --enable-static-zlib[=no]],[test "xyes" = x"$enableval" && STATIC_ZLIB="Yes"],[STATIC_ZLIB="No"])

if test "$ZLIB" = "Yes"; then
  AC_CHECK_HEADERS(zlib.h,[AC_SEARCH_LIBS([gzclose], [z], [ZLIB="Yes"], [ZLIB="No"])],[ZLIB="No"])
  if test "$ZLIB" = "No" && test -d ../../C-Sources/zlib; then
    ZLIB="Yes, from sources"
  else
    LIBZLIB="$LIBS"
  fi
fi
LIBS="$LIBS_BEFORE"

AM_CONDITIONAL([INCLUDEZLIB], [test "$ZLIB" = "Yes, from sources" -o "$STATIC_ZLIB" = "Yes"])

if test "$ZLIB" = "No" -o "$HDF5" = "No"; then
  HDF5="No"
else
AC_CHECK_PROG(MPICC,mpicc,[yes])
AC_MSG_CHECKING([hdf5.h])
for HDF_FIRST in Yes No; do
  for d in serial mpich openmpi; do
    if test -d /usr/include/hdf5/$d; then
      CPPFLAGS="$CPPFLAGS -I/usr/include/hdf5/$d"
      break
    fi
  done
  AC_PREPROC_IFELSE([AC_LANG_PROGRAM([[#include <hdf5.h>]],[])],AC_MSG_RESULT([OK]),[
    if ! test -z "$MPICC"; then
      CPPFLAGS_BAK=$CPPFLAGS
      MPI_CPPFLAGS=`OMPI_CC=$CC mpicc -showme:compile`
      CPPFLAGS="$CPPFLAGS $MPI_CPPFLAGS"
      AC_PREPROC_IFELSE([AC_LANG_PROGRAM([[#include <hdf5.h>]],[])],AC_MSG_RESULT([[OK, needs mpi.h]]),[CPPFLAGS=$CPPFLAGS_BAK; HDF5="No"])
    fi
    if test "No" = "$HDF5"; then
      HDF5="Yes"
      if test "$HDF_FIRST" = "Yes" && test "$host" = "$build" && apt-cache show libhdf5-dev > /dev/null 2>&1; then
        HDF5_INSTALL_CMD="apt-get install libhdf5-dev"
        if gksudo --description "HDF5 is an optional dependency of ModelicaMatIO used to read/write MATLAB 7.3 files. $HDF5_INSTALL_CMD" $HDF5_INSTALL_CMD; then
          continue
        else
          HDF5="No";
          AC_MSG_RESULT([failed])
        fi
      else
        HDF5="No";
        AC_MSG_RESULT([failed])
      fi
    fi
  ])
  break
done
fi

# Ok, so we have hdf5.h. Now for the fun stuff:
# We need to look for the 1.8 version of the API, but it is not always enabled by default even if the library supports it.
# So we first try to compile as usual. If that does not work, add a whole bunch of compatibility #defines and try again.
if test "$HDF5" = "Yes"; then

LIBS_BEFORE="$LIBS"
AC_SEARCH_LIBS([H5Eget_auto2],[hdf5 hdf5_serial hdf5_mpich hdf5_openmpi],[HDF5="Yes"],[HDF5="No"])
AC_MSG_CHECKING([default API version])
CPPFLAGS_BEFORE="$CPPFLAGS"
FLAGS=`test "$HDF5" = "Yes" && echo 1.8 1.6`
HDF5="No"
for FLAG in $FLAGS; do
  if test "$HDF5" = "Yes"; then
    break
  elif test "$FLAG" = "1.6"; then
    CPPFLAGS="$CPPFLAGS -DH5Acreate_vers=2 -DH5Aiterate_vers=2 -DH5Dcreate_vers=2 -DH5Dopen_vers=2 -DH5Eclear_vers=2 -DH5Eprint_vers=2 -DH5Epush_vers=2 -DH5Eset_auto_vers=2 -DH5Eget_auto_vers=2 -DH5Ewalk_vers=2 -DH5Gcreate_vers=2 -DH5Gopen_vers=2 -DH5Pget_filter_vers=2 -DH5Pget_filter_by_id_vers=2 -DH5Pinsert_vers=2 -DH5Pregister_vers=2 -DH5Rget_obj_type_vers=2 -DH5Tarray_create_vers=2 -DH5Tcommit_vers=2 -DH5Tget_array_dims_vers=2 -DH5Topen_vers=2";
  fi
  AC_TRY_LINK([
#include <stdio.h>
#include <stdlib.h>
#include <hdf5.h>],
[H5Eget_auto(0,NULL,NULL)],
[HDF5="Yes";AC_MSG_RESULT([$FLAG]);break],
[CPPFLAGS=$CPPFLAGS_BEFORE])
done

if test "$HDF5" = "No"; then
  AC_MSG_RESULT([1.8 API not available])
else
  LIBHDF5="$LIBS"
fi
LIBS=$LIBS_BEFORE

fi

AC_CHECK_HEADERS(dirent.h,[],[CPPFLAGS="$CPPFLAGS -DNO_FILE_SYSTEM"])
AC_CHECK_HEADERS(locale.h,[],[CPPFLAGS="$CPPFLAGS -DNO_LOCALE"])
AC_CHECK_HEADERS(time.h,[],[CPPFLAGS="$CPPFLAGS -DNO_TIME"])
AC_MSG_CHECKING([for getpid()])
AC_TRY_LINK([
#include <unistd.h>
],
[getpid();],
[AC_MSG_RESULT([yes])],
[AC_MSG_RESULT([no]); CPPFLAGS="$CPPFLAGS -DNO_PID"])

AC_ARG_ENABLE(dummy-usertab, [  --disable-dummy-usertab[=no] Skips generation of the dummy usertab function],[
  if test "xyes" = x"$enableval"; then
    DUMMY_USERTAB="yes"
  fi
],[DUMMY_USERTAB="yes"])
if test "yes" = "$DUMMY_USERTAB"; then
  CPPFLAGS="$CPPFLAGS -DDUMMY_FUNCTION_USERTAB=1"
  STATIC_CPPFLAGS="$CPPFLAGS -DDUMMY_FUNCTION_USERTAB=1"
fi
if test "$ZLIB" = "Yes"; then
CPPFLAGS="$CPPFLAGS -DHAVE_ZLIB=1"
fi
if test "$HDF5" = "Yes"; then
CPPFLAGS="$CPPFLAGS -DHAVE_HDF5=1"
fi
if test "$SHARED_TABLES" = "Yes"; then
CPPFLAGS="$CPPFLAGS -DTABLE_SHARE=1"
fi

echo "ModelicaMatIO may use ZLIB: $ZLIB"
echo "ModelicaMatIO may use HDF5: $HDF5"
echo "ModelicaMatIO may use MAT7.3: $HDF5"

if test "$libdir" = '${exec_prefix}/lib'; then
  # It is hard to detect where to put the libraries if we cross-compile. Let the tool decide where to put it.
  # Use Resources/Library/ as the default.
  libdir=`pwd`/../../Library/
fi

AC_OUTPUT([Makefile])
