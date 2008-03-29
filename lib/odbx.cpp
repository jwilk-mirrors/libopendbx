/*
 *  OpenDBX - A simple but extensible database abstraction layer
 *  Copyright (C) 2004-2007 Norbert Sendetzky and others
 *
 *  Distributed under the terms of the GNU Library General Public Licence
 * version 2 or (at your option) any later version.
 */



#include "opendbx/api"
#include "odbx_impl.hpp"
#include "odbxdrv.h"
#include <cstdlib>





/*
 *  Public C++ OpenDBX library interface
 */

namespace OpenDBX
{



/*
 *  OpenDBX exception implementation
 */


	Exception::Exception( string msg, int error, int severity ) : std::runtime_error( msg )
	{
		m_error = error;
		m_severity = severity;
	}



	int Exception::getErrorCode()
	{
		return m_error;
	}



	int Exception::getSeverity()
	{
		return m_severity;
	}





	/*
	*  OpenDBX large object interface
	*/



	Lob::Lob( Lob_Impl* impl )
	{
		m_impl = impl;
		m_ref = new int;

		*m_ref = 1;
	}



	Lob::~Lob()
	{
		if( --(*m_ref) == 0 )
		{
			delete m_impl;
			delete m_ref;
		}
	}



	Lob& Lob::operator=( const Lob& ref )
	{
		if( --(*m_ref) == 0 )
		{
			delete m_impl;
			delete m_ref;
		}

		m_impl = ref.m_impl;
		m_ref = ref.m_ref;
		++(*m_ref);

		return *this;
	}



	ssize_t Lob::read( void* buffer, size_t buflen )
	{
		return m_impl->read( buffer, buflen );
	}





	/*
	*  OpenDBX result interface
	*/



	Result::Result( Result_Impl* impl )
	{
		m_impl = impl;
		m_ref = new int;

		*m_ref = 1;
	}



	Result::~Result()
	{
		if( --(*m_ref) == 0 )
		{
			delete m_impl;
			delete m_ref;
		}
	}



	Result& Result::operator=( const Result& ref )
	{
		if( --(*m_ref) == 0 )
		{
			delete m_impl;
			delete m_ref;
		}

		m_impl = ref.m_impl;
		m_ref = ref.m_ref;
		++(*m_ref);

		return *this;
	}



	odbxres Result::getResult( struct timeval* timeout, unsigned long chunk )
	{
		return m_impl->getResult( timeout, chunk );
	}



	odbxrow Result::getRow()
	{
		return m_impl->getRow();
	}



	uint64_t Result::getRowsAffected()
	{
		return m_impl->getRowsAffected();
	}



	unsigned long Result::getColumnCount()
	{
		return m_impl->getColumnCount();
	}



	string Result::getColumnName( unsigned long pos )
	{
		return m_impl->getColumnName( pos );
	}



	odbxtype Result::getColumnType( unsigned long pos )
	{
		return m_impl->getColumnType( pos );
	}



	unsigned long Result::getFieldLength( unsigned long pos )
	{
		return m_impl->getFieldLength( pos );
	}



	const char* Result::getFieldValue( unsigned long pos )
	{
		return m_impl->getFieldValue( pos );
	}


	Lob Result::getLob( const char* value )
	{
		return m_impl->getLob( value );
	}





	/*
	*  OpenDBX Stmt interface
	*/



	Stmt::Stmt( Stmt_Impl* impl )
	{
		m_impl = impl;
		m_ref = new int;

		*m_ref = 1;
	}



	Stmt::~Stmt()
	{
		if( --(*m_ref) == 0 )
		{
			delete m_impl;
			delete m_ref;
		}
	}



	Stmt& Stmt::operator=( const Stmt& ref )
	{
		if( --(*m_ref) == 0 )
		{
			delete m_impl;
			delete m_ref;
		}

		m_impl = ref.m_impl;
		m_ref = ref.m_ref;
		++(*m_ref);

		return *this;
	}



// 	void Stmt::bind( const void* data, unsigned long size, size_t pos, int flags )
// 	{
// 		m_impl->bind( data, size, pos, flags );
// 	}



	string& Stmt::escape( const string& from, string& to )
	{
		return m_impl->escape( from.c_str(), from.size(), to );
	}



	string& Stmt::escape( const char* from, unsigned long fromlen, string& to )
	{
		return m_impl->escape( from, fromlen, to );
	}



// 	size_t Stmt::count()
// 	{
// 		return m_impl->count();
// 	}



