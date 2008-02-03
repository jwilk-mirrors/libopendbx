/*
 *  OpenDBX - A simple but extensible database abstraction layer
 *  Copyright (C) 2007 Norbert Sendetzky and others
 *
 *  Distributed under the terms of the GNU Library General Public Licence
 * version 2 or (at your option) any later version.
 */



#include "odbxdrv.h"
#include "odbc_lo.h"



/*
 *  Declaration of odbc capabilities
 */

struct odbx_lo_ops odbc_odbx_lo_ops = {
	.open = odbc_odbx_lo_open,
	.close = odbc_odbx_lo_close,
// 	.length = odbc_odbx_lo_length,
	.read = odbc_odbx_lo_read,
// 	.write = odbc_odbx_lo_write,
};



/*
 *  OpenDBX large object operations
 *  ODBC style
 */


static int odbc_odbx_lo_open( odbx_result_t* result, odbx_lo_t** lo, const char* value )
{
	return -ODBX_ERR_NOTSUP;
}



static int odbc_odbx_lo_close( odbx_lo_t* lo )
{
	return -ODBX_ERR_NOTSUP;
}


/*
static uint64_t odbc_odbx_lo_length( odbx_lo_t* lo )
{
	return -ODBX_ERR_NOTSUP;
}
*/


static ssize_t odbc_odbx_lo_read( odbx_lo_t* lo, void* buffer, size_t buflen )
{
	return -ODBX_ERR_NOTSUP;
}


/*
static ssize_t odbc_odbx_lo_write( odbx_lo_t* lo, void* buffer, size_t buflen )
{
	return -ODBX_ERR_NOTSUP;
}
*/
