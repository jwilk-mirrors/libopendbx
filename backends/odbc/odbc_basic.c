/*
 *  OpenDBX - A simple but extensible database abstraction layer
 *  Copyright (C) 2005-2007 Norbert Sendetzky and others
 *
 *  Distributed under the terms of the GNU Library General Public Licence
 * version 2 or (at your option) any later version.
 */

// test
#include <stdio.h>

#include <string.h>
#include <strings.h>
#include "odbxdrv.h"
#include "odbc_basic.h"



/*
 *  Declaration of ODBC capabilities
 */

struct odbx_basic_ops odbc_odbx_basic_ops = {
	.init = odbc_odbx_init,
	.bind = odbc_odbx_bind,
	.unbind = odbc_odbx_unbind,
	.finish = odbc_odbx_finish,
	.get_option = odbc_odbx_get_option,
	.set_option = odbc_odbx_set_option,
	.error = odbc_odbx_error,
	.error_type = odbc_odbx_error_type,
	.escape = NULL,
	.query = odbc_odbx_query,
	.result = odbc_odbx_result,
	.result_free = odbc_odbx_result_free,
	.rows_affected = odbc_odbx_rows_affected,
	.row_fetch = odbc_odbx_row_fetch,
	.column_count = odbc_odbx_column_count,
	.column_name = odbc_odbx_column_name,
	.column_type = odbc_odbx_column_type,
	.field_length = odbc_odbx_field_length,
	.field_value = odbc_odbx_field_value,
};



/*
 *  OpenDBX basic operations
 *  ODBC style
 */


static int odbc_odbx_init( odbx_t* handle, const char* host, const char* port )
{
	if( ( handle->generic = malloc( sizeof( struct odbcgen ) ) ) == NULL )
	{
		return -ODBX_ERR_NOMEM;
	}

	struct odbcgen* gen = (struct odbcgen*) handle->generic;
// printf( "odbx_init(): start\n" );

	if( ( gen->err = SQLAllocHandle( SQL_HANDLE_ENV, SQL_NULL_HANDLE, &(gen->env) ) ) == SQL_SUCCESS )
	{
// printf( "odbx_init(): SQLAllocHandle\n" );
		if( ( gen->err = SQLSetEnvAttr( gen->env, SQL_ATTR_ODBC_VERSION, (void*) SQL_OV_ODBC3, 0 ) ) == SQL_SUCCESS )
		{
// printf( "odbx_init(): SQLSetEnvAttr\n" );
			if( ( gen->err = SQLAllocHandle( SQL_HANDLE_DBC, gen->env, &(gen->conn) ) ) == SQL_SUCCESS )
			{
// printf( "odbx_init(): SQLAllocHandle 2\n" );
				int len = strlen( host ) + 1;

				if( ( gen->server = (char*) malloc( len ) ) != NULL )
				{
					memcpy( gen->server, host, len );
// printf( "odbx_init(): success\n" );
					return ODBX_ERR_SUCCESS;
				}

// printf( "odbx_init(): SQLFreeHandle\n" );
				gen->err = SQLFreeHandle( SQL_HANDLE_DBC, gen->conn );
			}
		}

// printf( "odbx_init(): SQLFreeHandle 2\n" );
		gen->err = SQLFreeHandle( SQL_HANDLE_ENV, gen->env );
	}

// printf( "odbx_init(): failure\n" );
	free( handle->generic );
	return -ODBX_ERR_NOMEM;
}