	Result Stmt::execute()
	{
		return Result( m_impl->execute() );
	}





	/*
	*  OpenDBX connection interface
	*/


	Conn::Conn()
	{
		m_impl = NULL;
		m_ref = NULL;
	}


	Conn::Conn( const char* backend, const char* host, const char* port )
	{
		m_impl = new Conn_Impl( backend, host, port );
		m_ref = new int;

		*m_ref = 1;
	}


	Conn::Conn( const string& backend, const string& host, const string& port )
	{
		m_impl = new Conn_Impl( backend.c_str(), host.c_str(), port.c_str() );
		m_ref = new int;

		*m_ref = 1;
	}



	Conn::~Conn()
	{
		if( m_ref != NULL && --(*m_ref) == 0 )
		{
			delete m_impl;
			delete m_ref;
		}
	}



	Conn& Conn::operator=( const Conn& ref )
	{
		if( m_ref != NULL && --(*m_ref) == 0 )
		{
			delete m_impl;
			delete m_ref;
		}

		m_impl = ref.m_impl;
		m_ref = ref.m_ref;
		++(*m_ref);

		return *this;
	}



	void Conn::bind( const char* database, const char* who, const char* cred, odbxbind method )
	{
		if( m_impl == NULL )
		{
			throw Exception( string( odbx_error( NULL, -ODBX_ERR_HANDLE ) ), -ODBX_ERR_HANDLE, odbx_error_type( NULL, -ODBX_ERR_HANDLE ) );
		}

		m_impl->bind( database, who, cred, method );
	}



	void Conn::bind( const string& database, const string& who, const string& cred, odbxbind method )
	{
		if( m_impl == NULL )
		{
			throw Exception( string( odbx_error( NULL, -ODBX_ERR_HANDLE ) ), -ODBX_ERR_HANDLE, odbx_error_type( NULL, -ODBX_ERR_HANDLE ) );
		}

		m_impl->bind( database.c_str(), who.c_str(), cred.c_str(), method );
	}



	void Conn::unbind()
	{
		if( m_impl == NULL )
		{
			throw Exception( string( odbx_error( NULL, -ODBX_ERR_HANDLE ) ), -ODBX_ERR_HANDLE, odbx_error_type( NULL, -ODBX_ERR_HANDLE ) );
		}

		m_impl->unbind();
	}



	bool Conn::getCapability( odbxcap cap )
	{
		if( m_impl == NULL )
		{
			throw Exception( string( odbx_error( NULL, -ODBX_ERR_HANDLE ) ), -ODBX_ERR_HANDLE, odbx_error_type( NULL, -ODBX_ERR_HANDLE ) );
		}

		return m_impl->getCapability( cap );
	}



	void Conn::getOption( odbxopt option, void* value )
	{
		if( m_impl == NULL )
		{
			throw Exception( string( odbx_error( NULL, -ODBX_ERR_HANDLE ) ), -ODBX_ERR_HANDLE, odbx_error_type( NULL, -ODBX_ERR_HANDLE ) );
		}

		m_impl->getOption( option, value );
	}



	void Conn::setOption( odbxopt option, void* value )
	{
		if( m_impl == NULL )
		{
			throw Exception( string( odbx_error( NULL, -ODBX_ERR_HANDLE ) ), -ODBX_ERR_HANDLE, odbx_error_type( NULL, -ODBX_ERR_HANDLE ) );
		}

		m_impl->setOption( option, value );
	}



	Stmt Conn::create( Stmt::Type type, const char* sql, unsigned long length )
	{
		if( m_impl == NULL )
		{
			throw Exception( string( odbx_error( NULL, -ODBX_ERR_HANDLE ) ), -ODBX_ERR_HANDLE, odbx_error_type( NULL, -ODBX_ERR_HANDLE ) );
		}

		return Stmt( m_impl->create( type, string( sql, length ) ) );
	}



	Stmt Conn::create( Stmt::Type type, const string& sql )
	{
		if( m_impl == NULL )
		{
			throw Exception( string( odbx_error( NULL, -ODBX_ERR_HANDLE ) ), -ODBX_ERR_HANDLE, odbx_error_type( NULL, -ODBX_ERR_HANDLE ) );
		}

		return Stmt( m_impl->create( type, sql ) );
	}



}   // namespace OpenDBX





/*
 *  Private C++ OpenDBX library impementation
 */

