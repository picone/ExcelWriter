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

#ifndef PHP_EXCELWRITER_H
#define PHP_EXCELWRITER_H

#include "xlsxwriter.h"

#define EXCEL_COLUMN_TYPE_NUMBER 1<<0
#define EXCEL_COLUMN_TYPE_STRING 1<<1

extern zend_module_entry excelwriter_module_entry;
#define phpext_excelwriter_ptr &excelwriter_module_entry

#define PHP_EXCELWRITER_VERSION "0.1.0"

#ifdef PHP_WIN32
#	define PHP_EXCELWRITER_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_EXCELWRITER_API __attribute__ ((visibility("default")))
#else
#	define PHP_EXCELWRITER_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#if defined(ZTS) && defined(COMPILE_DL_EXCELWRITER)
ZEND_TSRMLS_CACHE_EXTERN()
#endif

typedef struct _excel_book_object {
    lxw_workbook *book;
    zend_object std;
} excel_book_object;

static inline excel_book_object *php_excel_book_fetch_obj(zend_object *obj) {
    return (excel_book_object *)((char*)(obj) - XtOffsetOf(excel_book_object, std));
}

#define Z_EXCEL_BOOK_P(zv) php_excel_book_fetch_obj(Z_OBJ_P((zv)))

typedef struct _excel_sheet_object {
    lxw_worksheet *sheet;
    zval columns;
    zend_object std;
} excel_sheet_object;

static inline excel_sheet_object *php_excel_sheet_fetch_obj(zend_object *obj) {
    return (excel_sheet_object *)((char *)(obj) - XtOffsetOf(excel_sheet_object, std));
}

#define Z_EXCEL_SHEET_P(zv) php_excel_sheet_fetch_obj(Z_OBJ_P((zv)))

#endif	/* PHP_EXCELWRITER_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
