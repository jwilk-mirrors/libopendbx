 /*
 *  OpenDBX library handling interface
 *  Copyright (C) 2004-2007 Norbert Sendetzky <norbert@linuxnetworks.de>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307 USA.
 */



#include "odbxdrv.h"
#include <string.h>
#include <stdio.h>



#ifdef HAVE_CONFIG_H
#include <config.h>
#endif



#ifndef ODBXLIB_H
#define ODBXLIB_H



/* LIBPATH is defined in Makefile.am */
#define ODBX_PATHSIZE 1023



#if defined( ODBX_SINGLELIB )


#if defined( HAVE_LIBFBCLIENT ) || defined( HAVE_LIBGDS32 )
extern struct odbx_ops firebird_odbx_ops;
#endif
#if defined( HAVE_LIBSYBDB )
extern struct odbx_ops mssql_odbx_ops;
#endif
#if defined( HAVE_LIBMYSQLCLIENT_R ) || defined( HAVE_LIBMYSQLCLIENT ) || defined( HAVE_LIBMYSQL )
extern struct odbx_ops mysql_odbx_ops;
#endif
#if defined( HAVE_LIBODBC )
extern struct odbx_ops odbc_odbx_ops;
#endif
#if defined( HAVE_LIBCLNTSH )
extern struct odbx_ops oracle_odbx_ops;
#endif
#if defined( HAVE_LIBPQ )
extern struct odbx_ops pgsql_odbx_ops;
#endif
#if defined( HAVE_LIBSQLITE )
extern struct odbx_ops sqlite_odbx_ops;
#endif
#if defined( HAVE_LIBSQLITE3 )
extern struct odbx_ops sqlite3_odbx_ops;
#endif
#if defined( HAVE_LIBCT ) || defined( HAVE_LIBSYBCT_R ) || defined( HAVE_LIBSYBCT )
extern struct odbx_ops sybase_odbx_ops;
#endif


struct odbx_backend_syms
{
	const char* name;
	struct odbx_ops* ops;
};


static struct odbx_backend_syms odbx_lib_ops[] = {
#if defined( HAVE_LIBMYSQLCLIENT_R ) || defined( HAVE_LIBMYSQLCLIENT ) || defined( HAVE_LIBMYSQL )
	{ "mysql", &mysql_odbx_ops },
#endif
#if defined( HAVE_LIBPQ )
	{ "pgsql", &pgsql_odbx_ops },
#endif
#if defined( HAVE_LIBSQLITE3 )
	{ "sqlite3", &sqlite3_odbx_ops },
#endif
#if defined( HAVE_LIBFBCLIENT ) || defined( HAVE_LIBGDS32 )
	{ "firebird", &firebird_odbx_ops },
#endif
#if defined( HAVE_LIBODBC )
	{ "odbc", &odbc_odbx_ops },
#endif
#if defined( HAVE_LIBCLNTSH )
	{ "oracle", &oracle_odbx_ops },
#endif
#if defined( HAVE_LIBSYBDB )
	{ "mssql", &mssql_odbx_ops },
#endif
#if defined( HAVE_LIBCT ) || defined( HAVE_LIBSYBCT_R ) || defined( HAVE_LIBSYBCT )
	{ "sybase", &sybase_odbx_ops },
#endif
#if defined( HAVE_LIBSQLITE )
	{ "sqlite", &sqlite_odbx_ops },
#endif
	{ NULL, NULL }
};



static int _odbx_lib_open( struct odbx_t* handle, const char* backend )
{
	struct odbx_backend_syms* current = odbx_lib_ops;

	while( current->name != NULL )
	{
		if( strcmp( current->name, backend ) == 0 )
		{
			handle->ops = current->ops;
			return ODBX_ERR_SUCCESS;
		}

		current++;
	}

	return -ODBX_ERR_NOTEXIST;
}



static int _odbx_lib_close( struct odbx_t* handle )
{
	return ODBX_ERR_SUCCESS;
}



#elif defined( HAVE_LIBDL )


#ifdef HAVE_DLFCN_H
#include <dlfcn.h>
#endif