namespace OpenDBX
{
	using OpenDBX::Exception;



	/*
	*  OpenDBX large object implementation
	*/



	Lob_Impl::Lob_Impl( odbx_result_t* result, const char* value )
	{
		int err;

		m_result = result;

		if( ( err = odbx_lo_open( result, &m_lo, value ) ) < 0 )
		{
			throw( Exception( string( odbx_error( m_result->handle, err ) ), err, odbx_error_type( m_result->handle, err ) ) );
		}
	}



	Lob_Impl::~Lob_Impl()
	{
		if( m_lo != NULL ) { odbx_lo_close( m_lo ); }
	}



	ssize_t Lob_Impl::read( void* buffer, size_t buflen )
	{
		return odbx_lo_read( m_lo, buffer, buflen );
	}





	/*
	*  OpenDBX result implementation
	*/



	Result_Impl::Result_Impl( odbx_t* handle )
	{
		m_handle = handle;
		m_result = NULL;
	}



	Result_Impl::~Result_Impl()
	{
		int err;

		if( m_result != NULL ) { odbx_result_finish( m_result ); }
		while( ( err = odbx_result( m_handle, &m_result, NULL, 0 ) ) != ODBX_RES_DONE && odbx_error_type( m_handle, err ) >= 0 );
	}



	odbxres Result_Impl::getResult( struct timeval* timeout, unsigned long chunk )
	{
		int err;

		if( m_result != NULL )
		{
			if( ( err = odbx_result_finish( m_result ) ) != ODBX_ERR_SUCCESS )
			{
				throw( Exception( string( odbx_error( m_handle, err ) ), err, odbx_error_type( m_handle, err ) ) );
			}
		}

		if( ( err = odbx_result( m_handle, &m_result, timeout, chunk ) ) < 0 )
		{
			throw( Exception( string( odbx_error( m_handle, err ) ), err, odbx_error_type( m_handle, err ) ) );
		}

		return (odbxres) err;
	}



	odbxrow Result_Impl::getRow()
	{
		int err;

		if( ( err =  odbx_row_fetch( m_result ) ) < 0 )
		{
			throw( Exception( string( odbx_error( m_handle, err ) ), err, odbx_error_type( m_handle, err ) ) );
		}

		return (odbxrow) err;
	}



	uint64_t Result_Impl::getRowsAffected()
	{
		return odbx_rows_affected( m_result );
	}



	unsigned long Result_Impl::getColumnCount()
	{
		return odbx_column_count( m_result );
	}



	string Result_Impl::getColumnName( unsigned long pos )
	{
		if( pos < odbx_column_count( m_result ) )
		{
			if( odbx_column_name( m_result, pos ) != NULL )
			{
				return string( odbx_column_name( m_result, pos ) );
			}

			return string();
		}

		throw( Exception( string( odbx_error( NULL, -ODBX_ERR_PARAM ) ), -ODBX_ERR_PARAM, odbx_error_type( NULL, -ODBX_ERR_PARAM ) ) );
	}



	odbxtype Result_Impl::getColumnType( unsigned long pos )
	{
		if( pos < odbx_column_count( m_result ) )
		{
			return (odbxtype) odbx_column_type( m_result, pos );
		}

		throw( Exception( string( odbx_error( NULL, -ODBX_ERR_PARAM ) ), -ODBX_ERR_PARAM, odbx_error_type( NULL, -ODBX_ERR_PARAM ) ) );
	}



	unsigned long Result_Impl::getFieldLength( unsigned long pos )
	{
		if( pos < odbx_column_count( m_result ) )
		{
			return odbx_field_length( m_result, pos );
		}

		throw( Exception( string( odbx_error( NULL, -ODBX_ERR_PARAM ) ), -ODBX_ERR_PARAM, odbx_error_type( NULL, -ODBX_ERR_PARAM ) ) );
	}



	const char* Result_Impl::getFieldValue( unsigned long pos )
	{
		if( pos < odbx_column_count( m_result ) )
		{
			return odbx_field_value( m_result, pos );
		}

		throw( Exception( string( odbx_error( NULL, -ODBX_ERR_PARAM ) ), -ODBX_ERR_PARAM, odbx_error_type( NULL, -ODBX_ERR_PARAM ) ) );
	}


	Lob_Impl* Result_Impl::getLob( const char* value )
	{
		return new Lob_Impl( m_result, value );
	}





	/*
	*  OpenDBX Stmt implementation
	*/



