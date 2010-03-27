/*
 *  OpenDBX - A simple but extensible database abstraction layer
 *  Copyright (C) 2004-2010 Norbert Sendetzky and others
 *
 *  Distributed under the terms of the GNU Library General Public Licence
 * version 2 or (at your option) any later version.
 */


#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>
#include "opendbx/api"


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_GETOPT_H
#include <getopt.h>
#endif

#ifdef HAVE_PTHREAD_H
#include <pthread.h>
#endif


#define NUMTHREADS 10



using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::vector;
using OpenDBX::Conn;
using OpenDBX::Stmt;
using OpenDBX::Result;
using OpenDBX::Lob;



struct my_settings
{
	string backend;
	string host;
	string port;
	string db;
	string user;
	string pass;
	int runs;
	int verbose;
};





void help( char* name )
{
	cout << "Usage: " << name << " <options>" << endl;
	cout << "   -b <backend>    Database backend (mysql, pgsql, sqlite, sqlite3, firebird, freetds, sybase)" << endl;
	cout << "   -h <server>     Server name, IP address or directory" << endl;
	cout << "   -p <port>       Server port" << endl;
	cout << "   -d <database>   Database or file name" << endl;
	cout << "   -u <user>       User name for authentication" << endl;
	cout << "   -w <password>   Password for authentication" << endl;
	cout << "   -e              Force encrypted connection" << endl;
	cout << "   -r              Number of runs" << endl;
	cout << "   -v              Verbose mode" << endl;
}



void* threadtest( void* arg )
{
	int i;
	odbxres status;
	struct my_settings* set = (struct my_settings*) arg;

	OpenDBX::Conn conn = Conn( set->backend, set->host, set->port );
	conn.bind( set->db, set->user, set->pass );

	for( i = 0; i < set->runs; i++ )
	{
		if( set->verbose ) { cout << "  Execute(" << pthread_self() << "): SELECT * FROM \"odbxthreadtest\"" << endl; }
		OpenDBX::Result result = conn.create( "SELECT * FROM \"odbxthreadtest\"" ).execute();

		while( ( status = result.getResult() ) != ODBX_RES_DONE )
		{
			if( set->verbose ) { cout << "  Result::getResult(" << pthread_self() << ")" << endl; }

			switch( status )
			{
				case ODBX_RES_TIMEOUT:
				case ODBX_RES_NOROWS:
					continue;
			}

			while( result.getRow() != ODBX_ROW_DONE )
			{
				if( set->verbose ) { cout << "  Result::getRow(" << pthread_self() << ")" << endl; }
			}
		}
	}

	conn.unbind();
}



