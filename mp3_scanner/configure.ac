#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.65])
AC_INIT([MP3 Scanner], [1.0], [acidrain1@gmail.com])
AC_CONFIG_AUX_DIR(acconfig)
AC_PREFIX_DEFAULT(/usr/local)
AC_CONFIG_SRCDIR([src/scanner.c])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([-Wall -Werror])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL

# Checks for library functions.
AC_CHECK_LIB([id3], [create_ID3])
AC_CHECK_LIB([pcre], [pcre_compile])
AC_CHECK_LIB([sqlite3], [sqlite3_libversion])
AC_CHECK_LIB([FLAC], [FLAC__format_sample_rate_is_valid])

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT

cat <<EOF

   MP3 Scanner configuration summary
   -------------------------
   MP3 Scanner version       : ${PACKAGE_VERSION}
   compiler                  : ${CC}
   preprocessor flags        : ${CPPFLAGS}
   compiler flags            : ${CFLAGS}
   linker flags              : ${LDFLAGS}


   MP3 Scanner ${PACKAGE_VERSION} configured.


Type \`make' to build MP3 Scanner. Type \`make install' to install MP3 Scanner.

EOF