	Stmt_Impl::Stmt_Impl( odbx_t* handle )
	{
		m_handle = handle;
		m_escbuf = _resize( NULL, 32 );
		m_escsize = 32;
	}



	Stmt_Impl::~Stmt_Impl()
	{
		if( m_escbuf != NULL ) { std::free( m_escbuf ); }
	}



	Stmt_Impl* Stmt_Impl::instance( odbx_t* handle, Stmt::Type type, const string& sql )
	{
		switch( type )
		{
			case Stmt::Simple:
				return new StmtSimple_Impl( handle, sql );
			default:
				throw( Exception( string( odbx_error( NULL, -ODBX_ERR_PARAM ) ), -ODBX_ERR_PARAM, odbx_error_type( NULL, -ODBX_ERR_PARAM ) ) );
		}
	}



	string& Stmt_Impl::escape( const char* from, unsigned long fromlen, string& to )
	{
		int err;
		unsigned long size = m_escsize;

		while( fromlen * 2 + 1 > size ) { size = size * 2; }

		if( size > m_escsize )
		{
			m_escbuf = _resize( m_escbuf, size );
			m_escsize = size;
		}


		if( (err = odbx_escape( m_handle, from, fromlen, m_escbuf, &size ) ) < 0 )
		{
			throw( Exception( string( odbx_error( m_handle, err ) ), err, odbx_error_type( m_handle, err ) ) );
		}

		to.assign( m_escbuf, size );
		return to;
	}



	inline char* Stmt_Impl::_resize( char* buffer, size_t size )
	{
		if( ( buffer = (char*) std::realloc( buffer, size ) ) == NULL )
		{
			throw( Exception( string( odbx_error( m_handle, -ODBX_ERR_NOMEM ) ), -ODBX_ERR_NOMEM, odbx_error_type( m_handle, -ODBX_ERR_NOMEM ) ) );
		}

		return buffer;
	}





	/*
	*  OpenDBX StmtSimple implementation
	*/



	StmtSimple_Impl::StmtSimple_Impl( odbx_t* handle, const string& sql ) : Stmt_Impl( handle )
	{
		m_sql = sql;
		m_buffer = NULL;
		m_bufsize = 0;
/*		size_t pos = 0;

		while( ( pos = m_sql.find( "?", pos ) ) != string::npos )
		{
			m_pos.push_back( pos );
			pos += 1;
		}

		m_binds.resize( m_pos.size() );
		m_flags.resize( m_pos.size() );

		for( size_t i = 0; i < m_binds.size(); i++ )
		{
			m_binds[i] = NULL;
			m_flags[i] = Stmt::None;
		}*/
	}



	StmtSimple_Impl::StmtSimple_Impl() : Stmt_Impl( NULL )
	{
		m_buffer = NULL;
		m_bufsize = 0;
	}



	StmtSimple_Impl::~StmtSimple_Impl()
	{
		if( m_buffer != NULL ) { std::free( m_buffer ); }
	}



// 	void StmtSimple_Impl::bind( const void* data, unsigned long size, size_t pos, int flags )
// 	{
// 		if( pos >= m_pos.size() )
// 		{
// 			throw( Exception( string( odbx_error( NULL, -ODBX_ERR_PARAM ) ), -ODBX_ERR_PARAM, odbx_error_type( NULL, -ODBX_ERR_PARAM ) ) );
// 		}
//
// 		if( ( flags & Stmt::Null ) == 0 )
// 		{
// 			m_binds[pos] = data;
// 			m_bindsize[pos] = size;
// 		}
// 		m_flags[pos] = flags;
// 	}



// 	size_t StmtSimple_Impl::count()
// 	{
// 		return m_pos.size();
// 	}



	Result_Impl* StmtSimple_Impl::execute()
	{
// 		if( m_binds.size() ) { _exec_params(); }
// 		else { _exec_noparams(); }
		_exec_noparams();

		return new Result_Impl( m_handle );
	}



