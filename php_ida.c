/*
    +--------------------------------------------------------------------+
    | Interfaces for Database Access                                     |
    +--------------------------------------------------------------------+
    | Redistribution and use in source and binary forms, with or without |
    | modification, are permitted provided that the conditions mentioned |
    | in the accompanying LICENSE file are met.                          |
    +--------------------------------------------------------------------+
    | Copyright (c) 2013, Michael Wallner <mike@php.net>                |
    +--------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "Zend/zend_interfaces.h"
#include "ext/spl/spl_iterators.h"
#include "php_ida.h"

/* {{{ IDA\Connection */

/**
 * Database connection
 */
zend_class_entry *php_ida_Connection_ce;

/**
 * Create a new Connection
 * @param mixed $dsn
 * @param int $flags
 */
ZEND_BEGIN_ARG_INFO_EX(ai_Connection___construct, 0, 0, 0)
	ZEND_ARG_INFO(0, dsn) 
	ZEND_ARG_INFO(0, flags) 
ZEND_END_ARG_INFO();

/**
 * Reset the connection
 * @return void
 */
ZEND_BEGIN_ARG_INFO_EX(ai_Connection_reset, 0, 0, 0)
ZEND_END_ARG_INFO();

/**
 * Retrieve the connection status
 * @return int
 */
ZEND_BEGIN_ARG_INFO_EX(ai_Connection_getStatus, 0, 0, 0)
ZEND_END_ARG_INFO();

/**
 * Execute query
 * @return IDA\Result
 * @param string $query
 */
ZEND_BEGIN_ARG_INFO_EX(ai_Connection_exec, 0, 0, 1)
	ZEND_ARG_INFO(0, query) 
ZEND_END_ARG_INFO();

/**
 * Execute a one-shot (emulated/non-server) prepared query
 * @return IDA\Result
 * @param string $query
 * @param array $params
 * @param array $types
 */
ZEND_BEGIN_ARG_INFO_EX(ai_Connection_execParams, 0, 0, 2)
	ZEND_ARG_INFO(0, query) 
	ZEND_ARG_ARRAY_INFO(0, params, 0)
	ZEND_ARG_ARRAY_INFO(0, types, 1)
ZEND_END_ARG_INFO();

/**
 * Prepare query
 * @return IDA\Statement
 * @param string $name
 * @param string $query
 * @param array $types
 */
ZEND_BEGIN_ARG_INFO_EX(ai_Connection_prepare, 0, 0, 2)
	ZEND_ARG_INFO(0, name) 
	ZEND_ARG_INFO(0, query) 
	ZEND_ARG_ARRAY_INFO(0, types, 1)
ZEND_END_ARG_INFO();

/**
 * Quote a string for safe use in a query
 * @return string
 * @param string $string
 */
ZEND_BEGIN_ARG_INFO_EX(ai_Connection_quote, 0, 0, 1)
	ZEND_ARG_INFO(0, data) 
ZEND_END_ARG_INFO();

/**
 * Quote an identifier
 * @return string
 * @param string $name
 */
ZEND_BEGIN_ARG_INFO_EX(ai_Connection_quoteName, 0, 0, 1)
	ZEND_ARG_INFO(0, name) 
ZEND_END_ARG_INFO();

/**
 * Escape binary data
 * @return string
 * @param string $data
 */
ZEND_BEGIN_ARG_INFO_EX(ai_Connection_escapeBinary, 0, 0, 1)
	ZEND_ARG_INFO(0, data) 
ZEND_END_ARG_INFO();

/**
  * Unescape binary data
  * @return string
  * @param string $data
  */
ZEND_BEGIN_ARG_INFO_EX(ai_Connection_unsescapeBinary, 0, 0, 1)
	ZEND_ARG_INFO(0, data) 
ZEND_END_ARG_INFO();


static zend_function_entry php_ida_Connection_me[] = {
	/**
	 * Create a new Connection
	 * @param mixed $dsn
	 * @param int $flags
	 */
	PHP_ABSTRACT_ME(Connection, __construct, ai_Connection___construct)

	/**
	 * Reset the connection
	 * @return void
	 */
	PHP_ABSTRACT_ME(Connection, reset, ai_Connection_reset)

	/**
	 * Retrieve the connection status
	 * @return int
	 */
	PHP_ABSTRACT_ME(Connection, getStatus, ai_Connection_getStatus)

	/**
	 * Execute query
	 * @return IDA\Result
	 * @param string $query
	 */
	PHP_ABSTRACT_ME(Connection, exec, ai_Connection_exec)

	/**
	 * Execute a one-shot (emulated/non-server) prepared query
	 * @return IDA\Result
	 * @param string $query
	 * @param array $params
	 * @param array $types
	 */
	PHP_ABSTRACT_ME(Connection, execParams, ai_Connection_execParams)

	/**
	 * Prepare query
	 * @return IDA\Statement
	 * @param string $name
	 * @param string $query
	 * @param array $types
	 */
	PHP_ABSTRACT_ME(Connection, prepare, ai_Connection_prepare)

	/**
	 * Quote a string for safe use in a query
	 * @return string
	 * @param string $string
	 */
	PHP_ABSTRACT_ME(Connection, quote, ai_Connection_quote)

	/**
	 * Quote an identifier
	 * @return string
	 * @param string $name
	 */
	PHP_ABSTRACT_ME(Connection, quoteName, ai_Connection_quoteName)

	/**
	 * Escape binary data
	 * @return string
	 * @param string $data
	 */
	PHP_ABSTRACT_ME(Connection, escapeBinary, ai_Connection_escapeBinary)

	/**
	  * Unescape binary data
	  * @return string
	  * @param string $data
	  */
	PHP_ABSTRACT_ME(Connection, unsescapeBinary, ai_Connection_unsescapeBinary)

	{NULL}
};

