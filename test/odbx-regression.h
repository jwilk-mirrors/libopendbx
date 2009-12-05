/*
 *  OpenDBX - A simple but extensible database abstraction layer
 *  Copyright (C) 2004-2007 Norbert Sendetzky and others
 *
 *  Distributed under the terms of the GNU Library General Public Licence
 *  version 2 or (at your option) any later version.
 */



#include <stddef.h>



struct odbxtest
{
	const char* name;
	int value;
	int result;
	int tryit;
};


static struct odbxtest cap[] = {
	{ "ODBX_CAP_BASIC", ODBX_CAP_BASIC, 0, 1 },
	{ "ODBX_CAP_LO", ODBX_CAP_LO, 0, 1 },
};

#define CAPMAX 2


static struct odbxtest opt[] = {
	{ "ODBX_OPT_API_VERSION", ODBX_OPT_API_VERSION, 0, 0 },
	{ "ODBX_OPT_THREAD_SAFE", ODBX_OPT_THREAD_SAFE, 0, 0 },
	{ "ODBX_OPT_TLS", ODBX_OPT_TLS, 0, ODBX_TLS_TRY },
	{ "ODBX_OPT_CONNECT_TIMEOUT", ODBX_OPT_CONNECT_TIMEOUT, 0, 5 },
	{ "ODBX_OPT_MULTI_STATEMENTS", ODBX_OPT_MULTI_STATEMENTS, 0, ODBX_ENABLE },
	{ "ODBX_OPT_PAGED_RESULTS", ODBX_OPT_PAGED_RESULTS, 0, ODBX_ENABLE },
	{ "ODBX_OPT_COMPRESS", ODBX_OPT_COMPRESS, 0, ODBX_ENABLE },
};

#define OPTMAX 7



struct odbxstmt
{
	int num;
	const char* str;
};

#define CONNMAX 2


static struct odbxstmt firebird_basic[] = {
	{ 0 % CONNMAX, "SELECT * FROM \"odbxtest\" col = '%s'" },
	{ 0 % CONNMAX, "CREATE TABLE \"odbxtest\" ("
	"   \"i1\" CHARACTER(1),"
	"   \"i16\" SMALLINT,"
	"   \"i32\" INTEGER,"
	"   \"i64\" BIGINT,"
	"   \"d9\" DECIMAL(9,3),"
	"   \"f4\" REAL,"
	"   \"f3\" FLOAT(15),"
	"   \"f8\" DOUBLE PRECISION,"
	"   \"c2\" CHARACTER(2),"
	"   \"nc12\" NATIONAL CHARACTER(12),"
	"   \"str20\" CHARACTER VARYING(20),"
	"   \"nstr24\" NATIONAL CHARACTER VARYING(24),"
	"   \"clob4g\" BLOB SUB_TYPE TEXT,"
	"   \"timeval\" TIME,"
	"   \"timestmp\" TIMESTAMP,"
	"   \"dateval\" DATE"
	")" },

	{ 1 % CONNMAX, "INSERT INTO \"odbxtest\" ( \"i1\", \"i16\", \"i32\", \"i64\", \"d9\", \"f4\", \"f3\", \"f8\", \"c2\", \"nc12\", \"str20\", \"nstr24\", \"clob4g\", \"timeval\", \"timestmp\", \"dateval\" ) VALUES ( '0', 1000, 100000, 10000000, 12345.123, 10000.1, 100.1, 10000000.1, 'DE', 'äöüäöü', 'first string', 'äöüäöüäöüäöü', 'test clob de', '15:00:00', '2000-01-01 00:00:01', '2000-02-29' )" },

	{ 0 % CONNMAX, "INSERT INTO \"odbxtest\" ( \"i1\", \"i16\", \"i32\", \"i64\", \"d9\", \"f4\", \"f3\", \"f8\", \"c2\", \"nc12\", \"str20\", \"nstr24\", \"clob4g\", \"timeval\", \"timestmp\", \"dateval\" ) VALUES ( '1', 2, 4, 8, 1.333, 4.5, 3.99, 8.0, 'EN', 'aouaou', 'varstring', 'aouaouaouaou', 'test clob english', '23:59:59', '1999-01-01 00:00:00', '1999-01-01' )" },

	{ 1 % CONNMAX, "INSERT INTO \"odbxtest\" ( \"i1\", \"i16\", \"i32\", \"i64\", \"d9\", \"f4\", \"f3\", \"f8\", \"c2\", \"nc12\", \"str20\", \"nstr24\", \"clob4g\", \"timeval\", \"timestmp\", \"dateval\" ) VALUES ( NULL, 2, 4, 8, 2.5, 4.0, 3.1, 8.5, 'FR', 'éçèéçè', NULL, 'éçèéçèéçèéçè', 'test clob french', '15:00:00', '2005-12-31 23:59:59', '2005-12-01' )" },

	{ 0 % CONNMAX, "SELECT * FROM \"odbxtest\" WHERE \"i16\" = 2" },
	{ 1 % CONNMAX, "SELECT * FROM \"odbxtest\" WHERE \"str20\" = 'empty'" },
	{ 0 % CONNMAX, "SET TRANSACTION" },
	{ 0 % CONNMAX, "UPDATE \"odbxtest\" SET \"i16\" = 3 WHERE \"str20\" LIKE '%string'" },
	{ 0 % CONNMAX, "DELETE FROM \"odbxtest\" WHERE \"str20\" LIKE '%string'" },
	{ 0 % CONNMAX, "ROLLBACK" },
	{ 1 % CONNMAX, "UPDATE \"odbxtest\" SET \"c2\" = '' WHERE \"c2\" = 'AA'" },
	{ 0 % CONNMAX, "SELECT * FROM \"odbxtest\"" },
	{ 1 % CONNMAX, "DELETE FROM \"odbxtest\" WHERE \"str20\" LIKE '%string'" },
	{ 0 % CONNMAX, "SELECT COUNT(*) AS \"count\" FROM \"odbxtest\"" },
	{ 1 % CONNMAX, "DROP TABLE \"odbxtest\"" },
	{ -1, NULL }
};