static int _odbx_lib_open( struct odbx_t* handle, const char* backend )
{
	size_t len, plen;
	char lib[ODBX_PATHSIZE+1];
	void (*odbxreg)(struct odbx_ops**);


	len = plen = snprintf( lib, ODBX_PATHSIZE, LIBPATH );
	len += snprintf( lib + len, ODBX_PATHSIZE - len, "/" );
	len += snprintf( lib + len, ODBX_PATHSIZE - len, LIBPREFIX );
	len += snprintf( lib + len, ODBX_PATHSIZE - len, backend );
	len += snprintf( lib + len, ODBX_PATHSIZE - len, "backend" );
	len += snprintf( lib + len, ODBX_PATHSIZE - len, LIBSUFFIX );

	if( len > ODBX_PATHSIZE ) { return -ODBX_ERR_SIZE; }
	lib[len] = '\0';

	if( ( handle->backend = dlopen( backend, RTLD_LAZY ) ) == NULL )
	{
		if( ( handle->backend = dlopen( lib + plen + 1, RTLD_LAZY ) ) == NULL )
		{
			if( ( handle->backend = dlopen( lib, RTLD_LAZY ) ) == NULL )
			{
				fprintf( stderr, dgettext( "opendbx1", gettext_noop( "Loading backend library %s, %s or %s failed" ) ), backend, lib + plen + 1, lib );
				fprintf( stderr, "\n%s\n", dlerror() );
				return -ODBX_ERR_NOTEXIST;
			}
		}
	}

	if( ( *(void **) (&odbxreg) = dlsym( handle->backend, "odbxdrv_register" ) ) == NULL )
	{
		return -ODBX_ERR_NOOP;
	}

	(*odbxreg)( &(handle->ops) );
	return ODBX_ERR_SUCCESS;
}



static int _odbx_lib_close( struct odbx_t* handle )
{
	if( dlclose( handle->backend ) != 0 )
	{
		return ODBX_ERR_PARAM;
	}

	return ODBX_ERR_SUCCESS;
}



#elif defined( WIN32 )


#ifdef HAVE_WINDOWS_H
#include <windows.h>
#endif



static int _odbx_lib_register( struct odbx_t* handle, const char* library )
{
	typedef void (WINAPI*regfunc)(struct odbx_ops**);

	regfunc odbxreg;


	if( ( handle->backend = (void*) LoadLibrary( library ) ) != NULL )
	{
		if( ( odbxreg = (regfunc) GetProcAddress( (HMODULE) handle->backend, "odbxdrv_register" ) ) != NULL )
		{
			odbxreg( &(handle->ops) );
			return ODBX_ERR_SUCCESS;
		}

		FreeLibrary( (HMODULE) handle->backend );
		return -ODBX_ERR_NOOP;
	}

	return -ODBX_ERR_NOTEXIST;
}



static int _odbx_lib_open( struct odbx_t* handle, const char* backend )
{
	char lib[ODBX_PATHSIZE+1];
	size_t len, plen;


	// SetDllDirectory( LIBPATH )

	len = plen = snprintf( lib, ODBX_PATHSIZE, PACKAGE );
	len += snprintf( lib + len, ODBX_PATHSIZE - len, "\\" );
	len += snprintf( lib + len, ODBX_PATHSIZE - len, LIBPREFIX );
	len += snprintf( lib + len, ODBX_PATHSIZE - len, backend );
	len += snprintf( lib + len, ODBX_PATHSIZE - len, "backend" );
	len += snprintf( lib + len, ODBX_PATHSIZE - len, LIBSUFFIX );

	if( len > ODBX_PATHSIZE ) { return -ODBX_ERR_SIZE; }
	lib[len] = '\0';

	if( _odbx_lib_register( handle, backend ) != ODBX_ERR_SUCCESS )
	{
		if( _odbx_lib_register( handle, lib + plen + 1 ) != ODBX_ERR_SUCCESS )
		{
			if( _odbx_lib_register( handle, lib ) != ODBX_ERR_SUCCESS )
			{
				fprintf( stderr, dgettext( "opendbx1", gettext_noop( "Loading backend library %s, %s or %s failed\n" ) ), backend, lib + plen + 1, lib );
				return -ODBX_ERR_NOTEXIST;
			}
		}
	}

	return ODBX_ERR_SUCCESS;
}



static int _odbx_lib_close( struct odbx_t* handle )
{
	if( FreeLibrary( (HMODULE) handle->backend ) != 0 )
	{
		return ODBX_ERR_PARAM;
	}

	return ODBX_ERR_SUCCESS;
}



#else
#error "Building shared libraries requires capabilities to load libraries dynamically"
#endif


#endif
