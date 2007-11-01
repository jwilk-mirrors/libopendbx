/*
 *  OpenDBX - A simple but extensible database abstraction layer
 *  Copyright (C) 2006-2007 Norbert Sendetzky and others
 *
 *  Distributed under the terms of the GNU Library General Public Licence
 * version 2 or (at your option) any later version.
 */



#include "oracle_lo.h"
#include <stdlib.h>



/*
 *  Declaration of Oracle capabilities
 */

struct odbx_lo_ops oracle_odbx_lo_ops = {
	.open = oracle_odbx_lo_open,
	.close = oracle_odbx_lo_close,
	.read = oracle_odbx_lo_read,
};



/*
 *  ODBX large object operations
 *  Oracle style
 */


static int oracle_odbx_lo_open( odbx_result_t* result, odbx_lo_t** lo, const char* value )
{
	struct oralob* oralob;
	struct oraconn* conn = (struct oraconn*) handle->aux;


	if( ( *lo = (odbx_lo_t*) malloc( sizeof( struct odbx_lo_t ) ) ) == NULL )
	{
		return -ODBX_ERR_NOMEM;
	}

	if( ( oralob = (struct oralob*) malloc( sizeof( struct oralob ) ) ) == NULL )
	{
		free( *lo );
		*lo = NULL;

		return -ODBX_ERR_NOMEM;
	}

	oralob->lob = (OCILobLocator*) value;
	oralob->offset = 0;

	(*lo)->result = result;
	(*lo)->generic = (void*) oralob;

	if( ( conn->errcode = OCILobOpen( conn->ctx, conn->err, (OCILobLocator*) value, OCI_LOB_READONLY ) ) != OCI_SUCCESS )   // OCI_LOB_READWRITE
	{
		free( (*lo)->generic );
		(*lo)->generic = NULL;

		free( *lo );
		*lo = NULL;

		return -ODBX_ERR_BACKEND;
	}

	return ODBX_ERR_SUCCESS;
}



static int oracle_odbx_lo_close( odbx_lo_t* lo )
{
	if( lo->result == NULL || lo->result->handle == NULL || lo->result->handle->aux == NULL )
	{
		return ODBX_ERR_HANDLE;
	}

	struct oraconn* conn = (struct oraconn*) lo->result->handle->aux;

	if( ( conn->errcode = OCILobClose( conn->ctx, conn->err, lo->generic ) ) != OCI_SUCCESS )
	{
		return -ODBX_ERR_BACKEND;
	}

	free( lo->generic );
	lo->generic = NULL;
	free( lo );

	return ODBX_ERR_SUCCESS;
}



static ssize_t oracle_odbx_lo_read( odbx_lo_t* lo, void* buffer, size_t buflen )
{
	ub4 len = 0;
	struct oraconn* conn = (struct oraconn*) lo->result->handle->aux;
	struct oralob* oralob = (struct oralob*) lo->generic;


	if( lo->generic == NULL ) { return -ODBX_ERR_HANDLE; }

	conn->errcode = OCILobRead( conn->ctx, conn->err, lo->generic, &len, oralob->offset, buffer, buflen, NULL, NULL, 0, SQLCS_IMPLICIT );

	switch( conn->errcode )
	{
		case OCI_SUCCESS:
		case OCI_NEED_DATA:
			oralob->offset += len;
			return (ssize_t) len;
		default:
			return -ODBX_ERR_BACKEND;
	}
}
