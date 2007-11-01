/*
 *  OpenDBX - A simple but extensible database abstraction layer
 *  Copyright (C) 2004-2007 Norbert Sendetzky <norbert@linuxnetworks.de>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307 USA.
 */



#include <opendbx/api.h>
#include <stdexcept>
#include <vector>
#include <string>



#ifndef ODBX_API_HPP
#define ODBX_API_HPP



extern "C++" {



namespace
{
	class Lob_Impl;
	class Result_Impl;
	class Stmt_Impl;
	class Conn_Impl;
}



namespace OpenDBX
{
	using std::string;


	class Lob;
	class Result;
	class Stmt;
	class Conn;



	class Exception : public std::runtime_error
	{
	protected:

		int m_error;
		int m_severity;

	public:

		Exception( string msg, int err, int severity );
		int getErrorCode();
		int getSeverity();
	};



	class Lob
	{
	protected:

		Lob_Impl* m_impl;
		int* m_ref;

		Lob();
		Lob( Lob_Impl* impl );

		friend class Result;

	public:

		~Lob();
		Lob& operator=( const Lob& ref );

		ssize_t read( void* buffer, size_t buflen );
	};



	class Result
	{
	protected:

		Result_Impl* m_impl;
		int* m_ref;

		Result();
		Result( Result_Impl* impl );

		friend class Stmt;

	public:

		~Result();

		Result& operator=( const Result& ref );

		odbxres getResult( struct timeval* timeout, unsigned long chunk );

		odbxrow getRow();
		uint64_t getRowsAffected();

		unsigned long getColumnCount();
		string getColumnName( unsigned long pos );
		odbxtype getColumnType( unsigned long pos );

		unsigned long getFieldLength( unsigned long pos );
		const char* getFieldValue( unsigned long pos );

		Lob getLob( const char* value );
	};



	class Stmt
	{
	public:

		enum Type { Simple, Prepared };
		enum Flags { None = 0, Null = 1, Quote = 2 };

		~Stmt();
		Stmt& operator=( const Stmt& ref );

		void bind( const string& data, int flags, size_t pos );

		string& escape( const string& from, string& to );
		string& escape( const char* from, unsigned long fromlen, string& to );

		size_t count();
		Result execute();

	protected:

		int* m_ref;
		Stmt_Impl* m_impl;

		Stmt();
		Stmt( Stmt_Impl* impl );

		friend class Conn;
	};



	class Conn
	{
	protected:

		Conn_Impl* m_impl;
		int* m_ref;

	public:

		Conn();
		Conn( const string& backend, const string& host, const string& port );
		~Conn();

		Conn& operator=( const Conn& ref );

		void bind( const string& database, const string& who, const string& cred, odbxbind method = ODBX_BIND_SIMPLE );
		void unbind();

		bool getCapability( odbxcap cap );

		void getOption( odbxopt option, void* value );
		void setOption( odbxopt option, void* value );

		Stmt create( const string& sql, Stmt::Type type );
	};



}   // namespace OpenDBX



}   // extern C++



#endif
