/*
 *  OpenDBX - A simple but extensible database abstraction layer
 *  Copyright (C) 2004-2009 Norbert Sendetzky and others
 *
 *  Distributed under the terms of the GNU Library General Public Licence
 * version 2 or (at your option) any later version.
 */



#include "odbxdrv.h"
#include <stdarg.h>
#include <stdio.h>
#include <time.h>


#ifdef ENABLE_DEBUGLOG
#define DEBUGLOG(x) x
#else
#define DEBUGLOG(x)
#endif



#ifdef ENABLE_DEBUGLOG


#ifndef ODBXLOG_H
#define ODBXLOG_H



static int _odbx_log_write( struct odbx_log* log, int level, const char* message, ... )
{
	if( level <= log->level )
	{
		int err;
		char timestr[32];
		time_t tval = time( NULL );

		err= strftime( timestr, 32, "%Y-%m-%d %H:%M:%S", localtime( &tval ) );
		if( err < 0 || err > 32 )
		{
			return -ODBX_ERR_SIZE;
		}

		fprintf( log->resource, "[%s] (%d) ", timestr, level );

		va_list ap;
		va_start( ap, message );
		vfprintf( log->resource, message, ap );
		va_end( ap );

		fprintf( log->resource, "\n" );
	}

	return ODBX_ERR_SUCCESS;
}


static int _odbx_log_open( struct odbx_log* log )
{
	int err;
	char file[32];

	err = snprintf( file, 32, "opendbx-%p.dbg", (void*) log );
	if( err < 0 || err > 32 )
	{
		return -ODBX_ERR_SIZE;
	}

	if( ( log->resource = (void*) fopen( file, "a" ) ) == NULL )
	{
		return -ODBX_ERR_PARAM;
	}

	_odbx_log_write( log, 1, "Start logging for '%p'", (void*) log );

	return ODBX_ERR_SUCCESS;
}



static int _odbx_log_close( struct odbx_log* log )
{
	_odbx_log_write( log, 1, "Stop logging for '%p'", (void*) log );

	if( fclose( log->resource ) != 0 )
	{
		return -ODBX_ERR_PARAM;
	}

	log->resource = NULL;

	return ODBX_ERR_SUCCESS;
}



#endif   // ODBXLOG_H


#endif
