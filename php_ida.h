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

#ifndef PHP_IDA_H
#define PHP_IDA_H

extern int ida_module_number;
extern zend_module_entry ida_module_entry;
#define phpext_ida_ptr &ida_module_entry

#define PHP_IDA_VERSION "0.1.0dev"

#ifdef PHP_WIN32
#	define PHP_IDA_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_IDA_API extern __attribute__ ((visibility("default")))
#else
#	define PHP_IDA_API extern
#endif

/* {{{ PHP_IDA_EXPORTS */
/* Base */
PHP_IDA_API zend_class_entry *php_ida_Connection_ce;
PHP_IDA_API zend_class_entry *php_ida_Statement_ce;
PHP_IDA_API zend_class_entry *php_ida_Result_ce;
PHP_IDA_API zend_class_entry *php_ida_Transaction_ce;
PHP_IDA_API zend_class_entry *php_ida_LOB_ce;

/* Features */
PHP_IDA_API zend_class_entry *php_ida_AsyncConnection_ce;
PHP_IDA_API zend_class_entry *php_ida_TransactingConnection_ce;
PHP_IDA_API zend_class_entry *php_ida_AsyncTransactingConnection_ce;
PHP_IDA_API zend_class_entry *php_ida_AsyncTransaction_ce;
PHP_IDA_API zend_class_entry *php_ida_SavepointTransaction_ce;
PHP_IDA_API zend_class_entry *php_ida_AsyncSavepointTransaction_ce;
PHP_IDA_API zend_class_entry *php_ida_LOBTransaction_ce;
PHP_IDA_API zend_class_entry *php_ida_AsyncStatement_ce;
/* }}} */

#endif	/* PHP_IDA_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
