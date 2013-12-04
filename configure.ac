AC_PREREQ([2.69])
AC_INIT([odeint_bloch], [0.1], [kaveh@codeare.org])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([src/config.h])

AM_INIT_AUTOMAKE([-Wall -Werror])
AM_MAINTAINER_MODE
AC_CANONICAL_HOST

AC_PROG_CXX
AC_LANG([C++])
AX_COMPILER_VENDOR

AC_CHECK_HEADERS([H5Cpp.h],          [], 
   [AC_MSG_RESULT([cannot find H5Cpp.h, hdf5 dump / read functionality unavailable!])])
AC_CHECK_LIB([hdf5], [H5open])
AC_MSG_CHECKING([for getOrder in -lhdf5_cpp])
OLD_LIBS=$LIBS
LIBS="${LIBS} -lhdf5_cpp"

AC_TRY_LINK([#include <H5Cpp.h>],
   [
     H5::H5File file ("test.bin", H5F_ACC_RDONLY); 
	 H5::DataSet dataset = file.openDataSet("IntArray"); 
	 H5::IntType intype = dataset.getIntType(); 
	 H5std_string order_string; 
	 H5T_order_t order = intype.getOrder( order_string );
   ],
   [AC_MSG_RESULT([yes])]
   hdf5_found=yes,
   [LIBS=$OLD_LIBS]
   [AC_MSG_RESULT([no. Cannot find libhdf5_cpp, hdf5 dump/read not available.])])

if test -z $hdf5_found; then
    LIBS=$OLD_LIBS
fi

AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_UINT32_T
AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.
AC_CHECK_FUNCS([memset sqrt])

# Checks for header files.
case $host in
*darwin* | *rhapsody* | *macosx*)
    AC_CHECK_HEADERS([OpenCL/opencl.h])
    #LIBS="$LIBS -framework OpenCL"
    ;;
*)
    AC_CHECK_HEADERS([CL/opencl.h])
    #LIBS="$LIBS -lOpenCL"
    ;;
esac

# Time & Co for FFTW cycle
AC_CHECK_TYPE([hrtime_t],[AC_DEFINE(HAVE_HRTIME_T, 1, [Define to 1 if hrtime_t is defined in <sys/time.h>])],,[
#if HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
])
AC_CHECK_FUNCS([gethrtime read_real_time time_base_to_time clock_gettime mach_absolute_time])
dnl Cray UNICOS _rtc() (real-time clock) intrinsic
AC_MSG_CHECKING([for _rtc intrinsic])
rtc_ok=yes
AC_TRY_LINK([
#ifdef HAVE_INTRINSICS_H
#include <intrinsics.h>
#endif
], [_rtc()], [AC_DEFINE(HAVE__RTC,1,[Define if you have the UNICOS _rtc() intrinsic.])], [rtc_ok=no])
AC_MSG_RESULT($rtc_ok)

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
