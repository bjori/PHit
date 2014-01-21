<?php

function render($include, array $data = null) {
	if ($data) {
		extract($data);
	}
	ob_start();
	include $include;
	return ob_get_clean();
}

function inc($template, array $data = null) {
	return render("data://text/plain;base64,".base64_encode($template."\n"), $data);
}

function untab($text) {
	return preg_replace("/^\t/m", "", $text);
}

function map_iface($i) {
	$map = [
		"Countable" => "spl_ce_Countable",
		"Traversable" => "zend_ce_traversable"
	];
	return $map[$i];
}

if (!ini_get("allow_url_include")) {
	printf("Need -dallow_url_include=1 for including data://\n");
	exit(-1);
}

if ($_SERVER["argc"] !== 2 || !is_file($_SERVER["argv"][1])) {
	printf("Usage:\n\tphp -dallow_url_include=1 %s php_ida.c.in <README.md >php_ida.c\n", 
		basename($_SERVER["argv"][0]));
	var_dump($argc,$argv);
	exit(-1);
}

if (!preg_match_all("/(\<\?php.*?)\?\>/s", file_get_contents("php://stdin"), $files)) {
	printf("Invalid input\n");
	exit(-1);
}

foreach ($files[1] as $file) {
	inc(str_replace("namespace IDA", "namespace reference\\IDA", $file));
}

$declarations = "";
$registrations = "";

foreach (get_declared_interfaces() as $iface) {
	if (substr($iface, 0, 14) !== "reference\\IDA\\") {
		continue;
	}
	$pos = strrpos($iface, "\\");
	$ns = str_replace("\\", "\\\\", substr($iface, 10, $pos - 10));
	$class = substr($iface, $pos + 1);
	$reflection = new ReflectionClass($iface);
	$docblock = $reflection->getDocComment()."\n";
	$registrations .= render("generator/registration.php", 
			compact("ns", "class", "docblock"));
	$entries = "";
	$arginfos = "";

	if (($impl_ifaces = $reflection->getInterfaceNames())) {
		$ni = count($impl_ifaces);
		$impl_list = implode(", ", array_map("map_iface", $impl_ifaces));
		$registrations .= render("generator/implements.php", 
				compact("class", "ni", "impl_list"));
	}
	foreach ($reflection->getConstants() as $cname => $cvalue) {
		$registrations .= render("generator/constant.php",
				compact("class", "cname", "cvalue"));
	}
	foreach ($reflection->getMethods() as $m) {
		/* @var $m ReflectionMethod */
		if ($m->getDeclaringClass()->getName() !== $iface) {
			continue;
		}
		$method = $m->getName();
		$docblock = untab($m->getDocComment())."\n";
		$entries .= render("generator/me.php", 
				compact("class", "method", "docblock"));
		$args = "";
		$req_args = 0;
		foreach ($m->getParameters() as $param) {
			/* @var $param ReflectionParameter */
			$arg = $param->getName();
			$null = (int) $param->allowsNull();
			$by_ref = (int) $param->isPassedByReference();
			if ($param->isArray()) {
				$args .= render("generator/ai_arr.php", 
						compact("class", "method", "arg", "null", "by_ref"));
			} elseif (($arg_class = $param->getClass())) {
				$args .= render("generator/ai_obj.php", 
						compact("class", "method", "arg", "null", "by_ref", "arg_class"));
			} else {
				$callable = $param->isCallable();
				$args .= render("generator/ai_std.php", 
						compact("class", "method", "arg", "null", "by_ref", "callable"));
			}
			$req_args += !$param->isOptional();
		}
		$docblock = untab($docblock);
		$arginfos .= render("generator/arginfos.php", 
				compact("class", "method", "args", "req_args", "docblock"));
	}
	$docblock = untab($reflection->getDocComment())."\n";
	$declarations .= render("generator/declaration.php", 
			compact("ns", "class", "entries", "docblock", "arginfos"));
}

echo render($_SERVER["argv"][1], compact("declarations", "registrations"));