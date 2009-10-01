/*
 *  OpenDBX - A simple but extensible database abstraction layer
 *  Copyright (C) 2005-2008 Norbert Sendetzky and others
 *
 *  Distributed under the terms of the GNU Library General Public Licence
 * version 2 or (at your option) any later version.
 */



#include "sqlite3_basic.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/*
 *  Declaration of SQLite capabilities
 */

struct odbx_basic_ops sqlite3_odbx_basic_ops = {
	.init = sqlite3_odbx_init,
	.bind = sqlite3_odbx_bind,
	.unbind = sqlite3_odbx_unbind,
	.finish = sqlite3_odbx_finish,
	.get_option = sqlite3_odbx_get_option,
	.set_option = sqlite3_odbx_set_option,
	.error = sqlite3_odbx_error,
	.error_type = sqlite3_odbx_error_type,
	.escape = NULL,
	.query = sqlite3_odbx_query,
	.result = sqlite3_odbx_result,
	.result_finish = sqlite3_odbx_result_finish,
	.rows_affected = sqlite3_odbx_rows_affected,
	.row_fetch = sqlite3_odbx_row_fetch,
	.column_count = sqlite3_odbx_column_count,
	.column_name = sqlite3_odbx_column_name,
	.column_type = sqlite3_odbx_column_type,
	.field_isnull = sqlite3_odbx_field_isnull,
	.field_length = sqlite3_odbx_field_length,
	.field_value = sqlite3_odbx_field_value,
};



/*
 *  Private sqlite3 error messages
 */

static const char* sqlite3_odbx_errmsg[] = {
	gettext_noop("Unknown error"),
	gettext_noop("Invalid parameter"),
	gettext_noop("Opening database failed"),
};



/*
 *  ODBX basic operations
 *  SQLite3 style
 */


/*
 *  SQLite3 doesn't connect to a database server. Instead it opens the database
 *  (a file on the harddisk) directly. Thus, host doesn't contain the name of a
 *  computer but of a directory in the filesystem instead. Port is unused.
 */

static int sqlite3_odbx_init( odbx_t* handle, const char* host, const char* port )
{
	DEBUGLOG( handle->log.write( &(handle->log), 1, "sqlite3_odbx_init() called" ); )

	if( ( handle->aux = malloc( sizeof( struct sconn ) ) ) == NULL )
	{
		return -ODBX_ERR_NOMEM;
	}

	struct sconn* aux = handle->aux;

	aux->res = NULL;
	aux->path = NULL;
	aux->pathlen = 0;
	aux->stmt = NULL;
	aux->tail = NULL;
	aux->length = 0;
	aux->err = SQLITE_OK;

	handle->generic = NULL;

	if( host != NULL )
	{
		aux->pathlen = strlen( host );   /* host == directory */

		if( ( aux->path = malloc( aux->pathlen + 1 ) ) == NULL )
		{
			free( handle->aux );
			handle->aux = NULL;

			return -ODBX_ERR_NOMEM;
		}

		snprintf( aux->path, aux->pathlen + 1, "%s", host );
	}

	return ODBX_ERR_SUCCESS;
}



/*
 *  username and password are not used by SQLite. It relies on the filesystem
 *  access control mechanisms for allowing or preventing database access.
 */

static int sqlite3_odbx_bind( odbx_t* handle, const char* database, const char* who, const char* cred, int method )
{
	DEBUGLOG( handle->log.write( &(handle->log), 1, "sqlite3_odbx_bind() called" ); )

	struct sconn* aux = (struct sconn*) handle->aux;


	if( aux == NULL ) { return -ODBX_ERR_PARAM; }

	if( method != ODBX_BIND_SIMPLE ) { return -ODBX_ERR_NOTSUP; }

	if( database != NULL )
	{
		size_t flen = strlen( database );

		if( ( aux->path = realloc( aux->path, aux->pathlen + flen + 1 ) ) == NULL )
		{
			return -ODBX_ERR_NOMEM;
		}

		snprintf( aux->path + aux->pathlen, flen + 1, "%s", database );
	}

	sqlite3* s3conn;
	if( ( aux->err = sqlite3_open( aux->path, &s3conn ) ) != SQLITE_OK )
	{
		return -ODBX_ERR_BACKEND;
	}
	handle->generic = (void*) s3conn;

	return ODBX_ERR_SUCCESS;
}