/* }}} */

/* {{{ IDA\Statement */

/**
 * A prepared statement
 */
zend_class_entry *php_ida_Statement_ce;

/**
 * Get the associated connection
 * @return IDA\Connection
 */
ZEND_BEGIN_ARG_INFO_EX(ai_Statement_getConnection, 0, 0, 0)
ZEND_END_ARG_INFO();

/**
 * Retrieve the name of the statement
 * @return string
 */
ZEND_BEGIN_ARG_INFO_EX(ai_Statement_getName, 0, 0, 0)
ZEND_END_ARG_INFO();

/**
 * Bind a parameter to a variable
 * @param int $param
 * @param mixed $ref
 */
ZEND_BEGIN_ARG_INFO_EX(ai_Statement_bind, 0, 0, 2)
	ZEND_ARG_INFO(0, param) 
	ZEND_ARG_INFO(1, ref) 
ZEND_END_ARG_INFO();

/**
 * Execute the statement, optionally with params $params
 * @return IDA\Result
 * @param array $params
 */
ZEND_BEGIN_ARG_INFO_EX(ai_Statement_exec, 0, 0, 0)
	ZEND_ARG_ARRAY_INFO(0, params, 1)
ZEND_END_ARG_INFO();


static zend_function_entry php_ida_Statement_me[] = {
	/**
	 * Get the associated connection
	 * @return IDA\Connection
	 */
	PHP_ABSTRACT_ME(Statement, getConnection, ai_Statement_getConnection)

	/**
	 * Retrieve the name of the statement
	 * @return string
	 */
	PHP_ABSTRACT_ME(Statement, getName, ai_Statement_getName)

	/**
	 * Bind a parameter to a variable
	 * @param int $param
	 * @param mixed $ref
	 */
	PHP_ABSTRACT_ME(Statement, bind, ai_Statement_bind)

	/**
	 * Execute the statement, optionally with params $params
	 * @return IDA\Result
	 * @param array $params
	 */
	PHP_ABSTRACT_ME(Statement, exec, ai_Statement_exec)

	{NULL}
};

/* }}} */

/* {{{ IDA\Result */

/**
 * A query result (command success, result set, ...)
 */
zend_class_entry *php_ida_Result_ce;

/**
 * Retreive the number of found rows
 * @return int
 */
ZEND_BEGIN_ARG_INFO_EX(ai_Result_numRows, 0, 0, 0)
ZEND_END_ARG_INFO();

/**
 * Retrieve the number of columns in the result
 * @return int
 */
ZEND_BEGIN_ARG_INFO_EX(ai_Result_numCols, 0, 0, 0)
ZEND_END_ARG_INFO();

/**
 * Retrieve the number of affected rows
 * @return int
 */
ZEND_BEGIN_ARG_INFO_EX(ai_Result_affectedRows, 0, 0, 0)
ZEND_END_ARG_INFO();

/**
 * Get the currently set fetch type
 * @return int
 */
ZEND_BEGIN_ARG_INFO_EX(ai_Result_getFetchType, 0, 0, 0)
ZEND_END_ARG_INFO();

/**
 * Set the fetch typoe for this result
 * @return void
 * @param int $fetch_type
 */
ZEND_BEGIN_ARG_INFO_EX(ai_Result_setFetchType, 0, 0, 0)
	ZEND_ARG_INFO(0, fetch_type) 
ZEND_END_ARG_INFO();

/**
 * Retrieve any error code
 * @return int
 */
ZEND_BEGIN_ARG_INFO_EX(ai_Result_getError, 0, 0, 0)
ZEND_END_ARG_INFO();

/**
 * Retrieve an error message
 * @return string
 */
ZEND_BEGIN_ARG_INFO_EX(ai_Result_getErrorMessage, 0, 0, 0)
ZEND_END_ARG_INFO();

/**
 * Retrieve the status code
 * @return int
 */
ZEND_BEGIN_ARG_INFO_EX(ai_Result_getStatus, 0, 0, 0)
ZEND_END_ARG_INFO();

/**
 * Retrieve a static status message for the current status code
 * @return string
 */
ZEND_BEGIN_ARG_INFO_EX(ai_Result_getStatusMessage, 0, 0, 0)
ZEND_END_ARG_INFO();

/**
 * Bind column to a variable
 * @return bool
 * @param int $column
 * @param mixed $ref
 */
ZEND_BEGIN_ARG_INFO_EX(ai_Result_bind, 0, 0, 2)
	ZEND_ARG_INFO(0, column) 
	ZEND_ARG_INFO(1, ref) 
ZEND_END_ARG_INFO();

/**
 * Fetch the next row, assigning the columns to the bound variables
 * @return mixed result row
 */
ZEND_BEGIN_ARG_INFO_EX(ai_Result_fetchBound, 0, 0, 0)
ZEND_END_ARG_INFO();

/**
 * Fetch next row
 * @return mixed result row
 * @param int $fetch_type
 */
ZEND_BEGIN_ARG_INFO_EX(ai_Result_fetchRow, 0, 0, 0)
	ZEND_ARG_INFO(0, fetch_type) 
ZEND_END_ARG_INFO();

/**
 * Fetch a single column from next row
 * @return bool
 * @param int $column
 * @param mixed $ref
 */
ZEND_BEGIN_ARG_INFO_EX(ai_Result_fetchCol, 0, 0, 2)
	ZEND_ARG_INFO(0, column) 
	ZEND_ARG_INFO(1, ref) 
