#include <opendbx/api>
#include <string>
#include <map>


using std::string;
using std::map;



#ifndef COMMANDS_HPP
#define COMMANDS_HPP



class Commands
{
protected:

	typedef void (Commands::*cmdfunc)( const string& );

	OpenDBX::Conn m_conn;
	std::map<string, cmdfunc> m_cmds;

	void help( const string& str );
	void quit( const string& str );

public:

	Commands( OpenDBX::Conn& conn );

	void exec( const string& cmd );
};



#endif
