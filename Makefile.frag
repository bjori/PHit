$(top_srcdir)/php_ida.c: $(top_srcdir)/php_ida.c.in | $(top_srcdir)/api.php
	php -dallow_url_include=1 $(top_srcdir)/generator.php $^ <$| >$@