static struct odbxstmt* firebird_stmt[] = { firebird_basic, firebird_basic };



static struct odbxstmt mssql_multi[] = {
	{ 0 % CONNMAX, "SELECT * FROM \"odbxtest\"" },
	{ 0 % CONNMAX, "CREATE TABLE \"odbxtest\" ("
	"   \"i1\" CHARACTER(1) NULL,"
	"   \"i16\" SMALLINT,"
	"   \"i32\" INTEGER,"
	"   \"i64\" BIGINT,"
	"   \"d9\" DECIMAL(9,3),"
	"   \"f4\" REAL,"
	"   \"f3\" FLOAT(15),"
	"   \"f8\" DOUBLE PRECISION,"
	"   \"c2\" CHARACTER(2),"
	"   \"nc12\" NATIONAL CHARACTER(12),"
	"   \"str20\" CHARACTER VARYING(20) NULL,"
	"   \"nstr24\" NATIONAL CHARACTER VARYING(24),"
	"   \"clob64k\" TEXT,"
	"   \"timeval\" DATETIME,"
	"   \"timestmp\" DATETIME,"
	"   \"dateval\" DATETIME"
	")" },

	{ 1 % CONNMAX, "INSERT INTO \"odbxtest\" ( \"i1\", \"i16\", \"i32\", \"i64\", \"d9\", \"f4\", \"f3\", \"f8\", \"c2\", \"nc12\", \"str20\", \"nstr24\", \"clob64k\", \"timeval\", \"timestmp\", \"dateval\" ) VALUES ( '0', 1000, 100000, 10000000, 12345.123, 10000.1, 100.1, 10000000.1, 'DE', 'äöüäöü', 'first string', 'äöüäöüäöüäöü', 'a very long text', '15:00:00', '2000-01-01 00:00:01', '2000-02-29' )" },

	{ 0 % CONNMAX, "INSERT INTO \"odbxtest\" ( \"i1\", \"i16\", \"i32\", \"i64\", \"d9\", \"f4\", \"f3\", \"f8\", \"c2\", \"nc12\", \"str20\", \"nstr24\", \"clob64k\", \"timeval\", \"timestmp\", \"dateval\" ) VALUES ( '1', 2, 4, 8, 1.333, 4.5, 3.99, 8.0, 'EN', 'aouaou', 'varstring', 'aouaouaouaou', 'clob', '23:59:59', '1999-01-01 00:00:00', '1999-01-01' )" },

	{ 1 % CONNMAX, "INSERT INTO \"odbxtest\" ( \"i1\", \"i16\", \"i32\", \"i64\", \"d9\", \"f4\", \"f3\", \"f8\", \"c2\", \"nc12\", \"str20\", \"nstr24\", \"clob64k\", \"timeval\", \"timestmp\", \"dateval\" ) VALUES ( NULL, 2, 4, 8, 2.5, 4.0, 3.1, 8.5, 'FR', 'éçèéçè', NULL, 'éçèéçèéçèéçè', 'text', '15:00:00', '2005-12-31 23:59:59', '2005-12-01' )" },

	{ 0 % CONNMAX, "SELECT * FROM \"odbxtest\" WHERE \"i16\" = 2   SELECT * FROM \"odbxtest\" WHERE \"str20\" = 'empty'" },
	{ 1 % CONNMAX, "BEGIN TRANSACTION   UPDATE \"odbxtest\" SET \"i16\" = 3 WHERE \"str20\" LIKE '%string'   DELETE FROM \"odbxtest\" WHERE \"str20\" LIKE '%string'   ROLLBACK" },
	{ 0 % CONNMAX, "UPDATE \"odbxtest\" SET \"c2\" = '' WHERE \"c2\" = 'AA'" },
	{ 1 % CONNMAX, "SELECT * FROM \"odbxtest\"" },
	{ 0 % CONNMAX, "DELETE FROM \"odbxtest\" WHERE \"str20\" LIKE '%string'" },
	{ 1 % CONNMAX, "SELECT COUNT(*) AS \"count\" FROM \"odbxtest\"" },
	{ 0 % CONNMAX, "DROP TABLE \"odbxtest\"" },
	{ -1, NULL }
};

static struct odbxstmt* mssql_stmt[] = { mssql_multi, mssql_multi };



static struct odbxstmt mysql_basic[] = {
	{ 0 % CONNMAX, "SELECT * FROM \"odbxtest\"" },
	{ 0 % CONNMAX, "CREATE TABLE \"odbxtest\" ("
	"   \"i1\" BOOLEAN,"
	"   \"i16\" SMALLINT,"
	"   \"i32\" INTEGER,"
	"   \"i64\" BIGINT,"
	"   \"d9\" DECIMAL(9,3),"
	"   \"f4\" REAL,"
	"   \"f3\" FLOAT(15),"
	"   \"f8\" DOUBLE PRECISION,"
	"   \"c2\" CHARACTER(2),"
	"   \"nc12\" NATIONAL CHARACTER(12),"
	"   \"str20\" CHARACTER VARYING(20),"
	"   \"nstr24\" NATIONAL CHARACTER VARYING(24),"
	"   \"clob64k\" TEXT,"
	"   \"timeval\" TIME,"
	"   \"timestmp\" DATETIME,"
	"   \"dateval\" DATE"
	") ENGINE=InnoDB" },