	inline void StmtSimple_Impl::_exec_noparams()
	{
		int err;

		if( ( err = odbx_query( m_handle, m_sql.c_str(), m_sql.size() ) ) < 0 )
		{
			throw( Exception( string( odbx_error( m_handle, err ) ), err, odbx_error_type( m_handle, err ) ) );
		}
	}



// 	inline void StmtSimple_Impl::_exec_params()
// 	{
// 		int err;
// 		unsigned long esclen;
// 		size_t i, sqlpos = 0, bufpos = 0, lastpos = 0, max = m_sql.size() + 1;
//
// 		for( i = 0; i < m_binds.size(); i++ )
// 		{
// 			if( m_binds[i] != NULL ) { max += m_bindsize[i] * 2 + 2; }
// 			else { max += 4; }
// 		}
// 		m_buffer = _resize( m_buffer, max );
//
// 		for( i = 0; i < m_binds.size(); i++ )
// 		{
// 			sqlpos = m_pos[i];
//
// 			memcpy( m_buffer + bufpos, m_sql.c_str() + lastpos, sqlpos - lastpos );
// 			bufpos += sqlpos - lastpos;
// 			lastpos = sqlpos + 1;
//
// 			if( m_binds[i] != NULL )
// 			{
// 				if( ( m_flags[i] & Stmt::Quote ) > 0 ) { m_buffer[bufpos++] = '\''; }
// 				esclen = max - bufpos;
//
// 				if( (err = odbx_escape( m_handle, (const char*) m_binds[i], m_bindsize[i], m_buffer + bufpos, &esclen ) ) < 0 )
// 				{
// 					throw( Exception( string( odbx_error( m_handle, err ) ), err, odbx_error_type( m_handle, err ) ) );
// 				}
//
// 				bufpos += esclen;
// 				if( ( m_flags[i] & Stmt::Quote ) != 0 ) { m_buffer[bufpos++] = '\''; }
// 			}
// 			else
// 			{
// 				memcpy( m_buffer + bufpos, "NULL", 4 );
// 				bufpos += 4;
// 			}
//
// 			sqlpos += 1;
// 		}
//
// 		memcpy( m_buffer + bufpos, m_sql.c_str() + lastpos, m_sql.size() - lastpos );
// 		bufpos += m_sql.size() - lastpos;
// 		m_buffer[bufpos] = 0;
//
// 		if( ( err = odbx_query( m_handle, m_buffer, bufpos ) ) < 0 )
// 		{
// 			throw( Exception( string( odbx_error( m_handle, err ) ), err, odbx_error_type( m_handle, err ) ) );
// 		}
// 	}





	/*
	*  OpenDBX connection implementation
	*/



	Conn_Impl::Conn_Impl( const char* backend, const char* host, const char* port )
	{
		int err;

		if( ( err =  odbx_init( &m_handle, backend, host, port ) ) < 0 )
		{
			throw( Exception( string( odbx_error( m_handle, err ) ), err, odbx_error_type( m_handle, err ) ) );
		}
	}



	Conn_Impl::~Conn_Impl()
	{
		odbx_finish( m_handle );
	}



	void Conn_Impl::bind( const char* database, const char* who, const char* cred, odbxbind method )
	{
		int err;

		if( ( err = odbx_bind( m_handle, database, who, cred, method ) ) < 0 )
		{
			throw( Exception( string( odbx_error( m_handle, err ) ), err, odbx_error_type( m_handle, err ) ) );
		}
	}



	void Conn_Impl::unbind()
	{
		int err;

		if( ( err = odbx_unbind( m_handle ) ) < 0 )
		{
			throw( Exception( string( odbx_error( m_handle, err ) ), err, odbx_error_type( m_handle, err ) ) );
		}
	}



	bool Conn_Impl::getCapability( odbxcap cap )
	{
		int err = odbx_capabilities( m_handle, (unsigned int) cap );

		switch( err )
		{
			case ODBX_ENABLE:
				return true;
			case ODBX_DISABLE:
				return false;
			default:
				throw( Exception( string( odbx_error( m_handle, err ) ), err, odbx_error_type( m_handle, err ) ) );
		}
	}



	void Conn_Impl::getOption( odbxopt option, void* value )
	{
		int err;

		if( ( err = odbx_get_option( m_handle, option, value ) ) < 0 )
		{
			throw( Exception( string( odbx_error( m_handle, err ) ), err, odbx_error_type( m_handle, err ) ) );
		}
	}



	void Conn_Impl::setOption( odbxopt option, void* value )
	{
		int err;

		if( ( err = odbx_set_option( m_handle, option, value ) ) < 0 )
		{
			throw( Exception( string( odbx_error( m_handle, err ) ), err, odbx_error_type( m_handle, err ) ) );
		}
	}



	Stmt_Impl* Conn_Impl::create( Stmt::Type type, const string& sql )
	{
		return Stmt_Impl::instance( m_handle, type, sql );
	}



}   // namespace
