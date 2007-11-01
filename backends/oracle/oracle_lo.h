/*
 *  OpenDBX - A simple but extensible database abstraction layer
 *  Copyright (C) 2006-2007 Norbert Sendetzky and others
 *
 *  Distributed under the terms of the GNU Library General Public Licence
 * version 2 or (at your option) any later version.
 */



#include "oraclebackend.h"



#ifndef ORACLE_LO_H
#define ORACLE_LO_H



struct oralob
{
	OCILobLocator* lob;
	ub4 offset;
};



/*
 *  Large object operations
 */

static int oracle_odbx_lo_open( odbx_result_t* result, odbx_lo_t** lo, const char* value );

static int oracle_odbx_lo_close( odbx_lo_t* lo );

// static uint64_t oracle_odbx_lo_length( odbx_lo_t* lo );

static ssize_t oracle_odbx_lo_read( odbx_lo_t* lo, void* buffer, size_t buflen );

// static ssize_t oracle_odbx_lo_write( odbx_lo_t* lo, void* buffer, size_t buflen );



#endif
