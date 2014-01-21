# IDA

Interfaces for Database Access, or, what PDO could have been.

Imagine a world... no seriously, the intention of IDA is to enable any database
vendor to provide a database access implementation adhering to a commonly
defined application programming interface without the need to implement a PDO
driver which comes with a huge loss of performance in many situations and the
awkwardness of PDO base classes by using interfaces instead.

## Goals

  * common database access programming interfaces
  * freedom for the database system vendor to implement features unique to them
  * 100% native performance, no needless API layers tossed on top

## Open Issues

  * default fetch type
  * query parameter placeholders
  * query parameter types
  * cursor feature
  * charset api
  * metadata api
  * exception subtyping
  * sqlstate errors
  
## Tasks

  * define interfaces
  * write conformance test suite
  * write/migrate implementations

## Exception usage

  * any method that is supposed to return a complex structure (i.e. array or 
    object) must throw an appropriate exception if it is unable to do so.
  
## Interfaces

  * namespace IDA  
    First class interfaces that MUST be implemented by a conforming implementation.
  * namespace IDA\Feature
    Optional feature APIs that SHOULD be implemented by a conforming implemenation.


### IDA\Connection
```php
	<?php
	
	namespace IDA;
	
	/**
	 * Database connection
	 */
	interface Connection
	{
		/**
		 * Connection status is okay
		 */
		const OK = 0;
		
		/**
		 * Connection status is bad
		 */
		const BAD = 1;
		
		/**
		 * Connection has started an asynchronous operation (actually
		 * everything >=2 indicates asynchronous status abd is implementation
		 * defined)
		 */
		const STARTED = 2;
		
		/**
		 * Create a new Connection
		 * @param mixed $dsn
		 * @param int $flags
		 */
		public function __construct($dsn = null, $flags = 0);
		
		/**
		 * Reset the connection
		 * @return void
		 */
		public function reset();
		
		/**
		 * Retrieve the connection status
		 * @return int
		 */
		public function getStatus();
		
		/**
		 * Execute query
		 * @return IDA\Result
		 * @param string $query
		 */
		public function exec($query);
		
		/**
		 * Execute a one-shot (emulated/non-server) prepared query
		 * @return IDA\Result
		 * @param string $query
		 * @param array $params
		 * @param array $types
		 */
		public function execParams($query, array $params, array $types = null);
		
		/**
		 * Prepare query
		 * @return IDA\Statement
		 * @param string $name
		 * @param string $query
		 * @param array $types
		 */
		public function prepare($name, $query, array $types = null);
		
		/**
		 * Quote a string for safe use in a query
		 * @return string
		 * @param string $string
		 */
		public function quote($data);
		
		/**
		 * Quote an identifier
		 * @return string
		 * @param string $name
		 */
		public function quoteName($name);
		
		/**
		 * Escape binary data
		 * @return string
		 * @param string $data
		 */
		public function escapeBinary($data);
		 
		 /**
		  * Unescape binary data
		  * @return string
		  * @param string $data
		  */
		public function unsescapeBinary($data);
	}
	?>
```

### IDA\Statement
```php
	<?php
	
	namespace IDA;
	
	/**
	 * A prepared statement
	 */
	interface Statement
	{
		/**
		 * Get the associated connection
		 * @return IDA\Connection
		 */
		public function getConnection();
		
		/**
		 * Retrieve the name of the statement
		 * @return string
		 */
		public function getName();
		
		/**
		 * Bind a parameter to a variable
		 * @param int $param
		 * @param mixed $ref
		 */
		public function bind($param, &$ref);
		
		/**
		 * Execute the statement, optionally with params $params
		 * @return IDA\Result
		 * @param array $params
		 */
		public function exec(array $params = null);
	}
	?>
```