ZEND_END_ARG_INFO();

/**
 * Fetch the complete result set as a simple map, a multi dimensional 
 * array, each dimension indexed by a column and all columns or 
 * $val_coulmns building the leaves.
 * @return array
 * @param array $key_columns
 * @param array $val_columns
 * @param array $fetch_type
 */
ZEND_BEGIN_ARG_INFO_EX(ai_Result_map, 0, 0, 0)
	ZEND_ARG_INFO(0, key_columns) 
	ZEND_ARG_INFO(0, val_columns) 
	ZEND_ARG_INFO(0, fetch_type) 
ZEND_END_ARG_INFO();


static zend_function_entry php_ida_Result_me[] = {
	/**
	 * Retreive the number of found rows
	 * @return int
	 */
	PHP_ABSTRACT_ME(Result, numRows, ai_Result_numRows)

	/**
	 * Retrieve the number of columns in the result
	 * @return int
	 */
	PHP_ABSTRACT_ME(Result, numCols, ai_Result_numCols)

	/**
	 * Retrieve the number of affected rows
	 * @return int
	 */
	PHP_ABSTRACT_ME(Result, affectedRows, ai_Result_affectedRows)

	/**
	 * Get the currently set fetch type
	 * @return int
	 */
	PHP_ABSTRACT_ME(Result, getFetchType, ai_Result_getFetchType)

	/**
	 * Set the fetch typoe for this result
	 * @return void
	 * @param int $fetch_type
	 */
	PHP_ABSTRACT_ME(Result, setFetchType, ai_Result_setFetchType)

	/**
	 * Retrieve any error code
	 * @return int
	 */
	PHP_ABSTRACT_ME(Result, getError, ai_Result_getError)

	/**
	 * Retrieve an error message
	 * @return string
	 */
	PHP_ABSTRACT_ME(Result, getErrorMessage, ai_Result_getErrorMessage)

	/**
	 * Retrieve the status code
	 * @return int
	 */
	PHP_ABSTRACT_ME(Result, getStatus, ai_Result_getStatus)

	/**
	 * Retrieve a static status message for the current status code
	 * @return string
	 */
	PHP_ABSTRACT_ME(Result, getStatusMessage, ai_Result_getStatusMessage)

	/**
	 * Bind column to a variable
	 * @return bool
	 * @param int $column
	 * @param mixed $ref
	 */
	PHP_ABSTRACT_ME(Result, bind, ai_Result_bind)

	/**
	 * Fetch the next row, assigning the columns to the bound variables
	 * @return mixed result row
	 */
	PHP_ABSTRACT_ME(Result, fetchBound, ai_Result_fetchBound)

	/**
	 * Fetch next row
	 * @return mixed result row
	 * @param int $fetch_type
	 */
	PHP_ABSTRACT_ME(Result, fetchRow, ai_Result_fetchRow)

	/**
	 * Fetch a single column from next row
	 * @return bool
	 * @param int $column
	 * @param mixed $ref
	 */
	PHP_ABSTRACT_ME(Result, fetchCol, ai_Result_fetchCol)

	/**
	 * Fetch the complete result set as a simple map, a multi dimensional 
	 * array, each dimension indexed by a column and all columns or 
	 * $val_coulmns building the leaves.
	 * @return array
	 * @param array $key_columns
	 * @param array $val_columns
	 * @param array $fetch_type
	 */
	PHP_ABSTRACT_ME(Result, map, ai_Result_map)

	{NULL}
};

/* }}} */

/* {{{ IDA\Transaction */

/**
 * A database transaction.
 */
zend_class_entry *php_ida_Transaction_ce;

/**
 * Retrieve the associated connection
 * @return IDA\Connection
 */
ZEND_BEGIN_ARG_INFO_EX(ai_Transaction_getConnection, 0, 0, 0)
ZEND_END_ARG_INFO();

/**
 * Retrieve the current isolation level of the transaction
 * @return int
 */
ZEND_BEGIN_ARG_INFO_EX(ai_Transaction_getIsolation, 0, 0, 0)
ZEND_END_ARG_INFO();

/**
 * Set the isolation level of the transaction
 * @return void
 * @param int $isolation
 */
ZEND_BEGIN_ARG_INFO_EX(ai_Transaction_setIsolation, 0, 0, 0)
	ZEND_ARG_INFO(0, isolation) 
ZEND_END_ARG_INFO();

/**
 * Retrieve the read_only flag of the transaction
 * @return bool
 */
ZEND_BEGIN_ARG_INFO_EX(ai_Transaction_getReadOnly, 0, 0, 0)
ZEND_END_ARG_INFO();

/**
 * Set the read_only flags of the transaction
 * @return void
 * @param bool $read_only
 */
ZEND_BEGIN_ARG_INFO_EX(ai_Transaction_setReadOnly, 0, 0, 0)
	ZEND_ARG_INFO(0, read_only) 
ZEND_END_ARG_INFO();

/**
 * Commit this transaction
 * @return void
 */
ZEND_BEGIN_ARG_INFO_EX(ai_Transaction_commit, 0, 0, 0)
ZEND_END_ARG_INFO();

/**
 * Rollback this transaction
 * @return void
 */
ZEND_BEGIN_ARG_INFO_EX(ai_Transaction_rollback, 0, 0, 0)
ZEND_END_ARG_INFO();