	{ 1 % CONNMAX, "INSERT INTO \"odbxtest\" ( \"i1\", \"i16\", \"i32\", \"i64\", \"d9\", \"f4\", \"f3\", \"f8\", \"c2\", \"nc12\", \"str20\", \"nstr24\", \"clob64k\", \"timeval\", \"timestmp\", \"dateval\" ) VALUES ( 0, 1000, 100000, 10000000, 12345.123, 10000.1, 100.1, 10000000.1, 'DE', 'äöüäöü', 'first string', 'äöüäöüäöüäöü', 'a very long text', '15:00:00', '2000-01-01 00:00:01', '2000-02-29' )" },

	{ 0 % CONNMAX, "INSERT INTO \"odbxtest\" ( \"i1\", \"i16\", \"i32\", \"i64\", \"d9\", \"f4\", \"f3\", \"f8\", \"c2\", \"nc12\", \"str20\", \"nstr24\", \"clob64k\", \"timeval\", \"timestmp\", \"dateval\" ) VALUES ( 1, 2, 4, 8, 1.333, 4.5, 3.99, 8.0, 'EN', 'aouaou', 'varstring', 'aouaouaouaou', 'clob', '23:59:59', '1999-01-01 00:00:00', '1999-01-01' )" },

	{ 1 % CONNMAX, "INSERT INTO \"odbxtest\" ( \"i1\", \"i16\", \"i32\", \"i64\", \"d9\", \"f4\", \"f3\", \"f8\", \"c2\", \"nc12\", \"str20\", \"nstr24\", \"clob64k\", \"timeval\", \"timestmp\", \"dateval\" ) VALUES ( NULL, 2, 4, 8, 2.5, 4.0, 3.1, 8.5, 'FR', 'éçèéçè', NULL, 'éçèéçèéçèéçè', 'text', '15:00:00', '2005-12-31 23:59:59', '2005-12-01' )" },

	{ 0 % CONNMAX, "SELECT * FROM \"odbxtest\" WHERE \"i16\" = 2" },
	{ 1 % CONNMAX, "SELECT * FROM \"odbxtest\" WHERE \"str20\" = 'empty'" },
	{ 0 % CONNMAX, "START TRANSACTION" },
	{ 0 % CONNMAX, "UPDATE \"odbxtest\" SET \"i16\" = 3 WHERE \"str20\" LIKE '%string'" },
	{ 0 % CONNMAX, "DELETE FROM \"odbxtest\" WHERE \"str20\" LIKE '%string'" },
	{ 0 % CONNMAX, "ROLLBACK" },
	{ 1 % CONNMAX, "UPDATE \"odbxtest\" SET \"c2\" = '' WHERE \"c2\" = 'AA'" },
	{ 0 % CONNMAX, "SELECT * FROM \"odbxtest\"" },
	{ 1 % CONNMAX, "DELETE FROM \"odbxtest\" WHERE \"str20\" LIKE '%string'" },
	{ 0 % CONNMAX, "SELECT COUNT(*) AS \"count\" FROM \"odbxtest\"" },
	{ 1 % CONNMAX, "DROP TABLE \"odbxtest\"" },
	{ -1, NULL }
};

static struct odbxstmt mysql_multi[] = {
	{ 0 % CONNMAX, "SELECT * FROM \"odbxtest\"" },
	{ 0 % CONNMAX, "CREATE TABLE \"odbxtest\" ("
	"	\"i1\" BOOLEAN,"
	"	\"i16\" SMALLINT,"
	"	\"i32\" INTEGER,"
	"	\"i64\" BIGINT,"
	"	\"d9\" DECIMAL(9,3),"
	"	\"f4\" REAL,"
	"	\"f3\" FLOAT(15),"
	"	\"f8\" DOUBLE PRECISION,"
	"	\"c2\" CHARACTER(2),"
	"	\"nc12\" NATIONAL CHARACTER(12),"
	"	\"str20\" CHARACTER VARYING(20),"
	"	\"nstr24\" NATIONAL CHARACTER VARYING(24),"
	"	\"clob64k\" TEXT,"
	"	\"timeval\" TIME,"
	"	\"timestmp\" DATETIME,"
	"	\"dateval\" DATE"
	") ENGINE=InnoDB" },

	{ 1 % CONNMAX, "INSERT INTO \"odbxtest\" ( \"i1\", \"i16\", \"i32\", \"i64\", \"d9\", \"f4\", \"f3\", \"f8\", \"c2\", \"nc12\", \"str20\", \"nstr24\", \"clob64k\", \"timeval\", \"timestmp\", \"dateval\" ) VALUES ( 0, 1000, 100000, 10000000, 12345.123, 10000.1, 100.1, 10000000.1, 'DE', 'äöüäöü', 'first string', 'äöüäöüäöüäöü', 'a very long text', '15:00:00', '2000-01-01 00:00:01', '2000-02-29' )" },

