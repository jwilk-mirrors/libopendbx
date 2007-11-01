#!/bin/sh

#
# Automated regression testing
#

if ! test -f odbxtest.site
then
	echo "No odbxtest.site file found"
fi

ODBXAPP="./odbxplustest"

# Import database parameter
. odbxtest.site


# compare results
function odbxcompare ()
{
	diff test.out ref/$1.ref > /dev/null

	if test $? -eq 1
	then
		echo "  $1 FAILED"
		echo "  $1 FAILED" >> testresult.log
		echo "" >> testresult.err
		echo "$1 ERRORS:" >> testresult.err
		cat test.err >> testresult.err
		diff -b test.out ref/$1.ref >> testresult.err
	else
		echo "  $1 OK"
		echo "  $1 OK" >> testresult.log
	fi
}


echo "`date`:" > testresult.log
echo "`date`:" > testresult.err

for backend in $ODBXTEST_BACKENDS
do
	case $backend in
		firebird)
			LD_LIBRARY_PATH="../backends/firebird/.libs" \
			$ODBXAPP \
				-b "firebird" \
				-h "$FIREBIRD_HOST" \
				-p "$FIREBIRD_PORT" \
				-d "$FIREBIRD_DATABASE" \
				-u "$FIREBIRD_USERNAME" \
				-w "$FIREBIRD_PASSWORD" \
				1>test.out 2>test.err
			odbxcompare firebird
		;;
		mssql)
			LD_LIBRARY_PATH="../backends/mssql/.libs" \
			$ODBXAPP \
				-b "mssql" \
				-h "$MSSQL_HOST" \
				-p "$MSSQL_PORT" \
				-d "$MSSQL_DATABASE" \
				-u "$MSSQL_USERNAME" \
				-w "$MSSQL_PASSWORD" \
				1>test.out 2>test.err
			odbxcompare mssql
		;;
		mysql)
			LD_LIBRARY_PATH="../backends/mysql/.libs" \
			$ODBXAPP \
				-b "mysql" \
				-h "$MYSQL_HOST" \
				-p "$MYSQL_PORT" \
				-d "$MYSQL_DATABASE" \
				-u "$MYSQL_USERNAME" \
				-w "$MYSQL_PASSWORD" \
				1>test.out 2>test.err
			odbxcompare mysql
		;;
		oracle)
			LD_LIBRARY_PATH="../backends/oracle/.libs:/usr/lib/oracle/xe/app/oracle/product/10.2.0/client/lib" \
			$ODBXAPP \
				-b "oracle" \
				-h "$ORACLE_HOST" \
				-p "$ORACLE_PORT" \
				-d "$ORACLE_DATABASE" \
				-u "$ORACLE_USERNAME" \
				-w "$ORACLE_PASSWORD" \
				1>test.out 2>test.err
			odbxcompare oracle
		;;
		pgsql)
			LD_LIBRARY_PATH="../backends/pgsql/.libs" \
			$ODBXAPP \
				-b "pgsql" \
				-h "$PGSQL_HOST" \
				-p "$PGSQL_PORT2" \
				-d "$PGSQL_DATABASE" \
				-u "$PGSQL_USERNAME" \
				-w "$PGSQL_PASSWORD" \
				1>test.out 2>test.err
			odbxcompare pgsql
		;;
		sqlite)
			LD_LIBRARY_PATH="../backends/sqlite/.libs" \
			$ODBXAPP \
				-b "sqlite" \
				-h "$SQLITE_HOST" \
				-p "$SQLITE_PORT" \
				-d "$SQLITE_DATABASE" \
				-u "$SQLITE_USERNAME" \
				-w "$SQLITE_PASSWORD" \
				1>test.out 2>test.err
			odbxcompare sqlite
		;;
		sqlite3)
			LD_LIBRARY_PATH="../backends/sqlite3/.libs" \
			$ODBXAPP \
				-b "sqlite3" \
				-h "$SQLITE3_HOST" \
				-p "$SQLITE3_PORT" \
				-d "$SQLITE3_DATABASE" \
				-u "$SQLITE3_USERNAME" \
				-w "$SQLITE3_PASSWORD" \
				1>test.out 2>test.err
			odbxcompare sqlite3
		;;
		sybase)
			LD_LIBRARY_PATH="../backends/sybase/.libs" \
			$ODBXAPP \
				-b "sybase" \
				-h "$SYBASE_HOST" \
				-p "$SYBASE_PORT" \
				-d "$SYBASE_DATABASE" \
				-u "$SYBASE_USERNAME" \
				-w "$SYBASE_PASSWORD" \
				1>test.out 2>test.err
			odbxcompare sybase
		;;
	esac
done


# cleanup
rm -f test.out
rm -f test.err


exit 0