static zend_function_entry php_ida_Transaction_me[] = {
	/**
	 * Retrieve the associated connection
	 * @return IDA\Connection
	 */
	PHP_ABSTRACT_ME(Transaction, getConnection, ai_Transaction_getConnection)

	/**
	 * Retrieve the current isolation level of the transaction
	 * @return int
	 */
	PHP_ABSTRACT_ME(Transaction, getIsolation, ai_Transaction_getIsolation)

	/**
	 * Set the isolation level of the transaction
	 * @return void
	 * @param int $isolation
	 */
	PHP_ABSTRACT_ME(Transaction, setIsolation, ai_Transaction_setIsolation)

	/**
	 * Retrieve the read_only flag of the transaction
	 * @return bool
	 */
	PHP_ABSTRACT_ME(Transaction, getReadOnly, ai_Transaction_getReadOnly)

	/**
	 * Set the read_only flags of the transaction
	 * @return void
	 * @param bool $read_only
	 */
	PHP_ABSTRACT_ME(Transaction, setReadOnly, ai_Transaction_setReadOnly)

	/**
	 * Commit this transaction
	 * @return void
	 */
	PHP_ABSTRACT_ME(Transaction, commit, ai_Transaction_commit)

	/**
	 * Rollback this transaction
	 * @return void
	 */
	PHP_ABSTRACT_ME(Transaction, rollback, ai_Transaction_rollback)

	{NULL}
};

/* }}} */

/* {{{ IDA\LOB */

/**
 * A large object.
 */
zend_class_entry *php_ida_LOB_ce;

/**
 * Retrieve the associated transaction
 * @return IDA\Transation
 */
ZEND_BEGIN_ARG_INFO_EX(ai_LOB_getTransaction, 0, 0, 0)
ZEND_END_ARG_INFO();

/**
 * Retrieve the LOB identifier
 * @return mixed
 */
ZEND_BEGIN_ARG_INFO_EX(ai_LOB_getId, 0, 0, 0)
ZEND_END_ARG_INFO();

/**
 * Retrieve a PHP stream wrapper
 * @return resource
 */
ZEND_BEGIN_ARG_INFO_EX(ai_LOB_getStream, 0, 0, 0)
ZEND_END_ARG_INFO();

/**
 * Read from the LOB
 * @return string
 * @param int $length
 * @param int $read actually read length
 */
ZEND_BEGIN_ARG_INFO_EX(ai_LOB_read, 0, 0, 1)
	ZEND_ARG_INFO(0, length) 
	ZEND_ARG_INFO(1, read) 
ZEND_END_ARG_INFO();

/**
 * Write to the LOB
 * @return int
 * @param string $data
 */
ZEND_BEGIN_ARG_INFO_EX(ai_LOB_write, 0, 0, 1)
	ZEND_ARG_INFO(0, data) 
ZEND_END_ARG_INFO();

/**
 * Seek to a position within the LOB
 * @return int
 * @param int $offset
 * @param int $whence SEEK_* constant
 */
ZEND_BEGIN_ARG_INFO_EX(ai_LOB_seek, 0, 0, 1)
	ZEND_ARG_INFO(0, offset) 
	ZEND_ARG_INFO(0, whence) 
ZEND_END_ARG_INFO();

/**
 * Tell the current position within the LOB
 * @return int
 */
ZEND_BEGIN_ARG_INFO_EX(ai_LOB_tell, 0, 0, 0)
ZEND_END_ARG_INFO();

/**
 * Truncate the LOB
 * @return void
 * @param int $length
 */
ZEND_BEGIN_ARG_INFO_EX(ai_LOB_truncate, 0, 0, 0)
	ZEND_ARG_INFO(0, length) 
ZEND_END_ARG_INFO();


static zend_function_entry php_ida_LOB_me[] = {
	/**
	 * Retrieve the associated transaction
	 * @return IDA\Transation
	 */
	PHP_ABSTRACT_ME(LOB, getTransaction, ai_LOB_getTransaction)

	/**
	 * Retrieve the LOB identifier
	 * @return mixed
	 */
	PHP_ABSTRACT_ME(LOB, getId, ai_LOB_getId)

	/**
	 * Retrieve a PHP stream wrapper
	 * @return resource
	 */
	PHP_ABSTRACT_ME(LOB, getStream, ai_LOB_getStream)

	/**
	 * Read from the LOB
	 * @return string
	 * @param int $length
	 * @param int $read actually read length
	 */
	PHP_ABSTRACT_ME(LOB, read, ai_LOB_read)

	/**
	 * Write to the LOB
	 * @return int
	 * @param string $data
	 */
	PHP_ABSTRACT_ME(LOB, write, ai_LOB_write)

	/**
	 * Seek to a position within the LOB
	 * @return int
	 * @param int $offset
	 * @param int $whence SEEK_* constant
	 */
	PHP_ABSTRACT_ME(LOB, seek, ai_LOB_seek)

	/**
	 * Tell the current position within the LOB
	 * @return int
	 */
	PHP_ABSTRACT_ME(LOB, tell, ai_LOB_tell)

	/**
	 * Truncate the LOB
	 * @return void
	 * @param int $length
	 */
	PHP_ABSTRACT_ME(LOB, truncate, ai_LOB_truncate)

	{NULL}
};

/* }}} */

/* {{{ IDA\Feature\PersistentConnection */

/**
 * Indicates that the implementation supports persistent connections.
 */
zend_class_entry *php_ida_PersistentConnection_ce;

/**
 * Determine whether this is a persistent connection.
 * @return bool
 */
ZEND_BEGIN_ARG_INFO_EX(ai_PersistentConnection_isPersistent, 0, 0, 0)
ZEND_END_ARG_INFO();


static zend_function_entry php_ida_PersistentConnection_me[] = {
	/**
	 * Determine whether this is a persistent connection.
	 * @return bool
	 */
	PHP_ABSTRACT_ME(PersistentConnection, isPersistent, ai_PersistentConnection_isPersistent)

	{NULL}
};

