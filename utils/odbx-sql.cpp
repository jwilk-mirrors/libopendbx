#include "config.hpp"
#include "commands.hpp"
#include <opendbx/api>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stddef.h>
#include <locale.h>
#include <libintl.h>
#include <readline/readline.h>
#include <readline/history.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_GETOPT_H
#include <getopt.h>
#endif



using namespace OpenDBX;

using std::string;
using std::cout;
using std::cerr;
using std::endl;


struct format
{
	const char* delimiter;
	const char* separator;
};



const string help( const string& filename )
{
	std::ostringstream help;

	help << gettext( "Read and execute semicolon-terminated SQL statements from stdin" ) << endl;
	help << endl;
	help << gettext( "Usage: " ) << filename << " " << gettext( "<options>" ) << endl;
	help << endl;
	help << "    -c      " << gettext( "read configuration from file" ) << endl;
	help << "    -d      " << gettext( "start/end delimiter for fields in output" ) << endl;
	help << "    -h      " << gettext( "print help" ) << endl;
	help << "    -i      " << gettext( "interactive mode" ) << endl;
	help << "    -s      " << gettext( "separator between fields in output" ) << endl;

	return help.str();
}



void output( Result& result, struct format* fparam )
{
	odbxres stat;
	unsigned long fields;

	while( ( stat = result.getResult( NULL, 25 ) ) != ODBX_RES_DONE )
	{
		switch( stat )
		{
			case ODBX_RES_TIMEOUT:
			case ODBX_RES_NOROWS:
				continue;
			default:
				break;
		}

		fields = result.getColumnCount();

		while( result.getRow() != ODBX_ROW_DONE )
		{
			if( fields > 0 )
			{
				if( result.getFieldValue( 0 ) == NULL ) { cout << "NULL"; }
				else { cout << fparam->separator << result.getFieldValue( 0 ) << fparam->separator; }

				for( unsigned long i = 1; i < fields; i++ )
				{
					if( result.getFieldValue( i ) == NULL ) { cout << fparam->delimiter << "NULL"; }
					else { cout << fparam->delimiter << fparam->separator << result.getFieldValue( i ) << fparam->separator; }
				}
			}
			cout << endl;
		}
	}
}



void loopstmts( Conn& conn, struct format* fparam, bool iactive )
{
	char* line;
	size_t len;
	string sql;
	const char* fprompt = "";
	const char* cprompt = "";
	Commands cmd( conn );


	if( iactive )
	{
		cout << gettext( "Interactive SQL shell, use .help to list available commands" ) << endl;
		fprompt = "sql> ";
		cprompt = "  -> ";
		using_history();
	}

	while( ( line = readline( fprompt ) ) != NULL )
	{
		len = strlen( line );
		if( len == 0 ) { free( line ); continue;}
		if( line[0] == '.' ) { cmd.exec( string( line ) ); continue; }

		sql = string( line, len );
		free( line );

		if( sql[len-1] != ';' )
		{
			while( ( line = readline( cprompt ) ) != NULL )
			{
				len = strlen( line );
				sql += "\n" + string( line, len );
				free( line );

				if( sql[sql.size()-1] == ';' ) { break; }
			}
		}

		if( iactive ) { add_history( sql.c_str() ); }
		sql.erase( sql.size()-1, 1 );

		try
		{
			Stmt stmt = conn.create( Stmt::Simple, sql );
			Result result = stmt.execute();

			output( result, fparam );
		}
		catch( OpenDBX::Exception& oe )
		{
			cerr << gettext( "Error: " ) << oe.what() << endl;
			if( oe.getSeverity() < 0 ) { return; }
		}
	}
}



int main( int argc, char* argv[] )
{
	try
	{
		int param;
		bool iactive = false;
		char* filename = NULL;
		struct format fparam;


		setlocale( LC_ALL, "" );
		textdomain( "opendbx-utils" );
		bindtextdomain( "opendbx-utils", LOCALEDIR );

		fparam.delimiter = "\t";
		fparam.separator = "";

		cfg_opt_t opts[] =
		{
			CFG_STR( "backend", "mysql", CFGF_NONE ),
			CFG_STR( "host", "localhost", CFGF_NONE ),
			CFG_STR( "port", "", CFGF_NONE ),
			CFG_STR( "database", "", CFGF_NONE ),
			CFG_STR( "username", "", CFGF_NONE ),
			CFG_STR( "password", "", CFGF_NONE ),
			CFG_END()
		};

		while( ( param = getopt( argc, argv, "c:d:his:" ) ) != -1 )
		{
			switch( param )
			{
				case 'c':
					filename = optarg;
					break;
				case 'd':
					fparam.delimiter = optarg;
					break;
				case 'h':
					cout << help( string( argv[0] ) ) << endl;
					return 0;
				case 'i':
					iactive = true;
					break;
				case 's':
					fparam.separator = optarg;
					break;
				default:
					const char tmp = (char) param;
					throw std::runtime_error( "Unknown option '" + string( &tmp ) + "' with value '" + string( optarg ) + "'" );
			}
		}

		Config cfg( filename, opts );

		Conn conn( cfg.getStr( "backend" ), cfg.getStr( "host" ), cfg.getStr( "port" ) );
		conn.bind( cfg.getStr( "database" ), cfg.getStr( "username" ), cfg.getStr( "password" ), ODBX_BIND_SIMPLE );

		loopstmts( conn, &fparam, iactive );
		conn.unbind();
	}
	catch( OpenDBX::Exception& oe )
	{
		cerr << gettext( "Error: " ) << oe.what() << endl;
		return 1;
	}
	catch( ConfigException& ce )
	{
		cerr << gettext( "Error: " ) << ce.what() << endl;
		cerr << help( string( argv[0] ) ) << endl;
		return 1;
	}
	catch( runtime_error& e )
	{
		cerr << gettext( "Error: " ) << e.what() << endl;
		return 1;
	}
	catch( ... )
	{
		cerr << gettext( "Error: Caught unknown exception" ) << endl;
		return 1;
	}

	return 0;
}