	{ 0 % CONNMAX, "INSERT INTO \"odbxtest\" ( \"i1\", \"i16\", \"i32\", \"i64\", \"d9\", \"f4\", \"f3\", \"f8\", \"c2\", \"nc12\", \"str20\", \"nstr24\", \"clob64k\", \"timeval\", \"timestmp\", \"dateval\" ) VALUES ( 1, 2, 4, 8, 1.333, 4.5, 3.99, 8.0, 'EN', 'aouaou', 'varstring', 'aouaouaouaou', 'clob', '23:59:59', '1999-01-01 00:00:00', '1999-01-01' )" },

	{ 1 % CONNMAX, "INSERT INTO \"odbxtest\" ( \"i1\", \"i16\", \"i32\", \"i64\", \"d9\", \"f4\", \"f3\", \"f8\", \"c2\", \"nc12\", \"str20\", \"nstr24\", \"clob64k\", \"timeval\", \"timestmp\", \"dateval\" ) VALUES ( NULL, 2, 4, 8, 2.5, 4.0, 3.1, 8.5, 'FR', 'éçèéçè', NULL, 'éçèéçèéçèéçè', 'text', '15:00:00', '2005-12-31 23:59:59', '2005-12-01' )" },

	{ 0 % CONNMAX, "SELECT * FROM \"odbxtest\" WHERE \"i16\" = 2; SELECT * FROM \"odbxtest\" WHERE \"str20\" = 'empty'" },
	{ 1 % CONNMAX, "START TRANSACTION; UPDATE \"odbxtest\" SET \"i16\" = 3 WHERE \"str20\" LIKE '%string'; DELETE FROM \"odbxtest\" WHERE \"str20\" LIKE '%string'; ROLLBACK" },
	{ 0 % CONNMAX, "UPDATE \"odbxtest\" SET \"c2\" = '' WHERE \"c2\" = 'AA'" },
	{ 1 % CONNMAX, "SELECT * FROM \"odbxtest\"" },
	{ 0 % CONNMAX, "DELETE FROM \"odbxtest\" WHERE \"str20\" LIKE '%string'" },
	{ 1 % CONNMAX, "SELECT COUNT(*) AS \"count\" FROM \"odbxtest\"" },
	{ 0 % CONNMAX, "DROP TABLE \"odbxtest\"" },
	{ -1, NULL }
};

static struct odbxstmt* mysql_stmt[] = { mysql_basic, mysql_multi };



static struct odbxstmt odbc_basic[] = {
	{ 0 % CONNMAX, "SELECT * FROM odbxtest" },
	{ 0 % CONNMAX, "CREATE TABLE odbxtest ("
	"   i1 CHARACTER(1) NULL,"
	"   i16 SMALLINT,"
	"   i32 INTEGER,"
	"   d9 DECIMAL(9,3),"
	"   f4 REAL,"
	"   f3 FLOAT(15),"
	"   f8 DOUBLE PRECISION,"
	"   c2 CHARACTER(2),"
	"   nc12 NATIONAL CHARACTER(12),"
	"   str20 CHARACTER VARYING(20),"
	"   nstr24 NATIONAL CHARACTER VARYING(24)"
	")" },

	{ 0 % CONNMAX, "INSERT INTO odbxtest ( i1, i16, i32, d9, f4, f3, f8, c2, nc12, str20, nstr24 ) VALUES ( '0', 1000, 100000, 12345.123, 10000.1, 100.1, 10000000.1, 'DE', 'äöüäöü', 'first string', 'äöüäöüäöüäöü' )" },

	{ 0 % CONNMAX, "INSERT INTO odbxtest ( i1, i16, i32, d9, f4, f3, f8, c2, nc12, str20, nstr24 ) VALUES ( '1', 2, 4, 1.333, 4.5, 3.99, 8.0, 'EN', 'aouaou', 'varstring', 'aouaouaouaou' )" },

	{ 0 % CONNMAX, "INSERT INTO odbxtest ( i1, i16, i32, d9, f4, f3, f8, c2, nc12, str20, nstr24 ) VALUES ( NULL, 2, 4, 2.5, 4.0, 3.1, 8.5, 'FR', 'éçèéçè', NULL, 'éçèéçèéçèéçè' )" },

	{ 0 % CONNMAX, "SELECT * FROM odbxtest WHERE i16 = 2" },
	{ 0 % CONNMAX, "SELECT * FROM odbxtest WHERE str20 = 'empty'" },
	{ 0 % CONNMAX, "BEGIN TRANSACTION" },
	{ 0 % CONNMAX, "UPDATE odbxtest SET i16 = 3 WHERE str20 LIKE '%string'" },
	{ 0 % CONNMAX, "DELETE FROM odbxtest WHERE str20 LIKE '%string'" },
	{ 0 % CONNMAX, "ROLLBACK" },
	{ 0 % CONNMAX, "UPDATE odbxtest SET c2 = '' WHERE c2 = 'AA'" },
	{ 0 % CONNMAX, "SELECT * FROM odbxtest" },
	{ 0 % CONNMAX, "DELETE FROM odbxtest WHERE str20 LIKE '%string'" },
	{ 0 % CONNMAX, "SELECT COUNT(*) AS count FROM odbxtest" },
	{ 0 % CONNMAX, "DROP TABLE odbxtest" },
	{ -1, NULL }
};

static struct odbxstmt* odbc_stmt[] = { odbc_basic, odbc_basic };