/* }}} */

/* {{{ IDA\Feature\AsyncConnection */

/**
 * Indicates that the implementation supports asynchronous connections.
 */
zend_class_entry *php_ida_AsyncConnection_ce;

/**
 * Determine state of the underlying resource.
 * @return int self::POLLING_* constant
 */
ZEND_BEGIN_ARG_INFO_EX(ai_AsyncConnection_poll, 0, 0, 0)
ZEND_END_ARG_INFO();

/**
 * Retrieve the underlying resource for e.g. use with select()
 * @return resource
 */
ZEND_BEGIN_ARG_INFO_EX(ai_AsyncConnection_getResource, 0, 0, 0)
ZEND_END_ARG_INFO();

/**
 * Retrieve the connection status
 * @return int
 */
ZEND_BEGIN_ARG_INFO_EX(ai_AsyncConnection_getStatus, 0, 0, 0)
ZEND_END_ARG_INFO();

/**
 * Retrieve the last query result
 * @return IDA\Result
 */
ZEND_BEGIN_ARG_INFO_EX(ai_AsyncConnection_getResult, 0, 0, 0)
ZEND_END_ARG_INFO();

/**
 * Execute query asynchronously
 * @return void
 * @param string $query
 */
ZEND_BEGIN_ARG_INFO_EX(ai_AsyncConnection_execAsync, 0, 0, 1)
	ZEND_ARG_INFO(0, query) 
	ZEND_ARG_INFO(0, cb)  /* callable */ 
ZEND_END_ARG_INFO();

/**
 * Execute a one-shot (emulated/non-server) prepared query asynchronously
 * @return void
 * @param string $query
 * @param array $params
 * @param array $types
 * @param callable $cb
 */
ZEND_BEGIN_ARG_INFO_EX(ai_AsyncConnection_execParamsAsync, 0, 0, 2)
	ZEND_ARG_INFO(0, query) 
	ZEND_ARG_ARRAY_INFO(0, params, 0)
	ZEND_ARG_ARRAY_INFO(0, types, 1)
	ZEND_ARG_INFO(0, cb)  /* callable */ 
ZEND_END_ARG_INFO();

/**
 * Prepare query asynchronously
 * @return IDA\Statement
 * @param string $name
 * @param string $query
 * @param array $types
 */
ZEND_BEGIN_ARG_INFO_EX(ai_AsyncConnection_prepareAsync, 0, 0, 2)
	ZEND_ARG_INFO(0, name) 
	ZEND_ARG_INFO(0, query) 
	ZEND_ARG_ARRAY_INFO(0, types, 1)
ZEND_END_ARG_INFO();

/**
 * Reset the connection asynchronously
 * @return void
 */
ZEND_BEGIN_ARG_INFO_EX(ai_AsyncConnection_resetAsync, 0, 0, 0)
ZEND_END_ARG_INFO();

/**
 * Do whathever is neccessary to complete the current asynchronous
 * operation, i.e. synchronize with caller. Escape path for code 
 * unintentionally confronted with an asynchronous connection:
 * <code>
 * <?php
 * switch ($connection->getStatus()) {
 * case IDA\Connection::OK:
 *     break;
 * case IDA\Connection::BAD:
 *     $connection->reset();
 *     break;
 * default:
 *     $connection->waitForCompletion();
 *     break;
 * }
 * </code>
 * 
 * @return void
 */
ZEND_BEGIN_ARG_INFO_EX(ai_AsyncConnection_waitForCompletion, 0, 0, 0)
ZEND_END_ARG_INFO();


static zend_function_entry php_ida_AsyncConnection_me[] = {
	/**
	 * Determine state of the underlying resource.
	 * @return int self::POLLING_* constant
	 */
	PHP_ABSTRACT_ME(AsyncConnection, poll, ai_AsyncConnection_poll)

	/**
	 * Retrieve the underlying resource for e.g. use with select()
	 * @return resource
	 */
	PHP_ABSTRACT_ME(AsyncConnection, getResource, ai_AsyncConnection_getResource)

	/**
	 * Retrieve the connection status
	 * @return int
	 */
	PHP_ABSTRACT_ME(AsyncConnection, getStatus, ai_AsyncConnection_getStatus)

	/**
	 * Retrieve the last query result
	 * @return IDA\Result
	 */
	PHP_ABSTRACT_ME(AsyncConnection, getResult, ai_AsyncConnection_getResult)

	/**
	 * Execute query asynchronously
	 * @return void
	 * @param string $query
	 */
	PHP_ABSTRACT_ME(AsyncConnection, execAsync, ai_AsyncConnection_execAsync)

	/**
	 * Execute a one-shot (emulated/non-server) prepared query asynchronously
	 * @return void
	 * @param string $query
	 * @param array $params
	 * @param array $types
	 * @param callable $cb
	 */
	PHP_ABSTRACT_ME(AsyncConnection, execParamsAsync, ai_AsyncConnection_execParamsAsync)

	/**
	 * Prepare query asynchronously
	 * @return IDA\Statement
	 * @param string $name
	 * @param string $query
	 * @param array $types
	 */
	PHP_ABSTRACT_ME(AsyncConnection, prepareAsync, ai_AsyncConnection_prepareAsync)

	/**
	 * Reset the connection asynchronously
	 * @return void
	 */
	PHP_ABSTRACT_ME(AsyncConnection, resetAsync, ai_AsyncConnection_resetAsync)

	/**
	 * Do whathever is neccessary to complete the current asynchronous
	 * operation, i.e. synchronize with caller. Escape path for code 
	 * unintentionally confronted with an asynchronous connection:
	 * <code>
	 * <?php
	 * switch ($connection->getStatus()) {
	 * case IDA\Connection::OK:
	 *     break;
	 * case IDA\Connection::BAD:
	 *     $connection->reset();
	 *     break;
	 * default:
	 *     $connection->waitForCompletion();
	 *     break;
	 * }
	 * </code>
	 * 
	 * @return void
	 */
	PHP_ABSTRACT_ME(AsyncConnection, waitForCompletion, ai_AsyncConnection_waitForCompletion)

	{NULL}
};

