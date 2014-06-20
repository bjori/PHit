<?php

function render($include, array $data = null) {
	if ($data) {
		extract($data);
	}
	ob_start();
	include $include;
	return ob_get_clean();
}

function untab($text) {
	$text = preg_replace("/^\t/m", "", $text);
	$text = preg_replace("/^    /m", "\t", $text);
    return $text;
}

function map_iface($i) {
	$map = [
		"Iterator" => "spl_ce_Iterator",
		"Countable" => "spl_ce_Countable",
	];
    $user = substr($i, strrpos($i, "\\")+1);

    $user = strtolower($user);
	return isset($map[$i]) ? $map[$i] : "php_phongo_{$user}_ce";
}

if ($_SERVER["argc"] !== 2 || !is_file($_SERVER["argv"][1])) {
	printf("Usage:\n\tphp -dauto_prepend_file=README.md %s php_phongo.c.in >php_phongo.c\n", 
		basename($_SERVER["argv"][0]));
	var_dump($argc,$argv);
	exit(-1);
}

$interfaces = $classes = array();
foreach(array(get_declared_interfaces(), get_declared_classes()) as $entry) {
    foreach ($entry as $iface) {
        $reflection = new ReflectionClass($iface);

        if ($reflection->isInternal()) {
            fprintf(STDERR, "Skipping $iface\n");
            continue;
        }
        if ($reflection->isInterface()) {
            $interfaces[$reflection->getShortName()] = $iface;
        } else {
            $classes[$reflection->getShortName()] = $iface;
        }
    }
}
foreach(array($interfaces, $classes) as $entry) {
    foreach ($entry as $iface) {
$declarations = "";
$registrations = "";
$implementations = "";

        $reflection = new ReflectionClass($iface);
        $pos = strrpos($iface, "\\");
        $ns = substr($iface, 0, $pos);
        $class = substr($iface, $pos + 1);
        $ns = str_replace("\\", "\\\\", $ns);
        $docblock = untab($reflection->getDocComment())."\n";
        $classorinterface = $reflection->isInterface() ? "interface" : "class";
        if ($parent = $reflection->getParentClass()) {
            $parent = $parent->getName();
        } else {
            $parent = "NULL";
        }
        $registrations .= render("generator/$classorinterface.php", 
            compact("ns", "class", "docblock", "parent"));
        $entries = "";
        $arginfos = "";

        if (($impl_ifaces = $reflection->getInterfaceNames())) {
            $faces = array();
            foreach($impl_ifaces as $face) {
                if ($face == "Traversable") {
                    continue;
                }
                foreach($faces as $f) {
                    $rfc = null;
                    try {
                        $rf = new ReflectionClass($f);
                    } catch(Exception $e) {
                    }
                    if (!$rf || $rf->implementsInterface($face)) {
                        continue 2;
                    }
                }
                $faces[] = map_iface($face);
            }
            $ni = count($faces);
            $impl_list = implode(", ", $faces);
            $registrations .= render("generator/implements.php", 
                compact("class", "ni", "impl_list"));
        }
        foreach ($reflection->getConstants() as $cname => $cvalue) {
            $registrations .= render("generator/constant.php",
                compact("class", "cname", "cvalue"));
        }
        foreach ($reflection->getMethods() as $m) {
            $zpp = array();
            /* @var $m ReflectionMethod */
            if ($m->getDeclaringClass()->getName() !== $iface) {
                continue;
            }
            $method = $m->getName();
            $docblock = untab($m->getDocComment())."\n";
            $template = $reflection->isInterface() ? "abstract_me.php" : "me.php";
            $entries .= render("generator/$template", 
                compact("class", "method", "docblock"));
            $args = "";
            $req_args = 0;
            $protoargs = "";
            $n = 0;
            foreach ($m->getParameters() as $n => $param) {
                if ($n > 0) {
                    if ($param->isOptional()) {
                        $protoargs .= "[, ";
                    } else {
                        $protoargs .= ", ";
                    }
                }




                /* @var $param ReflectionParameter */
                $arg = $param->getName();
                $null = (int) $param->allowsNull();
                $by_ref = (int) $param->isPassedByReference();
                try {
                    $param->getClass();
                } catch(Exception $e) {
                    echo "Broken typehint:\n";
                    echo $class, "::", $m->getName();
                    exit;
                }

                list($type, $short, $argprotohint, $rethint) = get_zpp_type($n+1, $m->getNumberOfParameters(), $param, $docblock);
                if ($param->isArray()) {
                    $args .= render("generator/ai_arr.php", 
                        compact("class", "method", "arg", "null", "by_ref"));
                    $protoargs .= "array \$$arg";
                    if ($param->isOptional()) {
                        $protoargs .= " = array()";
                    }
                } elseif (($arg_class = $param->getClass())) {
                    $arg_class = $arg_class->getName();
                    $protoargs .= "$arg_class \$$arg";
                    $args .= render("generator/ai_obj.php", 
                        compact("class", "method", "arg", "null", "by_ref", "arg_class"));
                    if ($param->isOptional()) {
                        $protoargs .= " = null";
                    }
                } else {
                    $callable = $param->isCallable();
                    $protoargs .= $callable ? "callable \$$arg" : "$argprotohint \$$arg";
                    $args .= render("generator/ai_std.php", 
                        compact("class", "method", "arg", "null", "by_ref", "callable"));
                }
                $zpp[] = array(
                    "type" => $type,
                    "short" => $short,
                    "name"  => $arg,
                );
                $req_args += !$param->isOptional();
            }
            $classtype = "php_phongo_{$class}_t";
            $code = render("generator/zpp.php", 
                compact("zpp", "req_args", "classtype"));
            if ($n>0) {
                $protoargs .= str_repeat("]", ($n+1)-$req_args);
            }

            $arginfos .= render("generator/arginfos.php", 
                compact("class", "method", "args", "req_args", "docblock"));

            $oneliner = explode("\n", $docblock)[1];
            $oneliner = "   " . trim($oneliner, " *\t");
            $rettype = get_return_type($docblock);
            if ($method == "__construct") {
                $rettype = str_replace("\\\\", "\\", $ns) . "\\" . $class;
            }
            if (!$reflection->isInterface()) {
                $implementations .= render("generator/implementations.php", 
                    compact("class", "method", "oneliner", "protoargs", "code", "rettype"));
            }
        }
        $docblock = untab($reflection->getDocComment())."\n";
        $declarations .= render("generator/declaration.php", 
            compact("ns", "class", "entries", "docblock", "arginfos"));
        $minitname = $class;
        $data = render($_SERVER["argv"][1], compact("declarations", "registrations", "implementations", "minitname", "class"));
        file_put_contents("src/$class.c", $data);
    }
}

