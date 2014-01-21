PHP_ARG_ENABLE(ida, whether to enable ida support,
	[  --enable-ida           Enable ida support])

if test "$PHP_IDA" != "no"; then
	
	PHP_NEW_EXTENSION(ida, php_ida.c, $ext_shared)
	
	PHP_ADD_MAKEFILE_FRAGMENT
fi
