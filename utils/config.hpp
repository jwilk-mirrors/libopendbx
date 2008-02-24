#include <string>
#include <stdexcept>
#include <confuse.h>



#ifndef CONFIG_HPP
#define CONFIG_HPP



using std::string;
using std::runtime_error;



class ConfigException : public runtime_error
{
public:
	explicit ConfigException( const string& str ) : runtime_error( str ) {}
};



class Config
{
		cfg_t* m_cfg;

public:

	Config( const char* file, cfg_opt_t* opts );
	~Config();

	const char* getStr( const string& name );
	const string getString( const string& name );
};



#endif
