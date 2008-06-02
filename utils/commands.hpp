#include "odbx-sql.hpp"
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

	typedef void (Commands::*cmdfunc)( const string&, struct format* fparam );

	OpenDBX::Conn m_conn;
	std::map<string, cmdfunc> m_cmds;

	void header( const string& str, struct format* fparam );
	void help( const string& str, struct format* fparam );
	void quit( const string& str, struct format* fparam );

public:

	Commands( OpenDBX::Conn& conn );

	void exec( const string& cmd, struct format* fparam );
};



#endif