### IDA\Result
```php
	<?php
	
	namespace IDA;
	
	interface Result extends \Countable, \Traversable
	{
		/**
		 * Fetch as numerically indexed array
		 */
		const FETCH_ARRAY = 0;
		
		/**
		 * Fetch as associative array
		 */
		const FETCH_ASSOC = 1;
		
		/**
		 * Fetch as object
		 */
		const FETCH_OBJECT = 2;
		
		/**
		 * Retreive the number of found rows
		 * @return int
		 */
		public function numRows();
		
		/**
		 * Retrieve the number of columns in the result
		 * @return int
		 */
		public function numCols();
		
		/**
		 * Retrieve the number of affected rows
		 * @return int
		 */
		public function affectedRows();
		
		/**
		 * Get the currently set fetch type
		 * @return int
		 */
		public function getFetchType();
		
		/**
		 * Set the fetch typoe for this result
		 * @return void
		 * @param int $fetch_type
		 */
		public function setFetchType($fetch_type = 0);
		
		/**
		 * Retrieve any error code
		 * @return int
		 */
		public function getError();
		
		/**
		 * Retrieve an error message
		 * @return string
		 */
		public function getErrorMessage();
		
		/**
		 * Retrieve the status code
		 * @return int
		 */
		public function getStatus();
		
		/**
		 * Retrieve a static status message for the current status code
		 * @return string
		 */
		public function getStatusMessage();
		
		/**
		 * Bind column to a variable
		 * @return bool
		 * @param int $column
		 * @param mixed $ref
		 */
		public function bind($column, &$ref);
		
		/**
		 * Fetch the next row, assigning the columns to the bound variables
		 * @return mixed result row
		 */
		public function fetchBound();
		
		/**
		 * Fetch next row
		 * @return mixed result row
		 * @param int $fetch_type
		 */
		public function fetchRow($fetch_type = -1);
		
		/**
		 * Fetch a single column from next row
		 * @return bool
		 * @param int $column
		 * @param mixed $ref
		 */
		public function fetchCol($column, &$ref);
		
		/**
		 * Fetch the complete result set as a simple map, a multi dimensional 
		 * array, each dimension indexed by a column and all columns or 
		 * $val_coulmns building the leaves.
		 * @return array
		 * @param array $key_columns
		 * @param array $val_columns
		 * @param array $fetch_type
		 */
		public function map($key_columns = 0, $val_columns = null, $fetch_type = -1);
	}
	?>
```

### IDA\Transaction
```php
	<?php
	
	namespace IDA;
	
	/**
	 * A database transaction.
	 */
	interface Transaction
	{
		/**
		 * Isolation level "read uncommitted"
		 */
		const READ_UNCOMMITTED = -1;
		
		/**
		 * Isolation level "read committed"
		 */
		const READ_COMMITTED = 0;
		
		/**
		 * Isolation level "repeatable read"
		 */
		const REPEATABLE_READ = 1;
		
		/**
		 * Isolation level "serializable"
		 */
		const SERIALIZABLE = 2;
		
		/**
		 * Retrieve the associated connection
		 * @return IDA\Connection
		 */
		public function getConnection();
		
		/**
		 * Retrieve the current isolation level of the transaction
		 * @return int
		 */
		public function getIsolation();
		
		/**
		 * Set the isolation level of the transaction
		 * @return void
		 * @param int $isolation
		 */
		public function setIsolation($isolation = 0);
		
		/**
		 * Retrieve the read_only flag of the transaction
		 * @return bool
		 */
		public function getReadOnly();
		
		/**
		 * Set the read_only flags of the transaction
		 * @return void
		 * @param bool $read_only
		 */
		public function setReadOnly($read_only = false);
		
		/**
		 * Commit this transaction
		 * @return void
		 */
		public function commit();
		
		/**
		 * Rollback this transaction
		 * @return void
		 */
		public function rollback();
	}
	?>
```

### IDA\LOB
```php
	<?php
	
	namespace IDA;
	
	/**
	 * A large object.
	 */
	interface LOB
	{
		/**
		 * Writable
		 */
		const W = 0x2;
		
		/**
		 * Readable
		 */
		const R = 0x4;
		
		/**
		 * Read- and writable
		 */
		const RW = 0x6;
		
		/**
		 * Retrieve the associated connection
		 * @return IDA\Connection
		 */
		public function getTransaction();
		
		/**
		 * Retrieve the LOB identifier
		 * @return mixed
		 */
		public function getId();
		
		/**
		 * Retrieve a PHP stream wrapper
		 * @return resource
		 */
		public function getStream();
		
		/**
		 * Read from the LOB
		 * @return string
		 * @param int $length
		 * @param int $read actually read length
		 */
		public function read($length, &$read = 0);
		
		/**
		 * Write to the LOB
		 * @return int
		 * @param string $data
		 */
		public function write($data);
		
		/**
		 * Seek to a position within the LOB
		 * @return int
		 * @param int $offset
		 * @param int $whence SEEK_* constant
		 */
		public function seek($offset, $whence = 0);
		
		/**
		 * Tell the current position within the LOB
		 * @return int
		 */
		public function tell();
		
		/**
		 * Truncate the LOB
		 * @return void
		 * @param int $length
		 */
		public function truncate($length = 0);
	}
	?>
```

### IDA\Feature\PersistentConnection
```php
	<?php
	
	namespace IDA\Feature;
	
	/**
	 * Indicates that the implementation supports persistent connections.
	 */
	interface PersistentConnection
	{
		/**
		 * IDA\Connection constructor flag
		 */
		const PERSISTENT = 0x02;
		
		/**
		 * Determine whether this is a persistent connection.
		 * @return bool
		 */
		public function isPersistent();
	}
	?>
```