/* }}} */

/* {{{ IDA\Feature\TransactingConnection */

/**
 * Indicates that the implementation supports transactions.
 */
zend_class_entry *php_ida_TransactingConnection_ce;

/**
 * Start a transaction
 * @return IDA\Transaction
 * @param int $isolation
 * @param bool $readonly
 */
ZEND_BEGIN_ARG_INFO_EX(ai_TransactingConnection_startTransaction, 0, 0, 0)
	ZEND_ARG_INFO(0, isolation) 
	ZEND_ARG_INFO(0, readonly) 
ZEND_END_ARG_INFO();


static zend_function_entry php_ida_TransactingConnection_me[] = {
	/**
	 * Start a transaction
	 * @return IDA\Transaction
	 * @param int $isolation
	 * @param bool $readonly
	 */
	PHP_ABSTRACT_ME(TransactingConnection, startTransaction, ai_TransactingConnection_startTransaction)

	{NULL}
};

/* }}} */

/* {{{ IDA\Feature\AsyncTransactingConnection */

/**
 * Indicates that the connection implementation can start transactions asynchronously.
 */
zend_class_entry *php_ida_AsyncTransactingConnection_ce;

/**
 * Start a transaction asynchronously
 * @return IDA\Transaction
 * @param int $isolation
 * @param bool $readonly
 */
ZEND_BEGIN_ARG_INFO_EX(ai_AsyncTransactingConnection_startTransactionAsync, 0, 0, 0)
	ZEND_ARG_INFO(0, isolation) 
	ZEND_ARG_INFO(0, readonly) 
ZEND_END_ARG_INFO();


static zend_function_entry php_ida_AsyncTransactingConnection_me[] = {
	/**
	 * Start a transaction asynchronously
	 * @return IDA\Transaction
	 * @param int $isolation
	 * @param bool $readonly
	 */
	PHP_ABSTRACT_ME(AsyncTransactingConnection, startTransactionAsync, ai_AsyncTransactingConnection_startTransactionAsync)

	{NULL}
};

/* }}} */

/* {{{ IDA\Feature\AsyncTransaction */

/**
 * Indicates that the transaction implementation supports asynchronous commit/rollback.
 */
zend_class_entry *php_ida_AsyncTransaction_ce;

/**
 * Commit the transaction asynchronously
 * @return void
 */
ZEND_BEGIN_ARG_INFO_EX(ai_AsyncTransaction_commitAsync, 0, 0, 0)
ZEND_END_ARG_INFO();

/**
 * Rollback the transaction asynchronously
 * @return void
 */
ZEND_BEGIN_ARG_INFO_EX(ai_AsyncTransaction_rollbackAsync, 0, 0, 0)
ZEND_END_ARG_INFO();


static zend_function_entry php_ida_AsyncTransaction_me[] = {
	/**
	 * Commit the transaction asynchronously
	 * @return void
	 */
	PHP_ABSTRACT_ME(AsyncTransaction, commitAsync, ai_AsyncTransaction_commitAsync)

	/**
	 * Rollback the transaction asynchronously
	 * @return void
	 */
	PHP_ABSTRACT_ME(AsyncTransaction, rollbackAsync, ai_AsyncTransaction_rollbackAsync)

	{NULL}
};

/* }}} */

/* {{{ IDA\Feature\SavepointTransaction */

/**
 * Indicates that the transaction implementation supports savepoints.
 */
zend_class_entry *php_ida_SavepointTransaction_ce;

/**
 * Create a new savepoint which can be committed or rolled back.
 * @return void
 */
ZEND_BEGIN_ARG_INFO_EX(ai_SavepointTransaction_savepoint, 0, 0, 0)
ZEND_END_ARG_INFO();


static zend_function_entry php_ida_SavepointTransaction_me[] = {
	/**
	 * Create a new savepoint which can be committed or rolled back.
	 * @return void
	 */
	PHP_ABSTRACT_ME(SavepointTransaction, savepoint, ai_SavepointTransaction_savepoint)

	{NULL}
};

/* }}} */

/* {{{ IDA\Feature\AsyncSavepointTransaction */

/**
 * Indicates that the transaction implementation supports creating savepoints asynchronously.
 */
zend_class_entry *php_ida_AsyncSavepointTransaction_ce;

/**
 * Create a savepoint, asynchronously
 * @return void
 */
ZEND_BEGIN_ARG_INFO_EX(ai_AsyncSavepointTransaction_savepointAsync, 0, 0, 0)
ZEND_END_ARG_INFO();


static zend_function_entry php_ida_AsyncSavepointTransaction_me[] = {
	/**
	 * Create a savepoint, asynchronously
	 * @return void
	 */
	PHP_ABSTRACT_ME(AsyncSavepointTransaction, savepointAsync, ai_AsyncSavepointTransaction_savepointAsync)

	{NULL}
};

/* }}} */

/* {{{ IDA\Feature\LOBTransaction */

/**
 * Indicates that the transaction implementation supports LOB operations.
 */
zend_class_entry *php_ida_LOBTransaction_ce;

/**
 * Open a LOB
 * @return IDA\LOB
 * @param mixed $id
 * @param int $mode IDA\LOB constants
 */
