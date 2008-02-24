#include "config.hpp"



Config::Config( const char* filename, cfg_opt_t* opts )
{
	if( filename == NULL )
	{
		throw ConfigException( "Configuration file parameter is mandatory" );
	}

	if( opts == NULL )
	{
		throw ConfigException( "Configuration options are mandatory" );
	}

	if( ( m_cfg = cfg_init( opts, CFGF_NONE ) ) == NULL )
	{
		throw ConfigException( "Unable to initialize configuration file parser" );
	}

	switch( cfg_parse( m_cfg, filename ) )
	{
		case CFG_SUCCESS:
			return;
		case CFG_FILE_ERROR:
			throw ConfigException( "Unable to open configuration file '" + string( filename ) + "'" );
		case CFG_PARSE_ERROR:
			throw ConfigException( "Unable to parse configuration file '" + string( filename ) + "'" );
		default:
			throw ConfigException( "Unknown error returned from configuration parser for file '" + string( filename ) + "'" );
	}
}



Config::~Config()
{
	cfg_free( m_cfg );
}



const char* Config::getStr( const string& name )
{
	return cfg_getstr( m_cfg, name.c_str() );
}



const string Config::getString( const string& name )
{
	return string( cfg_getstr( m_cfg, name.c_str() ) );
}