static struct odbxstmt oracle_basic[] = {
	{ 0 % CONNMAX, "SELECT * FROM \"odbxtest\"" },
	{ 0 % CONNMAX, "CREATE TABLE \"odbxtest\" ("
	"   \"i1\" CHAR(1),"
	"   \"i16\" SMALLINT,"
	"   \"i32\" INTEGER,"
	"   \"i64\" NUMBER(20,0),"
	"   \"d9\" DECIMAL(9,3),"
	"   \"f4\" REAL,"
	"   \"f3\" FLOAT(15),"
	"   \"f8\" DOUBLE PRECISION,"
	"   \"c2\" CHARACTER(2),"
	"   \"nc12\" CHARACTER(12),"
	"   \"str20\" CHARACTER VARYING(20),"
	"   \"nstr24\" CHARACTER VARYING(24),"
	"   \"clob4g\" CLOB,"
	"   \"timestmp\" TIMESTAMP,"
	"   \"dateval\" DATE"
	")" },

	{ 1 % CONNMAX, "INSERT INTO \"odbxtest\" ( \"i1\", \"i16\", \"i32\", \"i64\", \"d9\", \"f4\", \"f3\", \"f8\", \"c2\", \"nc12\", \"str20\", \"nstr24\", \"clob4g\", \"timestmp\", \"dateval\"  ) VALUES ( '0', 1000, 100000, 10000000, 12345.123, 10000.1, 100.1, 10000000.1, 'DE', 'äöüäöü', 'first string', 'äöüäöüäöüäöü', 'a very long text', '2000-01-01 00:00:01', '2000-02-29' )" },

	{ 0 % CONNMAX, "INSERT INTO \"odbxtest\" ( \"i1\", \"i16\", \"i32\", \"i64\", \"d9\", \"f4\", \"f3\", \"f8\", \"c2\", \"nc12\", \"str20\", \"nstr24\", \"clob4g\", \"timestmp\", \"dateval\"  ) VALUES ( '1', 2, 4, 8, 1.333, 4.5, 3.99, 8.0, 'EN', 'aouaou', 'varstring', 'aouaouaouaou', 'clob', '1999-01-01 00:00:00', '1999-01-01' )" },

	{ 1 % CONNMAX, "INSERT INTO \"odbxtest\" ( \"i1\", \"i16\", \"i32\", \"i64\", \"d9\", \"f4\", \"f3\", \"f8\", \"c2\", \"nc12\", \"str20\", \"nstr24\", \"clob4g\", \"timestmp\", \"dateval\"  ) VALUES ( NULL, 2, 4, 8, 2.5, 4.0, 3.1, 8.5, 'FR', 'éçèéçè', NULL, 'éçèéçèéçèéçè', 'text', '2005-12-31 23:59:59', '2005-12-01' )" },

	{ 0 % CONNMAX, "SELECT * FROM \"odbxtest\" WHERE \"i16\" = 2 ORDER BY \"i1\"" },
	{ 1 % CONNMAX, "SELECT * FROM \"odbxtest\" WHERE \"str20\" = 'empty' ORDER BY \"i1\"" },
	{ 0 % CONNMAX, "SET TRANSACTION ISOLATION LEVEL READ COMMITTED" },
	{ 0 % CONNMAX, "UPDATE \"odbxtest\" SET \"i16\" = 3 WHERE \"str20\" LIKE '%string'" },
	{ 0 % CONNMAX, "DELETE FROM \"odbxtest\" WHERE \"str20\" LIKE '%string'" },
	{ 0 % CONNMAX, "ROLLBACK" },
	{ 1 % CONNMAX, "UPDATE \"odbxtest\" SET \"c2\" = '' WHERE \"c2\" = 'AA'" },
	{ 0 % CONNMAX, "SELECT * FROM \"odbxtest\" ORDER BY \"i1\"" },
	{ 1 % CONNMAX, "DELETE FROM \"odbxtest\" WHERE \"str20\" LIKE '%string'" },
	{ 0 % CONNMAX, "SELECT COUNT(*) AS \"count\" FROM \"odbxtest\"" },
	{ 1 % CONNMAX, "DROP TABLE \"odbxtest\"" },
	{ -1, NULL }
};

static struct odbxstmt* oracle_stmt[] = { oracle_basic, NULL };



static struct odbxstmt pgsql_multi[] = {
	{ 0 % CONNMAX, "SELECT * FROM \"odbxtest\"" },
	{ 0 % CONNMAX, "CREATE TABLE \"odbxtest\" ("
	"   \"i1\" BOOLEAN,"
	"   \"i16\" SMALLINT,"
	"   \"i32\" INTEGER,"
	"   \"i64\" BIGINT,"
	"   \"d9\" DECIMAL(9,3),"
	"   \"f4\" REAL,"
	"   \"f3\" FLOAT(15),"
	"   \"f8\" DOUBLE PRECISION,"
	"   \"c2\" CHARACTER(2),"
	"   \"nc12\" NATIONAL CHARACTER(12),"
	"   \"str20\" CHARACTER VARYING(20),"
	"   \"nstr24\" NATIONAL CHARACTER VARYING(24),"
	"   \"clob1g\" TEXT,"
	"   \"timeval\" TIME,"
	"   \"timestmp\" TIMESTAMP,"
	"   \"dateval\" DATE"
	")" },

	{ 1 % CONNMAX, "INSERT INTO \"odbxtest\" ( \"i1\", \"i16\", \"i32\", \"i64\", \"d9\", \"f4\", \"f3\", \"f8\", \"c2\", \"nc12\", \"str20\", \"nstr24\", \"clob1g\", \"timeval\", \"timestmp\", \"dateval\" ) VALUES ( '0', 1000, 100000, 10000000, 12345.123, 10000.1, 100.1, 10000000.1, 'DE', 'äöüäöü', 'first string', 'äöüäöüäöüäöü', 'a very long text', '15:00:00', '2000-01-01 00:00:01', '2000-02-29' )" },