static int sqlite3_odbx_unbind( odbx_t* handle )
{
	DEBUGLOG( handle->log.write( &(handle->log), 1, "sqlite3_odbx_unbind() called" ); )

	struct sconn* aux = (struct sconn*) handle->aux;

	if( aux == NULL ) { return -ODBX_ERR_PARAM; }

	if( aux->res != NULL )
	{
		sqlite3_finalize( aux->res );
		aux->res = NULL;
	}

	if( aux->stmt != NULL )
	{
		aux->length = 0;
		free( aux->stmt );
		aux->stmt = NULL;
	}

	if( ( aux->err = sqlite3_close( (sqlite3*) handle->generic ) ) != SQLITE_OK )
	{
		return -ODBX_ERR_BACKEND;
	}

	handle->generic = NULL;
	return ODBX_ERR_SUCCESS;
}



static int sqlite3_odbx_finish( odbx_t* handle )
{
	DEBUGLOG( handle->log.write( &(handle->log), 1, "sqlite3_odbx_finish() called" ); )

	if( handle->aux != NULL )
	{
		free( ((struct sconn*) handle->aux)->path );
		free(  handle->aux );
		handle->aux = NULL;

		return ODBX_ERR_SUCCESS;
	}

	return -ODBX_ERR_PARAM;
}



static int sqlite3_odbx_get_option( odbx_t* handle, unsigned int option, void* value )
{
	DEBUGLOG( handle->log.write( &(handle->log), 1, "sqlite3_odbx_get_option() called" ); )

	switch( option )
	{
		case ODBX_OPT_API_VERSION:
			*(int*) value = APINUMBER;
			break;
		case ODBX_OPT_MULTI_STATEMENTS:
			*(int*) value = ODBX_ENABLE;
			break;
		case ODBX_OPT_THREAD_SAFE:
			if( sqlite3_threadsafe() != 0 ) { *(int*) value = ODBX_ENABLE; }
			else { *(int*) value = ODBX_DISABLE; }
			break;
		case ODBX_OPT_TLS:
		case ODBX_OPT_PAGED_RESULTS:
		case ODBX_OPT_COMPRESS:
		case ODBX_OPT_CONNECT_TIMEOUT:
			*(int*) value = ODBX_DISABLE;
			break;
		default:
			return -ODBX_ERR_OPTION;
	}

	return ODBX_ERR_SUCCESS;
}



static int sqlite3_odbx_set_option( odbx_t* handle, unsigned int option, void* value )
{
	DEBUGLOG( handle->log.write( &(handle->log), 1, "sqlite3_odbx_set_option() called" ); )

	switch( option )
	{
		case ODBX_OPT_API_VERSION:
		case ODBX_OPT_THREAD_SAFE:
			return -ODBX_ERR_OPTRO;
		case ODBX_OPT_MULTI_STATEMENTS:

			if( *((int*) value) == ODBX_ENABLE ) { return ODBX_ERR_SUCCESS; }
			break;

		case ODBX_OPT_PAGED_RESULTS:
		case ODBX_OPT_COMPRESS:
		case ODBX_OPT_TLS:
			break;
		default:
			return -ODBX_ERR_OPTION;
	}

	return -ODBX_ERR_OPTWR;
}



static const char* sqlite3_odbx_error( odbx_t* handle )
{
	DEBUGLOG( handle->log.write( &(handle->log), 1, "sqlite3_odbx_error() called" ); )

	if( handle->generic != NULL )
	{
		return sqlite3_errmsg( (sqlite3*) handle->generic );
	}

	if( handle->aux == NULL )
	{
		return dgettext( "opendbx1", sqlite3_odbx_errmsg[1] );
	}

	switch( ((struct sconn*) handle->aux)->err )
	{
		case SQLITE_CANTOPEN:
			return dgettext( "opendbx1", sqlite3_odbx_errmsg[2] );
	}

	return dgettext( "opendbx1", sqlite3_odbx_errmsg[0] );
}



