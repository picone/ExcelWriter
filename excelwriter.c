/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2017 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_excelwriter.h"

#include "xlsxwriter.h"

/* If you declare any globals in php_excelwriter.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(excelwriter)
*/

/* True global resources - no need for thread safety here */
//static int le_excelwriter;

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("excelwriter.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_excelwriter_globals, excelwriter_globals)
    STD_PHP_INI_ENTRY("excelwriter.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_excelwriter_globals, excelwriter_globals)
PHP_INI_END()
*/
/* }}} */

/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_excelwriter_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_excelwriter_compiled)
{
	char *arg = NULL;
	size_t arg_len, len;
	zend_string *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	strg = strpprintf(0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "excelwriter", arg);

	RETURN_STR(strg);
}

PHP_FUNCTION(say)
{
    lxw_workbook *book = workbook_new("test.xlsx");
    lxw_worksheet *sheet = workbook_add_worksheet(book, NULL);

    worksheet_write_string(sheet, 0, 0, "Hello", NULL);
    worksheet_write_number(sheet, 0, 1, 2, NULL);
    workbook_close(book);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and
   unfold functions in source code. See the corresponding marks just before
   function definition, where the functions purpose is also documented. Please
   follow this convention for the convenience of others editing your code.
*/


/* {{{ php_excelwriter_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_excelwriter_init_globals(zend_excelwriter_globals *excelwriter_globals)
{
	excelwriter_globals->global_value = 0;
	excelwriter_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(excelwriter)
{
	/* If you have INI entries, uncomment these lines
	REGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(excelwriter)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(excelwriter)
{
#if defined(COMPILE_DL_EXCELWRITER) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(excelwriter)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(excelwriter)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "excelwriter support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* {{{ excelwriter_functions[]
 *
 * Every user visible function must have an entry in excelwriter_functions[].
 */
const zend_function_entry excelwriter_functions[] = {
	PHP_FE(confirm_excelwriter_compiled,	NULL)		/* For testing, remove later. */
    PHP_FE(say,                             NULL)
	PHP_FE_END	/* Must be the last line in excelwriter_functions[] */
};
/* }}} */

/* {{{ excelwriter_module_entry
 */
zend_module_entry excelwriter_module_entry = {
	STANDARD_MODULE_HEADER,
	"excelwriter",
	excelwriter_functions,
	PHP_MINIT(excelwriter),
	PHP_MSHUTDOWN(excelwriter),
	PHP_RINIT(excelwriter),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(excelwriter),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(excelwriter),
	PHP_EXCELWRITER_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_EXCELWRITER
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(excelwriter)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
