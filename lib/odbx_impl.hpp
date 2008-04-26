/*
 *  OpenDBX - A simple but extensible database abstraction layer
 *  Copyright (C) 2004-2007 Norbert Sendetzky and others
 *
 *  Distributed under the terms of the GNU Library General Public Licence
 * version 2 or (at your option) any later version.
 */



#include "lib/opendbx/api"
#include <map>
#include <string>
#include <vector>



#ifndef ODBX_IMPL_HPP
#define ODBX_IMPL_HPP



namespace OpenDBX
{
	using std::string;
	using std::vector;
	using std::map;
	using OpenDBX::Stmt;
	using OpenDBX::Result;
	using OpenDBX::Lob;



	struct Lob_Impl
	{
		odbx_lo_t* m_lo;
		odbx_result_t* m_result;


		Lob_Impl( odbx_result_t* result, const char* value );
		~Lob_Impl();

		ssize_t read( void* buffer, size_t buflen );
		ssize_t write( void* buffer, size_t buflen );
	};



	struct Result_Impl
	{
		odbx_t* m_handle;
		odbx_result_t* m_result;
		map<const string, unsigned long> m_pos;


		Result_Impl( odbx_t* handle );
		~Result_Impl();

		odbxres getResult( struct timeval* timeout, unsigned long chunk );

		odbxrow getRow();
		uint64_t rowsAffected();

		unsigned long columnCount();
		unsigned long columnPos( const string& name );
		string columnName( unsigned long pos );
		odbxtype columnType( unsigned long pos );

		unsigned long fieldLength( unsigned long pos );
		const char* fieldValue( unsigned long pos );

		Lob_Impl* getLob( const char* value );
	};



	struct Stmt_Impl
	{
		odbx_t* m_handle;


		Stmt_Impl( odbx_t* handle );
		virtual ~Stmt_Impl();

		static Stmt_Impl* instance( odbx_t* handle, Stmt::Type type, const string& sql );

// 		virtual void bind( const void* data, unsigned long size, size_t pos, int flags ) = 0;

// 		virtual size_t count() = 0;
		virtual Result_Impl* execute() = 0;
	};



	struct StmtSimple_Impl : public Stmt_Impl
	{
		string m_sql;
		vector<int> m_flags;
		vector<size_t> m_pos;
		vector<const void*> m_binds;
		vector<unsigned long> m_bindsize;
		size_t m_bufsize;
		char* m_buffer;


		StmtSimple_Impl( odbx_t* handle, const string& sql );
		StmtSimple_Impl();
		~StmtSimple_Impl();

		void bind( const void* data, unsigned long size, size_t pos, int flags );

		size_t count();
		Result_Impl* execute();

		inline void _exec_params();
		inline void _exec_noparams();
	};



	struct Conn_Impl
	{
		odbx_t* m_handle;
		char* m_escbuf;
		unsigned long m_escsize;

		inline char* _resize( char* buffer, size_t size );


		Conn_Impl( const char* backend, const char* host, const char* port );
		~Conn_Impl();

		void bind( const char* database, const char* who, const char* cred, odbxbind method = ODBX_BIND_SIMPLE );
		void unbind();

		bool getCapability( odbxcap cap );

		void getOption( odbxopt option, void* value );
		void setOption( odbxopt option, void* value );

		string& escape( const char* from, unsigned long fromlen, string& to );

		Stmt_Impl* create( Stmt::Type type, const char* sql, unsigned long length );
		Stmt_Impl* create( Stmt::Type type, const string& sql );
	};

}   // namespace



#endif
