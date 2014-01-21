	<?=$docblock?>
	memset(&ce, 0, sizeof(ce));
	INIT_NS_CLASS_ENTRY(ce, "<?=$ns?>", "<?=$class?>", php_ida_<?=$class?>_me);
	php_ida_<?=$class?>_ce = zend_register_internal_interface(&ce TSRMLS_CC);

