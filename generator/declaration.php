/* {{{ <?=str_replace("\\\\", "\\", $ns)?>\<?=$class?> */

<?=$docblock?>
zend_class_entry *php_ida_<?=$class?>_ce;

<?=$arginfos?>

static zend_function_entry php_ida_<?=$class?>_me[] = {
<?=$entries?>
	{NULL}
};

/* }}} */

