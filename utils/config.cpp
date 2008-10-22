#include "config.hpp"



Config::Config( int argc, char* argv[] )
{
	int param;


	if( argc > 0 ) { m_progname = string( argv[0] ); }

	while( ( param = getopt( argc, argv, "?b:c:d:f:h:ik:p:s:u:w" ) ) != -1 )
	{
		switch( param )
		{
			case '?':
				cout << help( string( argv[0] ) ) << endl; return 0;
			case 'b':
				backend = optarg; break;
			case 'c':
				configfile = optarg; break;
			case 'd':
				database = optarg; break;
			case 'f':
				fparam.delimiter = optarg; break;
			case 'h':
				host = optarg; break;
			case 'i':
				iactive = true;
				fparam.header = true;
				break;
			case 'k':
				keywordfile = optarg; break;
			case 'p':
				port = optarg; break;
			case 's':
				fparam.separator = optarg; break;
			case 'u':
				user = optarg; break;
			case 'w':
				pass = true; break;
			default:
				const char tmp = (char) param;
				throw std::runtime_error( "Unknown option '" + string( &tmp ) + "' with value '" + string( optarg ) + "'" );
		}
	}
}



void Config::parseFile( const char* filename )
{
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

	if( filename == NULL )
	{
		throw ConfigException( string( gettext( "Configuration file parameter is mandatory" ) ) );
	}

	if( ( cfg_t* cfg = cfg_init( opts, CFGF_NONE ) ) == NULL )
	{
		throw ConfigException( string( gettext( "Unable to initialize configuration file parser" ) ) );
	}

	string error;

	switch( cfg_parse( cfg, filename ) )
	{
		case CFG_SUCCESS:
			cfg_free( m_cfg );
			return;
		case CFG_FILE_ERROR:
			error =  "Unable to open configuration file: "; break;
		case CFG_PARSE_ERROR:
			error =  "Unable to parse configuration file: "; break;
		default:
			error = "Unknown error returned from configuration parser. File name: "; break;
	}

	cfg_free( m_cfg );

	throw ConfigException( string( gettext( error.c_str() ) ) + string( filename ) );
}



const char* Config::getStr( const string& name )
{
	return cfg_getstr( m_cfg, name.c_str() );
}



const string Config::getString( const string& name )
{
	return string( cfg_getstr( m_cfg, name.c_str() ) );
}



const string Config::help()
{
	std::ostringstream help;

	help << gettext( "Read and execute semicolon-terminated SQL statements from stdin" ) << endl;
	help << endl;
	help << gettext( "Usage: " ) << m_progname << " " << " [options]" << endl;
	help << endl;
	help << "  -?              " << gettext( "print this help" ) << endl;
	help << "  -b backend      " << gettext( "name of the backend or path to the backend library" ) << endl;
	help << "  -c configfile   " << gettext( "read configuration from file" ) << endl;
	help << "  -d database     " << gettext( "database name or database file name" ) << endl;
	help << "  -f delimiter    " << gettext( "start/end delimiter for fields in output" ) << endl;
	help << "  -h host         " << gettext( "host name, IP address or path to the database server" ) << endl;
	help << "  -i              " << gettext( "interactive mode" ) << endl;
	help << "  -k keywordfile  " << gettext( "SQL keyword file for command completion" ) << endl;
	help << "  -p port         " << gettext( "port name or number of the database server" ) << endl;
	help << "  -s separator    " << gettext( "separator between fields in output" ) << endl;
	help << "  -u user         " << gettext( "user name for authentication" ) << endl;
	help << "  -w              " << gettext( "with prompt asking for the passphrase" ) << endl;

	return help.str();
}