function get_return_type($docblock) {
    global $classes, $interfaces;

    $blocks = explode("\n", trim($docblock));
    foreach($blocks as $tmp) {
        if(strpos($tmp, "@return") !== false) {
            list($nada, $param, $type, $name) = explode(" ", trim($tmp));
            if (isset($classes[$type])) {
                return $classes[$type];
            }
            if (isset($interfaces[$type])) {
                return $interfaces[$type];
            }
            return $type;
        }
    }
    return "void";
}
function get_zpp_type($current, $max, $param, $docblock) {
    if ($param->isArray()) {
        return array("zval", "a");
    }
    $blocks = explode("\n", trim($docblock));
    $argn = 0;
    foreach($blocks as $argproto) {
        if(strpos($argproto, "@param") !== false) {
            if (++$argn == $current) {
                break;
            }
        }
    }
    if (strpos($argproto, $param->getName()) === false) {
        echo "Error - Not the argument name I expected in docblock\n";
        var_dump($param->getDeclaringClass()->getName(), $param->getName(), $argproto);
        exit;
    }
    list($nada, $param, $type, $name) = explode(" ", trim($argproto));
    switch($type) {
    case "string":
        return array("char", "s", "string");
    case "integer":
        return array("long", "l", "integer");
    case "array|object":
        return array("zval", "A", "array|object");
    case "mixed":
    case "zval":
        return array("zval", "z", "mixed");
    default:
        return array($type, "O", $type);
    }
}

function getZPPType($type) {
    switch($type) {
    case "zval":
    case "char":
    case "long":
    case "zend_bool":
        return $type;
    default:
        return "zval";
    }
}