	{ 0 % CONNMAX, "INSERT INTO \"odbxtest\" ( \"i1\", \"i16\", \"i32\", \"i64\", \"d9\", \"f4\", \"f3\", \"f8\", \"c2\", \"nc12\", \"str20\", \"nstr24\", \"clob1g\", \"timeval\", \"timestmp\", \"dateval\" ) VALUES ( '1', 2, 4, 8, 1.333, 4.5, 3.99, 8.0, 'EN', 'aouaou', 'varstring', 'aouaouaouaou', 'clob', '23:59:59', '1999-01-01 00:00:00', '1999-01-01' )" },

	{ 1 % CONNMAX, "INSERT INTO \"odbxtest\" ( \"i1\", \"i16\", \"i32\", \"i64\", \"d9\", \"f4\", \"f3\", \"f8\", \"c2\", \"nc12\", \"str20\", \"nstr24\", \"clob1g\", \"timeval\", \"timestmp\", \"dateval\" ) VALUES ( NULL, 2, 4, 8, 2.5, 4.0, 3.1, 8.5, 'FR', 'éçèéçè', NULL, 'éçèéçèéçèéçè', 'text', '15:00:00', '2005-12-31 23:59:59', '2005-12-01' )" },

	{ 0 % CONNMAX, "SELECT * FROM \"odbxtest\" WHERE \"i16\" = 2; SELECT * FROM \"odbxtest\" WHERE \"str20\" = 'empty'" },
	{ 1 % CONNMAX, "START TRANSACTION; UPDATE \"odbxtest\" SET \"i16\" = 3 WHERE \"str20\" LIKE '%string'; DELETE FROM \"odbxtest\" WHERE \"str20\" LIKE '%string'; ROLLBACK" },
	{ 0 % CONNMAX, "UPDATE \"odbxtest\" SET \"c2\" = '' WHERE \"c2\" = 'AA'" },
	{ 1 % CONNMAX, "SELECT * FROM \"odbxtest\"" },
	{ 0 % CONNMAX, "DELETE FROM \"odbxtest\" WHERE \"str20\" LIKE '%string'" },
	{ 1 % CONNMAX, "SELECT COUNT(*) AS \"count\" FROM \"odbxtest\"" },
	{ 0 % CONNMAX, "DROP TABLE \"odbxtest\"" },
	{ -1, NULL }
};

static struct odbxstmt* pgsql_stmt[] = { pgsql_multi, pgsql_multi };



static struct odbxstmt sqlite_multi[] = {
	{ 0 % CONNMAX, "SELECT * FROM \"odbxtest\"" },
	{ 0 % CONNMAX, "CREATE TABLE \"odbxtest\" ("
	"   \"i1\" BOOLEAN,"
	"   \"i16\" SMALLINT,"
	"   \"i32\" INTEGER,"
	"   \"i64\" BIGINT,"
	"   \"d9\" DECIMAL(9,3),"
	"   \"f4\" REAL,"
	"   \"f3\" FLOAT(15),"
	"   \"f8\" DOUBLE PRECISION,"
	"   \"c2\" CHARACTER(2),"
	"   \"nc12\" NATIONAL CHARACTER(12),"
	"   \"str20\" CHARACTER VARYING(20),"
	"   \"nstr24\" NATIONAL CHARACTER VARYING(24),"
	"   \"clob64k\" CLOB,"
	"   \"nclob64k\" CLOB,"
	"   \"timeval\" TIME,"
	"   \"timevaltz\" TIME WITH TIME ZONE,"
	"   \"timestmp\" TIMESTAMP,"
	"   \"timestmptz\" TIMESTAMP WITH TIME ZONE,"
	"   \"dateval\" DATE"
	")" },

	{ 0 % CONNMAX, "INSERT INTO \"odbxtest\" ( \"i1\", \"i16\", \"i32\", \"i64\", \"d9\", \"f4\", \"f3\", \"f8\", \"c2\", \"nc12\", \"str20\", \"nstr24\", \"clob64k\", \"nclob64k\", \"timeval\", \"timevaltz\", \"timestmp\", \"timestmptz\", \"dateval\" ) VALUES ( 0, 1000, 100000, 10000000, 12345.123, 10000.1, 100.1, 10000000.1, 'DE', 'äöüäöü', 'first string', 'äöüäöüäöüäöü', 'a very long text', 'äöü very long national text äöü', '15:00:00', '15:00:00-01', '2000-01-01 00:00:01', '2000-01-01 00:00:01-01', '2000-02-29' )" },

	{ 0 % CONNMAX, "INSERT INTO \"odbxtest\" ( \"i1\", \"i16\", \"i32\", \"i64\", \"d9\", \"f4\", \"f3\", \"f8\", \"c2\", \"nc12\", \"str20\", \"nstr24\", \"clob64k\", \"nclob64k\", \"timeval\", \"timevaltz\", \"timestmp\", \"timestmptz\", \"dateval\" ) VALUES ( 1, 2, 4, 8, 1.333, 4.5, 3.99, 8.0, 'EN', 'aouaou', 'varstring', 'aouaouaouaou', 'clob', 'national character clob', '23:59:59', '23:59:59+06', '1999-01-01 00:00:00', '1999-01-01 00:00:00+06', '1999-01-01' )" },