### IDA\Feature\AsyncConnection
```php
	<?php
	
	namespace IDA\Feature;
	
	/**
	 * Indicates that the implementation supports asynchronous connections.
	 */
	interface AsyncConnection
	{
		/**
		 * IDA\Connection constructo flag
		 */
		const ASYNC = 0x01;
		
		/**
		 * Polling failed
		 */
		const POLLING_FAILED = 0;
		
		/**
		 * Polling indicated that the implementation is waiting for a resource
		 * ready for reading.
		 */
		const POLLING_READING = 1;
		
		/**
		 * Polling indicated that the implementation is waiting for a resource
		 * ready for writing.
		 */
		const POLLING_WRITING = 2;
		
		/**
		 * Polling finished
		 */
		const POLLING_OK = 3;
		
		/**
		 * Determine state of the underlying resource.
		 * @return int self::POLLING_* constant
		 */
		public function poll();
		
		/**
		 * Retrieve the underlying resource for e.g. use with select()
		 * @return resource
		 */
		public function getResource();
		
		/**
		 * Retrieve the connection status
		 * @return int
		 */
		public function getStatus();
		
		/**
		 * Retrieve the last query result
		 * @return IDA\Result
		 */
		public function getResult();
		
		/**
		 * Execute query asynchronously
		 * @return void
		 * @param string $query
		 */
		public function execAsync($query, callable $cb = null);
		
		/**
		 * Execute a one-shot (emulated/non-server) prepared query asynchronously
		 * @return void
		 * @param string $query
		 * @param array $params
		 * @param array $types
		 * @param callable $cb
		 */
		public function execParamsAsync($query, array $params, array $types = null, callable $cb = null);
		
		/**
		 * Prepare query asynchronously
		 * @return IDA\Statement
		 * @param string $name
		 * @param string $query
		 * @param array $types
		 */
		public function prepareAsync($name, $query, array $types = null);
		
		/**
		 * Reset the connection asynchronously
		 * @return void
		 */
		public function resetAsync();
		
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
		public function waitForCompletion();
	}
	?>
```

### IDA\Feature\TransactingConnection
```php
	<?php
	
	namespace IDA\Feature;
	
	/**
	 * Indicates that the implementation supports transactions.
	 */
	interface TransactingConnection
	{
		/**
		 * Start a transaction
		 * @return IDA\Transaction
		 * @param int $isolation
		 * @param bool $readonly
		 */
		public function startTransaction($isolation = 0, $readonly = false);
	}
	?>
```

### IDA\Feature\AsyncTransactingConnection
```php
	<?php
	
	namespace IDA\Feature;
	
	/**
	 * Indicates that the connection implementation can start transactions asynchronously.
	 */
	interface AsyncTransactingConnection
	{
		/**
		 * Start a transaction asynchronously
		 * @return IDA\Transaction
		 * @param int $isolation
		 * @param bool $readonly
		 */
		public function startTransactionAsync($isolation = 0, $readonly = false);
	}
	?>
```

### IDA\Feature\AsyncTransaction
```php
	<?php
	
	namespace IDA\Feature;
	
	/**
	 * Indicates that the transaction implementation supports asynchronous commit/rollback.
	 */
	interface AsyncTransaction
	{
		/**
		 * Commit the transaction asynchronously
		 * @return void
		 */
		public function commitAsync();
		
		/**
		 * Rollback the transaction asynchronously
		 * @return void
		 */
		public function rollbackAsync();
	}
	?>
```

### IDA\Feature\SavepointTransaction
```php
	<?php
	
	namespace IDA\Feature;
	
	/**
	 * Indicates that the transaction implementation supports savepoints.
	 */
	interface SavepointTransaction
	{
		/**
		 * Create a new savepoint which can be committed or rolled back.
		 * @return void
		 */
		public function savepoint();
	}
	?>
```

### IDA\Feature\AsyncSavepointTransaction
```php
	<?php
	
	namespace IDA\Feature;
	
	/**
	 * Indiactes that the transaction implementation supports creating savepoints asynchronously.
	 */
	interface AsyncSavepointTransaction
	{
		/**
		 * Create a savepoint, asynchronously
		 * @return void
		 */
		public function savepointAsync();
	}
	?>
```

### IDA\Feature\LOBTransaction
```php
	<?php
	
	namespace IDA\Feature;
	
	/**
	 * Indicates that the transaction implementation supports LOB operations.
	 */
	interface LOBTransaction
	{
		/**
		 * Open a LOB
		 * @return IDA\LOB
		 * @param mixed $id
		 * @param int $mode IDA\LOB constants
		 */
		public function openLOB($id, $mode = IDA\LOB::RW);
		
		/**
		 * Create a LOB
		 * @return IDA\LOB
		 * @param int $mode
		 */
		public function createLOB($mode = IDA\LOB::RW);
		
		/**
		 * Delete a LOB
		 * @param mixed $id
		 */
		public function unlinkLOB($id);
	}
	?>
```

### IDA\Feature\AsyncStatement
```php
	<?php
	
	namespace IDA\Feature;
	
	/**
	 * Indicates that the statement implementation supports asynchronous execution.
	 */
	interface AsyncStatement
	{
		/**
		 * Execute the statement asynchronously
		 * @return void
		 * @param array $params
		 */
		public function execAsync(array $params = null);
	}
	?>
```