static int sqlite3_odbx_error_type( odbx_t* handle )
{
	DEBUGLOG( handle->log.write( &(handle->log), 1, "sqlite3_odbx_error_type() called" ); )

	int err;

	if( handle->generic != NULL )
	{
		err = sqlite3_errcode( (sqlite3*) handle->generic );
	}
	else
	{
		if( handle->aux == NULL ) { return -1; }
		err = ((struct sconn*) handle->aux)->err;
	}

	switch( err )
	{
		case SQLITE_OK:
			return 0;
		case SQLITE_PERM:
		case SQLITE_NOMEM:
		case SQLITE_READONLY:
		case SQLITE_IOERR:
		case SQLITE_CORRUPT:
		case SQLITE_FULL:
		case SQLITE_CANTOPEN:
		case SQLITE_NOLFS:
		case SQLITE_AUTH:
		case SQLITE_NOTADB:
			return -1;
	}

	return 1;
}



static int sqlite3_odbx_query( odbx_t* handle, const char* query, unsigned long length )
{
	DEBUGLOG( handle->log.write( &(handle->log), 1, "sqlite3_odbx_query() called" ); )

	struct sconn* aux = (struct sconn*) handle->aux;

	if( query == NULL || aux == NULL )
	{
		return -ODBX_ERR_PARAM;
	}

	if( ( aux->stmt = malloc( length + 1 ) ) == NULL )
	{
		return -ODBX_ERR_NOMEM;
	}

	memcpy( aux->stmt, query, length );

	aux->stmt[length] = 0;
	aux->length = length;
	aux->tail = aux->stmt;

	return ODBX_ERR_SUCCESS;
}



static int sqlite3_odbx_result( odbx_t* handle, odbx_result_t** result, struct timeval* timeout, unsigned long chunk )
{
	DEBUGLOG( handle->log.write( &(handle->log), 1, "sqlite3_odbx_result() called" ); )

	struct sconn* aux = (struct sconn*) handle->aux;

	if( aux == NULL ) { return -ODBX_ERR_PARAM; }
	if( aux->length == 0 ) { return ODBX_RES_DONE; }    /* no more results */

	if( timeout != NULL )
	{
		sqlite3_busy_timeout( handle->generic, timeout->tv_sec * 1000 + timeout->tv_usec / 1000 );
	}

	if( aux->res == NULL )
	{
#ifdef HAVE_SQLITE3_PREPARE_V2
		if( ( aux->err = sqlite3_prepare_v2( (sqlite3*) handle->generic, aux->tail, aux->length, &aux->res, (const char**) &(aux->tail) ) ) != SQLITE_OK )
#else
		if( ( aux->err = sqlite3_prepare( (sqlite3*) handle->generic, aux->tail, aux->length, &aux->res, (const char**) &(aux->tail) ) ) != SQLITE_OK )
#endif
		{
			aux->length = 0;
			free( aux->stmt );
			aux->stmt = NULL;

			return -ODBX_ERR_BACKEND;
		}
	}

	switch( ( aux->err = sqlite3_step( aux->res ) ) )   // fetch first row and see if a busy timeout occurs
	{
		case SQLITE_BUSY:
#ifdef SQLITE_IOERR_BLOCKED
		case SQLITE_IOERR_BLOCKED:
#endif
			return ODBX_RES_TIMEOUT;
	}

	if( ( aux->length = strlen( aux->tail ) ) == 0 )
	{
		free( aux->stmt );
		aux->stmt = NULL;
	}

	switch( aux->err )
	{
		case SQLITE_ROW:
		case SQLITE_DONE:
		case SQLITE_OK:
			break;
		default:
			sqlite3_finalize( aux->res );
			aux->res = NULL;
			return ODBX_ERR_BACKEND;
	}

	if( ( *result = (odbx_result_t*) malloc( sizeof( struct odbx_result_t ) ) ) == NULL )
	{
		sqlite3_finalize( aux->res );
		aux->res = NULL;
		return -ODBX_ERR_NOMEM;
	}

	(*result)->generic = aux->res;
	aux->res = NULL;

	if( sqlite3_column_count( (*result)->generic ) == 0 )
	{
		return ODBX_RES_NOROWS;   /* empty or not SELECT like query */
	}

	return ODBX_RES_ROWS;   /* result is available */
}