	{ 0 % CONNMAX, "INSERT INTO \"odbxtest\" ( \"i1\", \"i16\", \"i32\", \"i64\", \"d9\", \"f4\", \"f3\", \"f8\", \"c2\", \"nc12\", \"str20\", \"nstr24\", \"clob64k\", \"nclob64k\", \"timeval\", \"timevaltz\", \"timestmp\", \"timestmptz\", \"dateval\" ) VALUES ( NULL, 2, 4, 8, 2.5, 4.0, 3.1, 8.5, 'FR', 'éçèéçè', NULL, 'éçèéçèéçèéçè', 'text', 'éçè very long national text éçè', '15:00:00', '15:00:00+00', '2005-12-31 23:59:59', '2005-12-31 23:59:59+00', '2005-12-01' )" },

	{ 0 % CONNMAX, "SELECT * FROM \"odbxtest\" WHERE \"i16\" = 2; SELECT * FROM \"odbxtest\" WHERE \"str20\" = 'empty'" },
	{ 0 % CONNMAX, "BEGIN TRANSACTION; UPDATE \"odbxtest\" SET \"i16\" = 3 WHERE \"str20\" LIKE '%string'; DELETE FROM \"odbxtest\" WHERE \"str20\" LIKE '%string'; ROLLBACK" },
	{ 0 % CONNMAX, "UPDATE \"odbxtest\" SET \"c2\" = '' WHERE \"c2\" = 'AA'" },
	{ 0 % CONNMAX, "SELECT * FROM \"odbxtest\"" },
	{ 0 % CONNMAX, "DELETE FROM \"odbxtest\" WHERE \"str20\" LIKE '%string'" },
	{ 0 % CONNMAX, "SELECT COUNT(*) AS \"count\" FROM \"odbxtest\"" },
	{ 0 % CONNMAX, "DROP TABLE \"odbxtest\"" },
	{ -1, NULL }
};

static struct odbxstmt* sqlite_stmt[] = { sqlite_multi, sqlite_multi };



static struct odbxstmt sqlite3_multi[] = {
	{ 0 % CONNMAX, "SELECT * FROM \"odbxtest\"" },
	{ 0 % CONNMAX, "CREATE TABLE \"odbxtest\" ("
	"   \"i1\" BOOLEAN,"
	"   \"i16\" SMALLINT,"
	"   \"i32\" INTEGER,"
	"   \"i64\" BIGINT,"
	"   \"d9\" DECIMAL(9,3),"
	"   \"f4\" REAL,"
	"   \"f3\" FLOAT(15),"
	"   \"f8\" DOUBLE PRECISION,"
	"   \"c2\" CHARACTER(2),"
	"   \"nc12\" NATIONAL CHARACTER(12),"
	"   \"str20\" CHARACTER VARYING(20),"
	"   \"nstr24\" NATIONAL CHARACTER VARYING(24),"
	"   \"clob64k\" CLOB,"
	"   \"nclob64k\" CLOB,"
	"   \"timeval\" TIME,"
	"   \"timevaltz\" TIME WITH TIME ZONE,"
	"   \"timestmp\" TIMESTAMP,"
	"   \"timestmptz\" TIMESTAMP WITH TIME ZONE,"
	"   \"dateval\" DATE"
	")" },

	{ 1 % CONNMAX, "INSERT INTO \"odbxtest\" ( \"i1\", \"i16\", \"i32\", \"i64\", \"d9\", \"f4\", \"f3\", \"f8\", \"c2\", \"nc12\", \"str20\", \"nstr24\", \"clob64k\", \"nclob64k\", \"timeval\", \"timevaltz\", \"timestmp\", \"timestmptz\", \"dateval\" ) VALUES ( 0, 1000, 100000, 10000000, 12345.123, 10000.1, 100.1, 10000000.1, 'DE', 'äöüäöü', 'first string', 'äöüäöüäöüäöü', 'a very long text', 'äöü very long national text äöü', '15:00:00', '15:00:00-01', '2000-01-01 00:00:01', '2000-01-01 00:00:01-01', '2000-02-29' )" },

	{ 0 % CONNMAX, "INSERT INTO \"odbxtest\" ( \"i1\", \"i16\", \"i32\", \"i64\", \"d9\", \"f4\", \"f3\", \"f8\", \"c2\", \"nc12\", \"str20\", \"nstr24\", \"clob64k\", \"nclob64k\", \"timeval\", \"timevaltz\", \"timestmp\", \"timestmptz\", \"dateval\" ) VALUES ( 1, 2, 4, 8, 1.333, 4.5, 3.99, 8.0, 'EN', 'aouaou', 'varstring', 'aouaouaouaou', 'clob', 'national character clob', '23:59:59+06', '23:59:59', '1999-01-01 00:00:00', '1999-01-01 00:00:00+06', '1999-01-01' )" },

	{ 1 % CONNMAX, "INSERT INTO \"odbxtest\" ( \"i1\", \"i16\", \"i32\", \"i64\", \"d9\", \"f4\", \"f3\", \"f8\", \"c2\", \"nc12\", \"str20\", \"nstr24\", \"clob64k\", \"nclob64k\", \"timeval\", \"timevaltz\", \"timestmp\", \"timestmptz\", \"dateval\" ) VALUES ( NULL, 2, 4, 8, 2.5, 4.0, 3.1, 8.5, 'FR', 'éçèéçè', NULL, 'éçèéçèéçèéçè', 'text', 'éçè very long national text éçè', '15:00:00', '15:00:00+00', '2005-12-31 23:59:59', '2005-12-31 23:59:59+00', '2005-12-01' )" },

