	<?=sprintf("%-24s *intern;\n", strtolower($classtype))?>
	zend_error_handling       error_handling;
<?php
$short = "";
$args = array();
foreach($zpp as $n => $arg) {
	printf("\t%-25s%s%s%s;\n", getZPPType($arg["type"]), in_array($arg["type"], array("int", "long", "zend_bool")) ? " " : "*", $arg["name"], $n >= $req_args ? " = ". $arg["default"] : "");
	if ($n == $req_args) {
		$short .= "|";
	}
	if ($arg["short"]) {
        $short .= $arg["short"];
        if ($arg["default"] == "NULL" && !in_array($arg["type"], array("boolean", "long", "double"))) {
            $short .= "!";
        }
	}
	$args[] = "&{$arg["name"]}";
	if ($arg["type"] == "char") {
		printf("\t%-25s%s%s;\n", "int", " ", $arg["name"] . "_len");
		$args[] = "&{$arg["name"]}_len";
	} else if ($arg["short"] == "O") {
		$n = strtolower($arg["type"]);
		$args[] = "php_phongo_{$n}_ce";
	}
}
$args = join(", ", $args);
if (!empty($codes["cef"])) {
    echo $codes["cef"], "\n";
}
?>


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
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

<?php

if (!empty($codes["cimpl"])) {
    echo "\n";
    echo $codes["cimpl"], "\n";
}