static int sqlite3_odbx_result_finish( odbx_result_t* result )
{
	DEBUGLOG( result->handle->log.write( &(result->handle->log), 1, "sqlite3_odbx_result_finish() called" ); )

	struct sconn* aux = (struct sconn*) result->handle->aux;

	if( aux == NULL ) { return -ODBX_ERR_PARAM; }

	if( result->generic != NULL )
	{
		sqlite3_finalize( (sqlite3_stmt*) result->generic );
		result->generic = NULL;
	}

	free( result );

	return ODBX_ERR_SUCCESS;
}



static int sqlite3_odbx_row_fetch( odbx_result_t* result )
{
	DEBUGLOG( result->handle->log.write( &(result->handle->log), 1, "sqlite3_odbx_row_fetch() called" ); )

	struct sconn* aux = (struct sconn*) result->handle->aux;

	if( aux == NULL ) { return -ODBX_ERR_PARAM; }

	int err = aux->err;

	if( err != -1 ) { aux->err = -1; }   // use original error code the first time
	else { err = sqlite3_step( (sqlite3_stmt*) result->generic ); }

	switch( err )
	{
		case SQLITE_ROW:
			return ODBX_ROW_NEXT;
		case SQLITE_DONE:
		case SQLITE_OK:
		case SQLITE_MISUSE:   // Return DONE if function called more often afterwards
			return ODBX_ROW_DONE;
	}

	return -ODBX_ERR_BACKEND;
}



static uint64_t sqlite3_odbx_rows_affected( odbx_result_t* result )
{
	DEBUGLOG( result->handle->log.write( &(result->handle->log), 1, "sqlite3_odbx_rows_affected() called" ); )

	if( result->handle != NULL )
	{
		return (uint64_t) sqlite3_changes( (sqlite3*) result->handle->generic );
	}

	return 0;
}



static unsigned long sqlite3_odbx_column_count( odbx_result_t* result )
{
	DEBUGLOG( result->handle->log.write( &(result->handle->log), 1, "sqlite3_odbx_column_count() called" ); )

	return (unsigned long) sqlite3_column_count( (sqlite3_stmt*) result->generic );
}



static const char* sqlite3_odbx_column_name( odbx_result_t* result, unsigned long pos )
{
	DEBUGLOG( result->handle->log.write( &(result->handle->log), 1, "sqlite3_odbx_column_name() called" ); )

	return (const char*) sqlite3_column_name( (sqlite3_stmt*) result->generic, pos );
}



static int sqlite3_odbx_column_type( odbx_result_t* result, unsigned long pos )
{
	DEBUGLOG( result->handle->log.write( &(result->handle->log), 1, "sqlite3_odbx_column_type() called" ); )

	switch( sqlite3_column_type( (sqlite3_stmt*) result->generic, pos ) )
	{
		case SQLITE_INTEGER:
			return ODBX_TYPE_BIGINT;
		case SQLITE_FLOAT:
			return ODBX_TYPE_DOUBLE;
		case SQLITE_BLOB:
			return ODBX_TYPE_BLOB;
		case SQLITE_TEXT:
			return ODBX_TYPE_CLOB;
		default:
			return ODBX_TYPE_UNKNOWN;
	}
}



static int sqlite3_odbx_field_isnull( odbx_result_t* result, unsigned long pos )
{
	DEBUGLOG( result->handle->log.write( &(result->handle->log), 1, "sqlite3_odbx_field_isnull() called" ); )

	switch( sqlite3_column_type( (sqlite3_stmt*) result->generic, pos ) )
	{
		case SQLITE_NULL:
			return 1;
		case SQLITE_INTEGER:
		case SQLITE_FLOAT:
		case SQLITE_BLOB:
		case SQLITE_TEXT:
			return 0;
		default:
			return ODBX_ERR_BACKEND;
	}
}



static unsigned long sqlite3_odbx_field_length( odbx_result_t* result, unsigned long pos )
{
	DEBUGLOG( result->handle->log.write( &(result->handle->log), 1, "sqlite3_odbx_field_length() called" ); )

	return (unsigned long) sqlite3_column_bytes( (sqlite3_stmt*) result->generic, pos );
}



static const char* sqlite3_odbx_field_value( odbx_result_t* result, unsigned long pos )
{
	DEBUGLOG( result->handle->log.write( &(result->handle->log), 1, "sqlite3_odbx_field_value() called" ); )

	return (const char*) sqlite3_column_blob( (sqlite3_stmt*) result->generic, pos );
}
