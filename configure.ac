#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.57)
AC_INIT([opendbx], [1.4.0], [http://bugs.linuxnetworks.de/])
AC_CONFIG_SRCDIR([config.h.in])
AM_CONFIG_HEADER([config.h])
AM_INIT_AUTOMAKE
AM_MAINTAINER_MODE
AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.17])
AM_ICONV


# Library setup
APIVERSION="3:0:2"
AC_SUBST([APIVERSION])

APINUMBER="10200"
AC_SUBST([APINUMBER])

APIPLUSVERSION="1:0:0"
AC_SUBST([APIPLUSVERSION])

LIBVERSION=`echo $VERSION. | sed -e 's/[[^0-9.]]//g; s/\./  /g; s/ \([[0-9]]\) / 0\\1 /g; s/ //g'`
AC_SUBST([LIBVERSION])


# Environment specific settings
AC_SUBST([LIBPREFIX])
AC_SUBST([LIBSUFFIX])
LIBPREFIX="lib"
LIBSUFFIX=".so"
case "$host" in
	*-*-cygwin*)
		LIBPREFIX="cyg"
		LIBSUFFIX="-1.dll"
	;;
	*-*-mingw* | *-*-pw32*)
		AC_DEFINE([WIN32], 1, [Using Win32 API functions])
		LIBSUFFIX="-1.dll"
	;;
esac


# Libtool settings
AC_LIBTOOL_WIN32_DLL


# Checks for programs.
AC_PROG_CC
AC_PROG_CC_STDC
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_LN_S
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_PROG_LIBTOOL
AC_PROG_RANLIB


# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_INT64_T
AC_TYPE_UINT64_T
AC_STRUCT_TM


# Checks for header files.
AC_HEADER_STDC
AC_HEADER_TIME
AC_CHECK_HEADERS([dlfcn.h getopt.h inttypes.h libintl.h stdlib.h string.h sys/time.h unistd.h windows.h windef.h])


# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_FUNC_STRTOD
AC_FUNC_STRFTIME
AC_FUNC_SELECT_ARGTYPES
AC_CHECK_FUNCS([getopt memmove memset select setenv snprintf strdup strncasecmp strtol strtoul strtoull])


# Checks for additional libraries
AC_SUBST([MAINLIBS])
ORIGLIBS=${LIBS}; LIBS=""
AC_SEARCH_LIBS([dlopen], [dl], [AC_DEFINE([HAVE_LIBDL], 1, [Have -ldl])])
MAINLIBS=${LIBS}; LIBS=${ORIGLIBS}


# Checks for additional programs
AC_CHECK_PROG([db2man], [db2x_manxml], [yes], [no])
if test "x${db2man}" != xyes; then AC_MSG_RESULT([Warning: docbook man page converter not found - skip generating man pages]); fi
AM_CONDITIONAL([ENABLEMAN], [test "x${db2man}" = xyes])


# Build options
AC_ARG_ENABLE([singlelib], [AS_HELP_STRING([--enable-singlelib], [build single library including backends])], [singlelib="${enableval}"], [singlelib=no])
AM_CONDITIONAL([SINGLELIB], [test "x${singlelib}" = xyes])

