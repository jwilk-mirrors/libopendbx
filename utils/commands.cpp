#include "commands.hpp"
#include <iostream>



Commands::Commands( OpenDBX::Conn& conn )
{
	m_conn = conn;

	m_cmds[".help"] = &Commands::help;
	m_cmds[".quit"] = &Commands::quit;
}



void Commands::exec( const string& cmdstr )
{
	string cmd, arg;
	string::size_type pos = cmdstr.find_first_of( " \n\t" );

	if( pos != string::npos ) {
		cmd = cmdstr.substr( 0, pos );
		arg = cmdstr.substr( pos, string::npos );
	} else {
		cmd = cmdstr;
	}

	if( m_cmds.find( cmd ) == m_cmds.end() ) {
		std::cout << "Unknown command, use .help to list available commands" << std::endl;
	} else {
		(this->*m_cmds[cmd])( arg );
	}
}



void Commands::help( const string& str )
{
	std::cout <<
		"Available commands:\n"
		"    .help	print this help\n"
		"    .quit	exit program\n";
}



void Commands::quit( const string& str )
{
	std::cout << "Good bye" << std::endl;
	exit( 0 );
}
