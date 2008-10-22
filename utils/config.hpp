#include <string>
#include <stdexcept>
#include <confuse.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_GETOPT_H
#include <getopt.h>
#endif



#ifndef CONFIG_HPP
#define CONFIG_HPP



using std::map;
using std::string;
using std::runtime_error;



class ConfigException : public runtime_error
{
public:
	explicit ConfigException( const string& str ) : runtime_error( str ) {}
};



class Config
{
	string m_progname;
	map<string,string> m_config;

	void parseFile( const char* filename );

public:

	Config( int argc, char* argv[] );

	const char* getStr( const string& name );
	const string getString( const string& name );

	bool mustDo( const string& name );

	const string help();
};



#endif
