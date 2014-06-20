	INIT_NS_CLASS_ENTRY(ce, "<?=$ns?>", "<?=$class?>", php_phongo_<?=strtolower($class)?>_me);
<?php if($parent != "NULL"): ?>
	php_phongo_<?=strtolower($class)?>_ce = zend_register_internal_class_ex(&ce, NULL, "<?=$parent?>" TSRMLS_CC);
<?php else: ?>
	php_phongo_<?=strtolower($class)?>_ce = zend_register_internal_class(&ce TSRMLS_CC);
<?php endif ?>