ZEND_BEGIN_ARG_INFO_EX(ai_LOBTransaction_openLOB, 0, 0, 1)
	ZEND_ARG_INFO(0, id) 
	ZEND_ARG_INFO(0, mode) 
ZEND_END_ARG_INFO();

/**
 * Create a LOB
 * @return IDA\LOB
 * @param int $mode
 */
ZEND_BEGIN_ARG_INFO_EX(ai_LOBTransaction_createLOB, 0, 0, 0)
	ZEND_ARG_INFO(0, mode) 
ZEND_END_ARG_INFO();

/**
 * Delete a LOB
 * @param mixed $id
 */
ZEND_BEGIN_ARG_INFO_EX(ai_LOBTransaction_unlinkLOB, 0, 0, 1)
	ZEND_ARG_INFO(0, id) 
ZEND_END_ARG_INFO();


static zend_function_entry php_ida_LOBTransaction_me[] = {
	/**
	 * Open a LOB
	 * @return IDA\LOB
	 * @param mixed $id
	 * @param int $mode IDA\LOB constants
	 */
	PHP_ABSTRACT_ME(LOBTransaction, openLOB, ai_LOBTransaction_openLOB)

	/**
	 * Create a LOB
	 * @return IDA\LOB
	 * @param int $mode
	 */
	PHP_ABSTRACT_ME(LOBTransaction, createLOB, ai_LOBTransaction_createLOB)

	/**
	 * Delete a LOB
	 * @param mixed $id
	 */
	PHP_ABSTRACT_ME(LOBTransaction, unlinkLOB, ai_LOBTransaction_unlinkLOB)

	{NULL}
};

/* }}} */

/* {{{ IDA\Feature\AsyncStatement */

/**
 * Indicates that the statement implementation supports asynchronous execution.
 */
zend_class_entry *php_ida_AsyncStatement_ce;

/**
 * Execute the statement asynchronously
 * @return void
 * @param array $params
 * @param callable $cb
 */
ZEND_BEGIN_ARG_INFO_EX(ai_AsyncStatement_execAsync, 0, 0, 0)
	ZEND_ARG_ARRAY_INFO(0, params, 1)
	ZEND_ARG_INFO(0, cb)  /* callable */ 
ZEND_END_ARG_INFO();


static zend_function_entry php_ida_AsyncStatement_me[] = {
	/**
	 * Execute the statement asynchronously
	 * @return void
	 * @param array $params
	 * @param callable $cb
	 */
	PHP_ABSTRACT_ME(AsyncStatement, execAsync, ai_AsyncStatement_execAsync)

	{NULL}
};

/* }}} */