static int odbc_odbx_bind( odbx_t* handle, const char* database, const char* who, const char* cred, int method )
{
// printf( "odbx_bind(): start\n" );
	if( handle->generic == NULL ) { return -ODBX_ERR_PARAM; }
	if( method != ODBX_BIND_SIMPLE ) { return -ODBX_ERR_NOTSUP; }

	struct odbcgen* gen = (struct odbcgen*) handle->generic;

	size_t wlen = 0;
	size_t clen = 0;

	if( who != NULL ) { wlen = strlen( who ); }
	if( cred != NULL ) { clen = strlen( cred ); }

// printf( "odbx_bind(): SQLConnect\n" );
	if( ( gen->err = SQLConnect( gen->conn, (SQLCHAR*) gen->server, strlen( gen->server ), (SQLCHAR*) who, wlen, (SQLCHAR*) cred, clen ) ) == SQL_SUCCESS )
	{
// printf( "odbx_bind(): SQLSetConnectAttr\n" );
		if( ( gen->err = SQLSetConnectAttr( gen->conn, SQL_ATTR_CURRENT_CATALOG, (SQLCHAR*) database, strlen( database ) ) ) == SQL_SUCCESS )
		{
			SQLUINTEGER mode = SQL_AUTOCOMMIT_ON;

// printf( "odbx_bind(): SQLSetConnectAttr 2\n" );
			if( ( gen->err = SQLSetConnectAttr( gen->conn, SQL_ATTR_AUTOCOMMIT, &mode, 0 ) ) == SQL_SUCCESS )
			{
				mode = SQL_TXN_READ_COMMITTED;

// printf( "odbx_bind(): SQLSetConnectAttr 3\n" );
				if( ( gen->err = SQLSetConnectAttr( gen->conn, SQL_DEFAULT_TXN_ISOLATION, &mode, 0 ) ) == SQL_SUCCESS )
				{
					mode = SQL_ASYNC_ENABLE_ON;

// printf( "odbx_bind(): SQLSetConnectAttr 4\n" );
					if( ( gen->err = SQLSetConnectAttr( gen->conn, SQL_ATTR_ASYNC_ENABLE, &mode, 0 ) ) == SQL_SUCCESS )
					{
// printf( "odbx_bind(): SQLAllocHandle\n" );
						if( ( gen->err = SQLAllocHandle( SQL_HANDLE_STMT, gen->conn, &(gen->stmt) ) ) == SQL_SUCCESS )
						{
// printf( "odbx_bind(): success\n" );
							return ODBX_ERR_SUCCESS;
						}
					}
				}
			}
		}

// printf( "odbx_bind(): SQLDisconnect\n" );
		gen->err = SQLDisconnect( gen->conn );
	}

// printf( "odbx_bind(): failure\n" );
	return -ODBX_ERR_BACKEND;
}



static int odbc_odbx_unbind( odbx_t* handle )
{
// printf( "odbx_unbind(): start\n" );
	if( handle->generic == NULL ) { return -ODBX_ERR_PARAM; }

	struct odbcgen* gen = (struct odbcgen*) handle->generic;

// printf( "odbx_unbind(): SQLFreeHandle\n" );
	if( gen->stmt != NULL && ( gen->err = SQLFreeHandle( SQL_HANDLE_STMT, gen->stmt ) ) != SQL_SUCCESS )
	{
		return -ODBX_ERR_BACKEND;
	}

	gen->stmt = NULL;

// printf( "odbx_unbind(): SQLDisconnect\n" );
	if( ( gen->err = SQLDisconnect( gen->conn ) ) != SQL_SUCCESS )
	{
// printf( "odbx_unbind(): return ODBX_ERR_BACKEND\n" );
			return -ODBX_ERR_BACKEND;
	}

// printf( "odbx_unbind(): success\n" );
	return ODBX_ERR_SUCCESS;
}



static int odbc_odbx_finish( odbx_t* handle )
{
// printf( "odbx_finish(): start\n" );
	if( handle->generic == NULL ) { return -ODBX_ERR_PARAM; }

	struct odbcgen* gen = (struct odbcgen*) handle->generic;

// printf( "odbx_finish(): SQLFreeHandle\n" );
	if( gen->conn != NULL && ( gen->err = SQLFreeHandle( SQL_HANDLE_DBC, gen->conn ) ) != SQL_SUCCESS )
	{
		return -ODBX_ERR_BACKEND;
	}

	gen->conn = NULL;

// printf( "odbx_finish(): SQLFreeHandle 2\n" );
	if( gen->env != NULL && ( gen->err = SQLFreeHandle( SQL_HANDLE_ENV, gen->env ) ) != SQL_SUCCESS )
	{
		return -ODBX_ERR_HANDLE;
	}

	gen->env = NULL;

	if( gen->server != NULL ) { free( gen->server ); }
	free( handle->generic );

// printf( "odbx_finish(): success\n" );
	return ODBX_ERR_SUCCESS;
}



