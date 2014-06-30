	INIT_NS_CLASS_ENTRY(ce, "<?=$ns?>", "<?=$class?>", php_phongo_<?=strtolower($class)?>_me);
	ce.create_object = php_phongo_<?=strtolower($class)?>_create_object;
<?php if($parent != "NULL"): ?>
	php_phongo_<?=strtolower($class)?>_ce = zend_register_internal_class_ex(&ce, NULL, "<?=$parent?>" TSRMLS_CC);
<?php else: ?>
	php_phongo_<?=strtolower($class)?>_ce = zend_register_internal_class(&ce TSRMLS_CC);
<?php endif ?>
<?php if ($isfinalclass): ?>
    php_phongo_<?=strtolower($class)?>_ce.ce_flags |= ZEND_ACC_FINAL_CLASS
<?php endif ?>
