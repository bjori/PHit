<?php if (is_numeric($cvalue)): ?>
	zend_declare_class_constant_long(php_phongo_<?=strtolower($class)?>_ce, ZEND_STRL("<?=$cname?>"), 0x<?=sprintf("%02s", dechex($cvalue))?> TSRMLS_CC);
<?php elseif (defined($cname)): ?>
    zend_declare_class_constant_long(php_phongo_<?=strtolower($class)?>_ce, ZEND_STRL("<?=$cname?>"), <?=constant($cname)?> TSRMLS_CC);
<?php else: ?>
	zend_declare_class_constant_string(php_phongo_<?=strtolower($class)?>_ce, ZEND_STRL("<?=$cname?>"), "<?=$cvalue?>" TSRMLS_CC);
<?php endif ?>
