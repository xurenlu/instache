<?php
$br = (php_sapi_name() == "cli")? "":"<br>";

if(!extension_loaded('instache')) {
	dl('instache.' . PHP_SHLIB_SUFFIX);
}
$module = 'instache';
$functions = get_extension_funcs($module);
echo "Functions available in the test extension:$br\n";
foreach($functions as $func) {
    echo $func."$br\n";
}
echo "$br\n";
$function = 'confirm_' . $module . '_compiled';
if (extension_loaded($module)) {
	$str = $function($module);
} else {
	$str = "Module $module is not compiled into PHP";
}
echo "$str\n";
echo "\n\n";
instache_put("t1","val1");
instache_put("t2","val2");
instache_put("t3","val3");
assert("val1"== instache_get("t1"));
assert("val2"== instache_get("t2"));
assert("val3"== instache_get("t3"));
//instache_free();
?>