PHP_MINIT_FUNCTION(ida)
{
	zend_class_entry ce;

	/**
	 * Database connection
	 */
	memset(&ce, 0, sizeof(ce));
	INIT_NS_CLASS_ENTRY(ce, "IDA", "Connection", php_ida_Connection_me);
	php_ida_Connection_ce = zend_register_internal_interface(&ce TSRMLS_CC);

	zend_declare_class_constant_long(php_ida_Connection_ce, ZEND_STRL("OK"), 0 TSRMLS_CC);
	zend_declare_class_constant_long(php_ida_Connection_ce, ZEND_STRL("BAD"), 1 TSRMLS_CC);
	zend_declare_class_constant_long(php_ida_Connection_ce, ZEND_STRL("STARTED"), 2 TSRMLS_CC);
	/**
	 * A prepared statement
	 */
	memset(&ce, 0, sizeof(ce));
	INIT_NS_CLASS_ENTRY(ce, "IDA", "Statement", php_ida_Statement_me);
	php_ida_Statement_ce = zend_register_internal_interface(&ce TSRMLS_CC);

	/**
	 * A query result (command success, result set, ...)
	 */
	memset(&ce, 0, sizeof(ce));
	INIT_NS_CLASS_ENTRY(ce, "IDA", "Result", php_ida_Result_me);
	php_ida_Result_ce = zend_register_internal_interface(&ce TSRMLS_CC);

	zend_class_implements(php_ida_Result_ce TSRMLS_CC, 2, spl_ce_Countable, zend_ce_traversable);

	zend_declare_class_constant_long(php_ida_Result_ce, ZEND_STRL("FETCH_ARRAY"), 0 TSRMLS_CC);
	zend_declare_class_constant_long(php_ida_Result_ce, ZEND_STRL("FETCH_ASSOC"), 1 TSRMLS_CC);
	zend_declare_class_constant_long(php_ida_Result_ce, ZEND_STRL("FETCH_OBJECT"), 2 TSRMLS_CC);
	/**
	 * A database transaction.
	 */
	memset(&ce, 0, sizeof(ce));
	INIT_NS_CLASS_ENTRY(ce, "IDA", "Transaction", php_ida_Transaction_me);
	php_ida_Transaction_ce = zend_register_internal_interface(&ce TSRMLS_CC);

	zend_declare_class_constant_long(php_ida_Transaction_ce, ZEND_STRL("READ_UNCOMMITTED"), -1 TSRMLS_CC);
	zend_declare_class_constant_long(php_ida_Transaction_ce, ZEND_STRL("READ_COMMITTED"), 0 TSRMLS_CC);
	zend_declare_class_constant_long(php_ida_Transaction_ce, ZEND_STRL("REPEATABLE_READ"), 1 TSRMLS_CC);
	zend_declare_class_constant_long(php_ida_Transaction_ce, ZEND_STRL("SERIALIZABLE"), 2 TSRMLS_CC);
	/**
	 * A large object.
	 */
	memset(&ce, 0, sizeof(ce));
	INIT_NS_CLASS_ENTRY(ce, "IDA", "LOB", php_ida_LOB_me);
	php_ida_LOB_ce = zend_register_internal_interface(&ce TSRMLS_CC);

	zend_declare_class_constant_long(php_ida_LOB_ce, ZEND_STRL("W"), 2 TSRMLS_CC);
	zend_declare_class_constant_long(php_ida_LOB_ce, ZEND_STRL("R"), 4 TSRMLS_CC);
	zend_declare_class_constant_long(php_ida_LOB_ce, ZEND_STRL("RW"), 6 TSRMLS_CC);
	/**
	 * Indicates that the implementation supports persistent connections.
	 */
	memset(&ce, 0, sizeof(ce));
	INIT_NS_CLASS_ENTRY(ce, "IDA\\Feature", "PersistentConnection", php_ida_PersistentConnection_me);
	php_ida_PersistentConnection_ce = zend_register_internal_interface(&ce TSRMLS_CC);

	zend_declare_class_constant_long(php_ida_PersistentConnection_ce, ZEND_STRL("PERSISTENT"), 2 TSRMLS_CC);
	/**
	 * Indicates that the implementation supports asynchronous connections.
	 */
	memset(&ce, 0, sizeof(ce));
	INIT_NS_CLASS_ENTRY(ce, "IDA\\Feature", "AsyncConnection", php_ida_AsyncConnection_me);
	php_ida_AsyncConnection_ce = zend_register_internal_interface(&ce TSRMLS_CC);

	zend_declare_class_constant_long(php_ida_AsyncConnection_ce, ZEND_STRL("ASYNC"), 1 TSRMLS_CC);
	zend_declare_class_constant_long(php_ida_AsyncConnection_ce, ZEND_STRL("POLLING_FAILED"), 0 TSRMLS_CC);
	zend_declare_class_constant_long(php_ida_AsyncConnection_ce, ZEND_STRL("POLLING_READING"), 1 TSRMLS_CC);
	zend_declare_class_constant_long(php_ida_AsyncConnection_ce, ZEND_STRL("POLLING_WRITING"), 2 TSRMLS_CC);
	zend_declare_class_constant_long(php_ida_AsyncConnection_ce, ZEND_STRL("POLLING_OK"), 3 TSRMLS_CC);
	/**
	 * Indicates that the implementation supports transactions.
	 */
	memset(&ce, 0, sizeof(ce));
	INIT_NS_CLASS_ENTRY(ce, "IDA\\Feature", "TransactingConnection", php_ida_TransactingConnection_me);
	php_ida_TransactingConnection_ce = zend_register_internal_interface(&ce TSRMLS_CC);

	/**
	 * Indicates that the connection implementation can start transactions asynchronously.
	 */
	memset(&ce, 0, sizeof(ce));
	INIT_NS_CLASS_ENTRY(ce, "IDA\\Feature", "AsyncTransactingConnection", php_ida_AsyncTransactingConnection_me);
	php_ida_AsyncTransactingConnection_ce = zend_register_internal_interface(&ce TSRMLS_CC);

	/**
	 * Indicates that the transaction implementation supports asynchronous commit/rollback.
	 */
	memset(&ce, 0, sizeof(ce));
	INIT_NS_CLASS_ENTRY(ce, "IDA\\Feature", "AsyncTransaction", php_ida_AsyncTransaction_me);
	php_ida_AsyncTransaction_ce = zend_register_internal_interface(&ce TSRMLS_CC);

	/**
	 * Indicates that the transaction implementation supports savepoints.
	 */
	memset(&ce, 0, sizeof(ce));
	INIT_NS_CLASS_ENTRY(ce, "IDA\\Feature", "SavepointTransaction", php_ida_SavepointTransaction_me);
	php_ida_SavepointTransaction_ce = zend_register_internal_interface(&ce TSRMLS_CC);

	/**
	 * Indicates that the transaction implementation supports creating savepoints asynchronously.
	 */
	memset(&ce, 0, sizeof(ce));
	INIT_NS_CLASS_ENTRY(ce, "IDA\\Feature", "AsyncSavepointTransaction", php_ida_AsyncSavepointTransaction_me);
	php_ida_AsyncSavepointTransaction_ce = zend_register_internal_interface(&ce TSRMLS_CC);

	/**
	 * Indicates that the transaction implementation supports LOB operations.
	 */
	memset(&ce, 0, sizeof(ce));
	INIT_NS_CLASS_ENTRY(ce, "IDA\\Feature", "LOBTransaction", php_ida_LOBTransaction_me);
	php_ida_LOBTransaction_ce = zend_register_internal_interface(&ce TSRMLS_CC);

	/**
	 * Indicates that the statement implementation supports asynchronous execution.
	 */
	memset(&ce, 0, sizeof(ce));
	INIT_NS_CLASS_ENTRY(ce, "IDA\\Feature", "AsyncStatement", php_ida_AsyncStatement_me);
	php_ida_AsyncStatement_ce = zend_register_internal_interface(&ce TSRMLS_CC);


	return SUCCESS;
}

PHP_MINFO_FUNCTION(ida)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "Interfaces for Database Access support", "enabled");
	php_info_print_table_row(2, "Extension version", PHP_IDA_VERSION);
	php_info_print_table_end();
}

zend_module_entry ida_module_entry = {
	STANDARD_MODULE_HEADER,
	"ida",
	NULL,
	PHP_MINIT(ida),
	NULL,
	NULL,
	NULL,
	PHP_MINFO(ida),
	PHP_IDA_VERSION,
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_IDA
ZEND_GET_MODULE(ida)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