int main( int argc, char* argv[] )
{
	int i;
	struct my_settings set;
	int param, verbose = 0, encrypt = 0;
	OpenDBX::Conn conn;


	set.runs = 1;

	while( ( param = getopt( argc, argv, "b:h:p:d:u:w:e:r:v" ) ) != -1 )
	{
		switch( param )
		{
			case 'b':
				set.backend = string( optarg );
				break;
			case 'h':
				set.host = string( optarg );
				break;
			case 'p':
				set.port = string( optarg );
				break;
			case 'd':
				set.db = string( optarg );
				break;
			case 'u':
				set.user = string( optarg );
				break;
			case 'w':
				set.pass = string( optarg );
				break;
			case 'v':
				set.verbose = 1;
				break;
			case 'e':
				if( optarg != NULL ) { encrypt = (int) strtol( optarg, NULL, 10 ); }
				break;
			case 'r':
				if( optarg != NULL ) { set.runs = (int) strtol( optarg, NULL, 10 ); }
				break;
			default:
				cerr << "Unknown option '" << (char) param << "' with arg '" << (char*) optarg << "'" << endl;
				return 1;
		}
	}


	if( verbose )
	{
		cout << endl << "Parameter:" << endl;
		if( !set.backend.empty() ) { cout << "  backend=" << set.backend << endl; }
		if( !set.host.empty() ) { cout << "  host=" << set.host << endl; }
		if( !set.port.empty() ) { cout << "  port=" << set.port << endl; }
		if( !set.db.empty() ) { cout << "  database=" << set.db << endl; }
		if( !set.user.empty() ) { cout << "  user=" << set.user << endl; }
		if( !set.pass.empty() ) { cout << "  password=" << set.pass << endl; }
		if( encrypt ) { cout << "  encrypt=" << encrypt << endl; }
		if( set.runs ) { cout << "  runs=" << set.runs << endl; }
		if( set.verbose ) { cout << "  verbose=" << set.verbose << endl; }
	}


	try
	{
#ifndef HAVE_LIBPTHREAD

		if( set.verbose ) { cout << "  Conn::Conn()" << endl; }
		conn = Conn( set.backend, set.host, set.port );

		if( set.verbose ) { cout << "  Conn::bind()" << endl; }
		conn.bind( set.db, set.user, set.pass );

		if( set.verbose ) { cout << "  Execute: CREATE TABLE \"odbxthreadtest\" ( i32 INTEGER, str16 CHARACTER VARYING(16) )" << endl; }
		conn.create( "CREATE TABLE \"odbxthreadtest\" ( i32 INTEGER, str16 CHARACTER VARYING(16) )" ).execute().finish();

		if( set.verbose ) { cout << "  Execute: INSERT INTO \"odbxthreadtest\" ( i32, str16 ) VALUES ( 1, 'string 1' )" << endl; }
		conn.create( "INSERT INTO \"odbxthreadtest\" ( i32, str16 ) VALUES ( 1, 'string 1' )" ).execute().finish();

		if( set.verbose ) { cout << "  Execute: INSERT INTO \"odbxthreadtest\" ( i32, str16 ) VALUES ( 2, 'string 2' )" << endl; }
		conn.create( "INSERT INTO \"odbxthreadtest\" ( i32, str16 ) VALUES ( 2, 'string 2' )" ).execute().finish();

		if( set.verbose ) { cout << "  Execute: INSERT INTO \"odbxthreadtest\" ( i32, str16 ) VALUES ( 3, 'string 3' )" << endl; }
		conn.create( "INSERT INTO \"odbxthreadtest\" ( i32, str16 ) VALUES ( 3, 'string 3' )" ).execute().finish();

		if( set.verbose ) { cout << "  Execute: INSERT INTO \"odbxthreadtest\" ( i32, str16 ) VALUES ( 4, 'string 4' )" << endl; }
		conn.create( "INSERT INTO \"odbxthreadtest\" ( i32, str16 ) VALUES ( 4, 'string 4' )" ).execute().finish();

		if( set.verbose ) { cout << "  Execute: INSERT INTO \"odbxthreadtest\" ( i32, str16 ) VALUES ( 5, 'string 5' )" << endl; }
		conn.create( "INSERT INTO \"odbxthreadtest\" ( i32, str16 ) VALUES ( 5, 'string 5' )" ).execute().finish();


		int i;
		pthread_attr_t attr;
		pthread_t threads[NUMTHREADS];


		pthread_attr_init( &attr );
		pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_JOINABLE );

		for( i = 0; i < NUMTHREADS; i++ )
		{
			if( pthread_create( &(threads[i]), &attr, threadtest, (void*) &set ) )
			{
				cerr << "Could not create new thread" << endl;
				return 1;
			}
		}

		pthread_attr_destroy( &attr );

		for( i = 0; i < NUMTHREADS; i++ )
		{
			if( pthread_join( threads[i], NULL ) )
			{
				cerr << "Waiting for thread failed" << endl;
				return 1;
			}
		}


		if( set.verbose ) { cout << "  Execute: DROP TABLE \"odbxthreadtest\"" << endl; }
		conn.create( "DROP TABLE \"odbxthreadtest\"" ).execute().finish();

		if( set.verbose ) { cout << "  Conn::unbind()" << endl; }
		conn.unbind();

#else

		throw std::runtime_error( "pthread library not available. Please make sure it's installed and recompile the test application" );

#endif
	}
	catch( OpenDBX::Exception& oe )
	{
		cerr << "Caught exception: " << oe.what() << endl;
		return 1;
	}
	catch( std::exception& e )
	{
		cerr << "Caught STL exception: " << e.what() << endl;
		return 1;
	}
	catch( ... )
	{
		cerr << "Caught unknown exception" << endl;
		return 1;
	}

	return 0;
}
