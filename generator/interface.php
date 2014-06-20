	INIT_NS_CLASS_ENTRY(ce, "<?=$ns?>", "<?=$class?>", php_phongo_<?=strtolower($class)?>_me);
	php_phongo_<?=strtolower($class)?>_ce = zend_register_internal_interface(&ce TSRMLS_CC);