static int odbc_odbx_get_option( odbx_t* handle, unsigned int option, void* value )
{
	switch( option )
	{
		case ODBX_OPT_API_VERSION:
			*(int*) value = 10100;
			break;
		case ODBX_OPT_THREAD_SAFE:
		case ODBX_OPT_CONNECT_TIMEOUT:
			*(int*) value = ODBX_ENABLE;
			break;
		case ODBX_OPT_TLS:
		case ODBX_OPT_MULTI_STATEMENTS:
		case ODBX_OPT_PAGED_RESULTS:
		case ODBX_OPT_COMPRESS:
			*(int*) value = ODBX_DISABLE;
			break;
		default:
// printf( "odbx_get_option(): unknown option\n" );
			return -ODBX_ERR_OPTION;
	}

// printf( "odbx_get_option(): success\n" );
	return ODBX_ERR_SUCCESS;
}



static int odbc_odbx_set_option( odbx_t* handle, unsigned int option, void* value )
{
	switch( option )
	{
		case ODBX_OPT_API_VERSION:
		case ODBX_OPT_THREAD_SAFE:
// printf( "odbx_get_option(): option read-only\n" );
			return -ODBX_ERR_OPTRO;
		case ODBX_OPT_TLS:
		case ODBX_OPT_MULTI_STATEMENTS:
		case ODBX_OPT_PAGED_RESULTS:
		case ODBX_OPT_COMPRESS:
// printf( "odbx_get_option(): option couldn't be changed\n" );
			return -ODBX_ERR_OPTWR;

		case ODBX_OPT_CONNECT_TIMEOUT:
			;   // gcc workaround
// printf( "odbx_set_option(): SQLSetConnectAttr\n" );
			struct odbcgen* gen = (struct odbcgen*) handle->generic;
 			if( ( gen->err = SQLSetConnectAttr( gen->conn, SQL_ATTR_LOGIN_TIMEOUT, (SQLPOINTER) value, SQL_IS_UINTEGER ) ) != SQL_SUCCESS )
			{
 				return -ODBX_ERR_BACKEND;
			}
			break;

		default:
			return -ODBX_ERR_OPTION;
	}

// printf( "odbx_set_option(): success\n" );
	return -ODBX_ERR_SUCCESS;
}



static const char* odbc_odbx_error( odbx_t* handle )
{
// printf( "odbc_odbx_error(): start\n" );
	SQLCHAR sqlstate[6];
	SQLINTEGER nerror;
	SQLSMALLINT msglen = 0;
	struct odbcgen* gen = (struct odbcgen*) handle->generic;

// printf( "odbc_odbx_error(): SQLGetDiagRec 1\n" );
	if( SQLGetDiagRec( SQL_HANDLE_STMT, gen->stmt, 1, sqlstate, &nerror, (SQLCHAR*) gen->errmsg, sizeof( gen->errmsg ), &msglen ) == SQL_SUCCESS )
	{
		return (const char*) gen->errmsg;
	}

// printf( "odbc_odbx_error(): SQLGetDiagRec 2\n" );
	if( SQLGetDiagRec( SQL_HANDLE_DBC, gen->conn, 1, sqlstate, &nerror, (SQLCHAR*) gen->errmsg, sizeof( gen->errmsg ), &msglen ) == SQL_SUCCESS )
	{
		return (const char*) gen->errmsg;
	}

// printf( "odbc_odbx_error(): SQLGetDiagRec 3\n" );
	if( SQLGetDiagRec( SQL_HANDLE_ENV, gen->env, 1, sqlstate, &nerror, (SQLCHAR*) gen->errmsg, sizeof( gen->errmsg ), &msglen ) == SQL_SUCCESS )
	{
		return (const char*) gen->errmsg;
	}

// printf( "odbc_odbx_error(): no message\n" );
	return "";
}



static int odbc_odbx_error_type( odbx_t* handle )
{
// printf( "odbc_odbx_error_type(): start\n" );
	switch( ((struct odbcgen*) handle->generic)->err )
	{
		case SQL_SUCCESS:
// printf( "odbc_odbx_error_type(): return success\n" );
			return 0;
		case SQL_SUCCESS_WITH_INFO:
// printf( "odbc_odbx_error_type(): return warning\n" );
			return 1;
		default:
// printf( "odbc_odbx_error_type(): return fatal\n" );
			return -1;
	}
}