AC_ARG_ENABLE([test], [AS_HELP_STRING([--disable-test], [don't build OpenDBX test applications])], [test="${enableval}"], [test=yes])
AM_CONDITIONAL([ENABLETEST], [test "x${test}" = xyes])

AC_ARG_ENABLE([utils], [AS_HELP_STRING([--disable-utils], [don't build OpenDBX utility applications])], [utils="${enableval}"], [utils=yes])
AM_CONDITIONAL([ENABLEUTILS], [test "x${utils}" = xyes])

if test "x${utils}" = xyes; then
	AC_CHECK_HEADERS([readline/readline.h], , [AC_MSG_ERROR([libreadline header (readline.h) not found])])
	AC_CHECK_HEADERS([readline/history.h], , [AC_MSG_ERROR([libreadline header (history.h) not found])])
	AC_CHECK_LIB([readline], [readline], [AC_DEFINE([HAVE_LIBREADLINE], 1, [Have -lreadline])], [AC_MSG_ERROR([readline library (libreadline) not found])])
	AC_CHECK_LIB([ncurses], [initscr], [AC_DEFINE([HAVE_LIBNCURSES], 1, [Have -lncurses])], [AC_MSG_ERROR([ncurses library (libncurses) not found])])
fi


# Conditional builds
AC_SUBST([backends])
AC_SUBST([backendlibs])
AC_ARG_WITH([backends], [AS_HELP_STRING([--with-backends=...], [compile listed backends])], [backends="${withval}"])

# Conditional checks and libs
for backend in $backends
do
	backendlibs="${backendlibs} ../backends/${backend}/lib${backend}backend.la"

	case $backend in
		firebird)
			AC_CHECK_HEADERS([ibase.h], , [AC_MSG_ERROR([firebird header (ibase.h) not found])])
			AC_SUBST([LIBFIREBIRD])
			AC_CHECK_LIB(
				[fbclient], [isc_attach_database],
				[AC_DEFINE([HAVE_LIBFBCLIENT], 1, [Have -lfbclient]) LIBFIREBIRD="fbclient"],
				[AC_MSG_ERROR([firebird library (libfbclient) not found])]
			)
		;;
		mssql)
			AC_CHECK_HEADERS([sybfront.h], , [AC_MSG_ERROR([mssql header (sybfront.h) not found])])
			AC_SUBST([LIBMSSQL])
			AC_CHECK_LIB(
				[sybdb], [dbresults],
				[AC_DEFINE([HAVE_LIBSYBDB], 1, [Have -lsybdb]) LIBMSSQL="sybdb"],
				[AC_MSG_ERROR([mssql library (libsybdb) not found])]
			)
		;;
		mysql)
			AC_CHECK_HEADERS([mysql.h], , [AC_MSG_ERROR([mysql header (mysql.h) not found])])
			AC_SUBST([LIBMYSQL])
			AC_CHECK_LIB(
				[mysqlclient_r], [mysql_init],
				[AC_DEFINE([HAVE_LIBMYSQLCLIENT_R], 1, [Have -lmysqlclient_r]) LIBMYSQL="mysqlclient_r"],
				[AC_CHECK_LIB(
					[mysqlclient], [mysql_init],
					[AC_DEFINE([HAVE_LIBMYSQLCLIENT], 1, [Have -lmysqlclient]) LIBMYSQL="mysqlclient"],
					[AC_MSG_ERROR([mysql library (libmysqlclient) not found])]
				)]
			)
			AC_CHECK_LIB(
				[$LIBMYSQL], [mysql_next_result],
				[AC_DEFINE([HAVE_MYSQL_NEXT_RESULT], 1, [Define to 1 if you have mysql_next_result])]
			)
		;;
		odbc)
			AC_CHECK_HEADERS([sql.h], , [AC_MSG_ERROR([odbc header (sql.h) not found])], [
#ifdef HAVE_WINDEF_H
#include <windef.h>
#endif
			])
			AC_SUBST([LIBODBC])
			AC_CHECK_LIB(
				[odbc], [SQLConnect],
				[AC_DEFINE([HAVE_LIBODBC], 1, [Have -lodbc]) LIBODBC="odbc"],
				AC_CHECK_LIB(
					[iodbc], [SQLConnect],
					[AC_DEFINE([HAVE_LIBODBC], 1, [Have -liodbc]) LIBODBC="iodbc"],
					[AC_DEFINE([HAVE_LIBODBC], 1, [Have -lodbc32]) LIBODBC="odbc32"]
#					AC_CHECK_LIB(
#						[odbc32], [SQLConnect],
#						[AC_DEFINE([HAVE_LIBODBC], 1, [Have -lodbc32]) LIBODBC="odbc32"],
#						[AC_MSG_ERROR([odbc library (libodbc, libiodbc or libodbc32) not found])]
#					)
				)
			)
		;;
		oracle)
			AC_CHECK_HEADERS([oci.h], , [AC_MSG_ERROR([oracle header (oci.h) not found])])
			AC_SUBST([LIBORACLE])
			AC_CHECK_LIB(
				[clntsh], [OCIInitialize],
				AC_DEFINE([HAVE_LIBCLNTSH], 1, [Have -lclntsh]) LIBORACLE="clntsh",
				[AC_MSG_ERROR([oracle library (libclntsh) not found])]
			)
			AC_CHECK_LIB(
				[clntsh], [OCILobWrite2],
				AC_DEFINE([HAVE_OCILOBWRITE2], 1, [Define to 1 if you have OCILobWrite2])
			)
		;;
		pgsql)
			AC_CHECK_HEADERS([libpq-fe.h], , [AC_MSG_ERROR([postgresql header (libpq-fe.h) not found])])
			AC_CHECK_HEADERS([limits.h], , [AC_MSG_ERROR([system limits not found])])
			AC_SUBST([LIBPGSQL])
			AC_CHECK_LIB(
				[pq], [PQconnectdb],
				[AC_DEFINE([HAVE_LIBPQ], 1, [Have -lpq]) LIBPGSQL="pq"],
				[AC_MSG_ERROR([postgresql library (libpq) not found])]
			)
			AC_CHECK_LIB(
				[$LIBPGSQL], [PQescapeStringConn],
				[AC_DEFINE([HAVE_PQ_ESCAPE_STRING_CONN], 1, [Define to 1 if you have PQescapeStringConn])]
			)
		;;
		sqlite)
			AC_CHECK_HEADERS([sqlite.h], , [AC_MSG_ERROR([sqlite header (sqlite.h) not found])])
			AC_SUBST([LIBSQLITE])
			AC_CHECK_LIB(
				[sqlite], [sqlite_get_table],
				[AC_DEFINE([HAVE_LIBSQLITE], 1, [Have -lsqlite]) LIBSQLITE="sqlite"],
				[AC_MSG_ERROR([sqlite library (libsqlite) not found])]
			)
		;;
		sqlite3)
			AC_CHECK_HEADERS([sqlite3.h], , [AC_MSG_ERROR([sqlite3 header (sqlite3.h) not found])])
			AC_SUBST([LIBSQLITE3])
			AC_CHECK_LIB(
				[sqlite3], [sqlite3_prepare],
				[AC_DEFINE([HAVE_LIBSQLITE3], 1, [Have -lsqlite3]) LIBSQLITE3="sqlite3"],
				[AC_MSG_ERROR([sqlite3 library (libsqlite3) not found])]
			)
		;;
		sybase)
			AC_CHECK_HEADERS([ctpublic.h], , [AC_MSG_ERROR([sybase header (ctpublic.h) not found])])
			AC_SUBST([LIBSYBASE])
			AC_CHECK_LIB(
				[ct], [ct_init],
				[AC_DEFINE([HAVE_LIBCT], 1, [Have -lct]) LIBSYBASE="ct"],
				AC_CHECK_LIB(
					[sybct_r], [ct_init],
					[AC_DEFINE([HAVE_LIBSYBCT_R], 1, [Have -lsybct_r]) LIBSYBASE="sybct_r"],
					AC_CHECK_LIB(
						[sybct], [ct_init],
						[AC_DEFINE([HAVE_LIBSYBCT], 1, [Have -lsybct]) LIBSYBASE="sybct"],
						[AC_MSG_ERROR([sybase library (libct, libsybct_r or libsyb) not found])]
					)
				)
			)
		;;
	esac
done


# Generate Makefiles
AC_CONFIG_FILES([
	Makefile
	backends/Makefile
	backends/firebird/Makefile
	backends/mssql/Makefile
	backends/mysql/Makefile
	backends/odbc/Makefile
	backends/oracle/Makefile
	backends/pgsql/Makefile
	backends/sqlite/Makefile
	backends/sqlite3/Makefile
	backends/sybase/Makefile
	doc/Makefile
	lib/Makefile
	po/Makefile.in
	test/Makefile
	utils/Makefile
	utils/doc/Makefile
	utils/po/Makefile.in
	opendbx.spec
	opendbx.pc
])
AC_OUTPUT
