#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT(Arm_Simulator, 1.5, Guillaume.Huard@imag.fr)
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([foreign])
AC_CONFIG_SRCDIR([csapp.c])
AC_CONFIG_HEADER([config.h])

# Checks for programs.
AC_PROG_CC
AM_PROG_AS
AM_PROG_CC_C_O
AM_PROG_LEX

# Checks for libraries.

# Checks for header files.
AC_HEADER_STDC
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS([arpa/inet.h fcntl.h inttypes.h netdb.h netinet/in.h stdlib.h string.h strings.h sys/socket.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_MODE_T
AC_TYPE_OFF_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_INT8_T
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_HEADER_TIME

# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_GETPGRP
AC_FUNC_MALLOC
AC_FUNC_MMAP
AC_FUNC_REALLOC
AC_FUNC_SELECT_ARGTYPES
AC_FUNC_STAT
AC_CHECK_FUNCS([alarm bzero dup2 gethostbyaddr gethostbyname memset munmap select socket strerror])

AC_ARG_WITH([arm-host], 
  AS_HELP_STRING([--with-arm-host],
              [prefix for the cross compiler targetting the ARM plateform @<:@default=check@:>@])],
  [], [with_arm_host=check])
AS_CASE(["$with_arm_host"],
 [no], [],
 [check|yes], [
    AC_CHECK_PROGS([GCC_CROSS],[arm-elf-gcc arm-linux-gnueabi-gcc arm-none-eabi-gcc], [none])
    AS_CASE(["$GCC_CROSS"],
      [*arm-elf-gcc], [with_arm_host=arm-elf],
      [*arm-linux-gnueabi-gcc], [with_arm_host=arm-linux-gnueabi],
      [*arm-none-eabi-gcc], [with_arm_host=arm-none-eabi],
      [none], [
	AC_MSG_WARN([Cannot find a cross compiler for the ARM plateform.])
	AS_IF([test -x /opt/gnu/arm/bin/arm-eabi-gcc], 
	  [AC_MSG_ERROR([Check your PATH, /opt/gnu/arm/bin/arm-elf-gcc seems to be there])],
          [AC_MSG_ERROR([Check your installation and/or your PATH])])])],
 [])

if test x"$with_arm_host" != xno; then
  AC_MSG_NOTICE([enabling the Examples directory with $with_arm_host cross-compiler target])
  AC_CONFIG_FILES([Examples/Makefile])
fi
AM_CONDITIONAL([HAVE_ARM_COMPILER],[test x"$with_arm_host" != xno])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