static int odbc_odbx_query( odbx_t* handle, const char* query, unsigned long length )
{
// printf( "odbc_odbx_query(): start\n" );
	struct odbcgen* gen = (struct odbcgen*) handle->generic;

// 	if( !strncasecmp( "COMMIT", query, 6 ) )
// 	{
// printf( "odbc_odbx_query(): commit\n" );
// 		if( ( gen->err = SQLEndTran( SQL_HANDLE_DBC, gen->conn, SQL_COMMIT ) ) != SQL_SUCCESS )
// 		{
// 			return -ODBX_ERR_BACKEND;
// 		}
// 	}
//
// 	if( !strncasecmp( "ROLLBACK", query, 8 ) )
// 	{
// printf( "odbc_odbx_query(): rollback\n" );
// 		if( ( gen->err = SQLEndTran( SQL_HANDLE_DBC, gen->conn, SQL_ROLLBACK ) ) != SQL_SUCCESS )
// 		{
// 			return -ODBX_ERR_BACKEND;
// 		}
// 	}

// printf( "odbc_odbx_query(): SQLExecDirect\n" );
	if( ( gen->err = SQLExecDirect( gen->stmt, (SQLCHAR*) query, (SQLINTEGER) length ) ) != SQL_SUCCESS )
	{
		return -ODBX_ERR_BACKEND;
	}

	gen->resnum = 0;

	return ODBX_ERR_SUCCESS;
}



static int odbc_odbx_result( odbx_t* handle, odbx_result_t** result, struct timeval* timeout, unsigned long chunk )
{
// printf( "odbc_odbx_result(): start\n" );
	struct odbcgen* gen = (struct odbcgen*) handle->generic;


	if( gen == NULL )
	{
		return -ODBX_ERR_PARAM;
	}

// 	SQLSetConnectAttr( SQL_ATTR_CONNECTION_TIMEOUT )

	if( gen->resnum > 0 )
	{
// printf( "odbc_odbx_result(): SQLMoreResults\n" );
		switch( ( gen->err = SQLMoreResults( gen->stmt ) ) )
		{
			case SQL_SUCCESS:
// printf( "odbc_odbx_result(): more results\n" );
				break;
			case SQL_NO_DATA:
// printf( "odbc_odbx_result(): no more results\n" );
				gen->resnum = -1;
				return ODBX_RES_DONE;
			default:
				return -ODBX_ERR_BACKEND;
		}
	}
	gen->resnum++;

	if( ( *result = (odbx_result_t*) malloc( sizeof( odbx_result_t ) ) ) == NULL )
	{
		return -ODBX_ERR_NOMEM;
	}

	(*result)->generic = NULL;
	(*result)->aux = NULL;

	SQLSMALLINT cols;
// printf( "odbc_odbx_result(): SQLNumResultCols\n" );
	if( SQLNumResultCols( gen->stmt, &cols ) != SQL_SUCCESS )
	{
		return -ODBX_ERR_BACKEND;
	}

	if( cols == 0 ) { return ODBX_RES_NOROWS; }

//	allocate memory
//	SQLBindCol()

	return ODBX_RES_ROWS;
}



static void odbc_odbx_result_free( odbx_result_t* result )
{
	struct odbcgen* gen = (struct odbcgen*) result->handle->generic;
// 	struct odbcres* res = (struct odbcres*) result->generic;
// 	struct odbcraux* raux = (struct odbcraux*) result->aux;

	if( gen != NULL && gen->resnum == -1 ) { SQLCloseCursor( gen->stmt ); }

// 	if( res != NULL )
// 	{
// 		for( unsigned int i = 0; i < raux->cols; i++ )
// 		{
// 			free( res[i].data );
// 		}
// 		free( res );
// 	}

	result->generic = NULL;
}



static int odbc_odbx_row_fetch( odbx_result_t* result )
{
	switch( SQLFetch( ((struct odbcgen*) result->handle->generic)->stmt ) )
	{
		case SQL_SUCCESS:
			return 1;
		case SQL_NO_DATA:
			return 0;
	}

	return -ODBX_ERR_BACKEND;
}



