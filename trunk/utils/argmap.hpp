/*
 *  Parameter parsing from file, command line and environment
 *  (c) 2006-2008, Norbert Sendetzky <norbert@linuxnetworks.de>
 *  Inspired by PowerDNS ArgvMap code
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */



#include <map>
#include <string>
#include <vector>
#include <stdexcept>



#ifndef ARGMAP_HPP
#define ARGMAP_HPP



using std::map;
using std::string;
using std::vector;



class ArgException : public std::runtime_error
{
public:
	explicit ArgException( const string& str ) : std::runtime_error( str ) {}
};



class ArgMap
{
protected:

	struct argopt {
		string longopt;
		string shortopt;
		string value;
		string help;
		bool req;
	};

	vector<string> m_items;
	vector<struct argopt> m_arg;
	map<string,size_t> m_long;
	map<string,size_t> m_short;

	int getOptionType( const string& arg );
	void parseItem( const string& item, bool strict );
	void parseLongOption( const string& keyvalue, bool strict );
	void parseShortOption( const string& arg, const string& value, bool strict );

public:

	enum opttype { Delimiter, Item, Short, Long };

	string& set( const string& longopt, const string& shortopt, const string& help, bool req = true );

	bool mustDo( const string &arg );
	long asLong( const string& arg );
	double asDouble( const string& arg );
	const string& asString( const string& arg );

	vector<string> getArgList();
	const vector<string>& getItems();

	bool checkArgv( int argc, char* argv[], const string& str, string& val );
	void parseArgv( int argc, char* argv[], bool strict = true );
	void parseFile( const string& filename, bool strict = true );
	void parseEnv( char* env[], bool strict = false );

	string help( const string& prefix = "" );
	string config();
};



#endif /* ARGUMENTS_HH */
