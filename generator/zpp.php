	<?=sprintf("%-21s *intern;\n", strtolower($classtype))?>
	zend_error_handling	error_handling;
<?php
$short = "";
$args = array();
foreach($zpp as $n => $arg) {
	printf("\t%-22s%s%s;\n", getZPPType($arg["type"]), in_array($arg["type"], array("int", "long")) ? " " : "*", $arg["name"]);
	if ($n == $req_args) {
		$short .= "|";
	}
	if ($arg["short"]) {
		$short .= $arg["short"];
	}
	$args[] = "&{$arg["name"]}";
	if ($arg["type"] == "char") {
		printf("\t%-22s%s%s;\n", "int", " ", $arg["name"] . "_len");
		$args[] = "&{$arg["name"]}_len";
	} else if ($arg["short"] == "O") {
		$n = strtolower($arg["type"]);
		$args[] = "php_phongo_{$n}_ce";
	}
}
$args = join(", ", $args);
?>

	(void)return_value; (void)return_value_ptr; (void)return_value_used; /* We don't use these */

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (<?=strtolower($classtype)?> *)zend_object_store_get_object(getThis() TSRMLS_CC);

<?php if($args): ?>
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "<?=$short?>", <?=$args?>) == FAILURE) {
<?php else: ?>
	if (zend_parse_parameters_none() == FAILURE) {
<?php endif ?>
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);