static uint64_t odbc_odbx_rows_affected( odbx_result_t* result )
{
// printf( "odbc_odbx_rows_affected(): start\n" );
	SQLLEN count;
	struct odbcgen* gen = (struct odbcgen*) result->handle->generic;

// printf( "odbc_odbx_rows_affected(): SQLRowCount\n" );
	if( gen != NULL && SQLRowCount( gen->stmt, &count ) == SQL_SUCCESS )
	{
// printf( "odbc_odbx_rows_affected(): value=%ld\n", count );
		if( count < 0 ) { count = 0; }   // normalize if -1 (unknown) is returned
		return (uint64_t) count;
	}

// printf( "odbc_odbx_rows_affected(): error\n" );
	return 0;
}



static unsigned long odbc_odbx_column_count( odbx_result_t* result )
{
// 	SQLSMALLINT count;
// 	struct odbcgen* gen = (struct odbcgen*) result->handle->generic;
//
// 	if( gen != NULL && SQLNumResultCols( gen->stmt, &count ) == SQL_SUCCESS )
// 	{
// 		return (unsigned long) count;
// 	}

	return 0;
}



static const char* odbc_odbx_column_name( odbx_result_t* result, unsigned long pos )
{
// 	SQLSMALLINT len;
// 	struct odbcraux* raux = (struct odbcraux*) result->aux;
// 	struct odbcgen* gen = (struct odbcgen*) result->handle->generic;
//
// 	if( gen != NULL && SQLColAttribute( gen->stmt, (SQLUSMALLINT) pos+1, SQL_DESC_LABEL, (SQLPOINTER) raux->colname, sizeof( raux->colname ), &len, NULL ) == SQL_SUCCESS )
// 	{
// 		return (const char*) raux->colname;
// 	}

	return NULL;
}



static int odbc_odbx_column_type( odbx_result_t* result, unsigned long pos )
{
// 	SQLINTEGER sqltype;
// 	struct odbcgen* gen = (struct odbcgen*) result->handle->generic;
//
// 	if( gen != NULL && SQLColAttribute( gen->stmt, (SQLUSMALLINT) pos+1, SQL_DESC_TYPE, NULL, 0, NULL, (SQLPOINTER) sqltype ) == SQL_SUCCESS )
// 	{
// 		switch( sqltype )
// 		{
// 			case SQL_SMALLINT:
// 				return ODBX_TYPE_SMALLINT;
// 			case SQL_INTEGER:
// 				return ODBX_TYPE_INTEGER;
// 			case SQL_BIGINT:
// 				return ODBX_TYPE_BIGINT;
//
// 			case SQL_DECIMAL:
// 			case SQL_NUMERIC:
// 				return ODBX_TYPE_DECIMAL;
//
// 			case SQL_REAL:
// 				return ODBX_TYPE_REAL;
// 			case SQL_DOUBLE:
// 				return ODBX_TYPE_DOUBLE;
// 			case SQL_FLOAT:
// 				return ODBX_TYPE_FLOAT;
//
// 			case SQL_CHAR:
// 				return ODBX_TYPE_CHAR;
// 			case SQL_VARCHAR:
// 				return ODBX_TYPE_VARCHAR;
//
// 			case SQL_LONGVARCHAR:
// 				return ODBX_TYPE_CLOB;
// 			case SQL_BINARY:
// 			case SQL_VARBINARY:
// 			case SQL_LONGVARBINARY:
// 				return ODBX_TYPE_BLOB;
//
// 			case SQL_TYPE_DATE:
// 				return ODBX_TYPE_DATE;
// 			case SQL_TYPE_TIME:
// 				return ODBX_TYPE_TIME;
// 			case SQL_TYPE_TIMESTAMP:
// 				return ODBX_TYPE_TIMESTAMP;
// 		}
// 	}

	return ODBX_TYPE_UNKNOWN;
}



static unsigned long odbc_odbx_field_length( odbx_result_t* result, unsigned long pos )
{
//	SQLColAttribute( SQL_DESC_OCTET_LENGTH )
	return 0;
}



static const char* odbc_odbx_field_value( odbx_result_t* result, unsigned long pos )
{
//	SQLGetData()
	return NULL;
}



/*
 * ODBC private function
 */