	{ 0 % CONNMAX, "SELECT * FROM \"odbxtest\" WHERE \"i16\" = 2; SELECT * FROM \"odbxtest\" WHERE \"str20\" = 'empty'" },
	{ 1 % CONNMAX, "BEGIN TRANSACTION; UPDATE \"odbxtest\" SET \"i16\" = 3 WHERE \"str20\" LIKE '%string'; DELETE FROM \"odbxtest\" WHERE \"str20\" LIKE '%string'; ROLLBACK" },
	{ 0 % CONNMAX, "UPDATE \"odbxtest\" SET \"c2\" = '' WHERE \"c2\" = 'AA'" },
	{ 1 % CONNMAX, "SELECT * FROM \"odbxtest\"" },
	{ 0 % CONNMAX, "DELETE FROM \"odbxtest\" WHERE \"str20\" LIKE '%string'" },
	{ 1 % CONNMAX, "SELECT COUNT(*) AS \"count\" FROM \"odbxtest\"" },
	{ 0 % CONNMAX, "DROP TABLE \"odbxtest\"" },   // Can't create table immediately again after dropping it via second connection
	{ -1, NULL }
};

static struct odbxstmt* sqlite3_stmt[] = { sqlite3_multi, sqlite3_multi };



static struct odbxstmt sybase_multi[] = {
	{ 0 % CONNMAX, "SELECT * FROM \"odbxtest\"" },
	{ 0 % CONNMAX, "CREATE TABLE \"odbxtest\" ("
	"   \"i1\" CHARACTER VARYING(1) NULL,"
	"   \"i16\" SMALLINT,"
	"   \"i32\" INTEGER,"
	"   \"i64\" BIGINT,"
	"   \"d9\" DECIMAL(9,3),"
	"   \"f4\" REAL,"
	"   \"f3\" FLOAT(15),"
	"   \"f8\" DOUBLE PRECISION,"
	"   \"c2\" CHARACTER(2),"
	"   \"nc12\" NATIONAL CHARACTER(12),"
	"   \"str20\" CHARACTER VARYING(20) NULL,"
	"   \"nstr24\" NATIONAL CHARACTER VARYING(24),"
	"   \"clob64k\" TEXT,"
	"   \"timeval\" TIME,"
	"   \"timestmp\" DATETIME,"
	"   \"dateval\" DATE"
	")" },

	{ 1 % CONNMAX, "INSERT INTO \"odbxtest\" ( \"i1\", \"i16\", \"i32\", \"i64\", \"d9\", \"f4\", \"f3\", \"f8\", \"c2\", \"nc12\", \"str20\", \"nstr24\", \"clob64k\", \"timeval\", \"timestmp\", \"dateval\" ) VALUES ( '0', 1000, 100000, 10000000, 12345.123, 10000.1, 100.1, 10000000.1, 'DE', 'äöüäöü', 'first string', 'äöüäöüäöüäöü', 'a very long text', '15:00:00', '2000-01-01 00:00:01', '2000-02-29' )" },

	{ 0 % CONNMAX, "INSERT INTO \"odbxtest\" ( \"i1\", \"i16\", \"i32\", \"i64\", \"d9\", \"f4\", \"f3\", \"f8\", \"c2\", \"nc12\", \"str20\", \"nstr24\", \"clob64k\", \"timeval\", \"timestmp\", \"dateval\" ) VALUES ( '1', 2, 4, 8, 1.333, 4.5, 3.99, 8.0, 'EN', 'aouaou', 'varstring', 'aouaouaouaou', 'clob', '23:59:59', '1999-01-01 00:00:00', '1999-01-01' )" },

	{ 1 % CONNMAX, "INSERT INTO \"odbxtest\" ( \"i1\", \"i16\", \"i32\", \"i64\", \"d9\", \"f4\", \"f3\", \"f8\", \"c2\", \"nc12\", \"str20\", \"nstr24\", \"clob64k\", \"timeval\", \"timestmp\", \"dateval\" ) VALUES ( NULL, 2, 4, 8, 2.5, 4.0, 3.1, 8.5, 'FR', 'éçèéçè', NULL, 'éçèéçèéçèéçè', 'text', '15:00:00', '2005-12-31 23:59:59', '2005-12-01' )" },

	{ 0 % CONNMAX, "SELECT * FROM \"odbxtest\" WHERE \"i16\" = 2   SELECT * FROM \"odbxtest\" WHERE \"str20\" = 'empty'" },
	{ 1 % CONNMAX, "BEGIN TRANSACTION   UPDATE \"odbxtest\" SET \"i16\" = 3 WHERE \"str20\" LIKE '%string'   DELETE FROM \"odbxtest\" WHERE \"str20\" LIKE '%string'   ROLLBACK" },
	{ 0 % CONNMAX, "UPDATE \"odbxtest\" SET \"c2\" = '' WHERE \"c2\" = 'AA'" },
	{ 1 % CONNMAX, "SELECT * FROM \"odbxtest\"" },
	{ 0 % CONNMAX, "DELETE FROM \"odbxtest\" WHERE \"str20\" LIKE '%string'" },
	{ 1 % CONNMAX, "SELECT COUNT(*) AS \"count\" FROM \"odbxtest\"" },
	{ 0 % CONNMAX, "DROP TABLE \"odbxtest\"" },
	{ -1, NULL }
};

static struct odbxstmt* sybase_stmt[] = { sybase_multi, sybase_multi };
