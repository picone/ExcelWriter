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
  | Author: ChienHo                                                      |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_excelwriter.h"

/* Handlers */
static zend_object_handlers excel_book_object_handlers;
static zend_object_handlers excel_sheet_object_handlers;

/* Class entries */
zend_class_entry *excel_book_ce;
zend_class_entry *excel_sheet_ce;
zend_class_entry *excel_column_ce;

/* {{{ ExcelBook::__construct(filename)*/
PHP_METHOD(ExcelBook, __construct)
{
    char *filename;
    zval *self;
    excel_book_object *object;
    size_t filename_len = 0;
    if (FAILURE == zend_parse_parameters_throw(ZEND_NUM_ARGS(), "p", &filename, &filename_len)) {
        return;
    }
    if (filename_len == 0) {
        return;
    }
    self = getThis();
    object = Z_EXCEL_BOOK_P(self);
    if (!object) {
        return;
    }
    object->book = workbook_new(filename);
    if (!object->book) {
        php_error_docref0(NULL, E_ERROR, "create workbook fail");
    }
}
/* }}} */

/* {{{ ExcelBook::addSheet(sheet_name) */
PHP_METHOD(ExcelBook, addSheet)
{
    char *sheet_name;
    size_t sheet_name_len = 0;
    excel_book_object *object;
    excel_sheet_object *sheet_object;
    if (FAILURE == zend_parse_parameters_throw(ZEND_NUM_ARGS(), "s", &sheet_name, &sheet_name_len)) {
        return;
    }
    object = Z_EXCEL_BOOK_P(getThis());
    if (NULL == object || NULL == object->book) {
        return;
    }
    if (sheet_name_len == 0) {
        sheet_name = NULL;
    }
    object_init_ex(return_value, excel_sheet_ce);
    sheet_object = Z_EXCEL_SHEET_P(return_value);
    sheet_object->sheet = workbook_add_worksheet(object->book, sheet_name);
}
/* }}} */

/* {{ ExcelBook::save() */
PHP_METHOD(ExcelBook, save)
{
    excel_book_object *object = Z_EXCEL_BOOK_P(getThis());
    lxw_error error;
    if (!object) {
        return;
    }
    if (!object->book) {
        RETURN_FALSE
    }
    error = workbook_close(object->book);
    object->book = NULL;
    if (error) {
        RETURN_STRING(lxw_strerror(error))
    } else {
        RETURN_TRUE
    }
}
/* }}} */

/* {{{ ExcelSheet::addColumn*/
PHP_METHOD(ExcelSheet, addColumn)
{
    zend_string *column_name = NULL;
    zval *column_value = NULL, column_zval;
    zend_long column_type = EXCEL_COLUMN_TYPE_STRING;
    excel_sheet_object *object;
#ifdef FAST_ZPP
    ZEND_PARSE_PARAMETERS_START(2, 3)
        Z_PARAM_STR(column_name)
        Z_PARAM_ZVAL(column_value)
        Z_PARAM_OPTIONAL
        Z_PARAM_LONG(column_type)
    ZEND_PARSE_PARAMETERS_END();
#else
    if (FAILURE == zend_parse_parameters_throw(ZEND_NUM_ARGS(), "sz|l", &column_name, &column_value, &column_type)) {
        return;
    }
#endif
    object = Z_EXCEL_SHEET_P(getThis());
    if (!object || !column_name || !column_value || !column_type) {
        RETURN_FALSE
    }
    if (FAILURE == object_init_ex(&column_zval, excel_column_ce)) {
        RETURN_FALSE
    }
    //设置ExcelColumn的属性字段
    zend_update_property_string(excel_column_ce, &column_zval, ZEND_STRL("name"), ZSTR_VAL(column_name));
    zend_update_property_long(excel_column_ce, &column_zval, ZEND_STRL("type"), column_type);
    zend_update_property(excel_column_ce, &column_zval, ZEND_STRL("value"), column_value);
    add_next_index_zval(&object->columns, &column_zval);
    RETURN_TRUE
}

/* }}} */
PHP_METHOD(ExcelSheet, writeByArray)
{
    zval *data, *column_entry, *data_entry, *column_name, *column_type, *column_value, column_return_value, *args, rv;
    zend_ulong row_key, col_key;
    excel_sheet_object *object;
    lxw_col_t column_number = 0;
    if (FAILURE == zend_parse_parameters_throw(ZEND_NUM_ARGS(), "a", &data)) {
        return;
    }
    object = Z_EXCEL_SHEET_P(getThis());
    if (!object) {
        return;
    }
    ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(&object->columns), column_entry) {
        column_name = zend_read_property(excel_column_ce, column_entry, ZEND_STRL("name"), 0, &rv);
        worksheet_write_string(object->sheet, 0, column_number, Z_STRVAL_P(column_name), NULL);
        column_number ++;
    } ZEND_HASH_FOREACH_END();
    worksheet_freeze_panes(object->sheet, 1, column_number);
    args = safe_emalloc(sizeof(zval), 1, 0);
    ZEND_HASH_FOREACH_NUM_KEY_VAL(Z_ARRVAL_P(data), row_key, data_entry) {
        ZEND_HASH_FOREACH_NUM_KEY_VAL(Z_ARRVAL_P(&object->columns), col_key, column_entry) {
            column_value = zend_read_property(excel_column_ce, column_entry, ZEND_STRL("value"), 0, &rv);
            if (Z_TYPE_INFO_P(column_value) == IS_LONG) {

            } else if (Z_TYPE_INFO_P(column_value) == IS_STRING) {
                if (zend_hash_exists(Z_ARRVAL_P(data_entry), Z_STR_P(column_value))) {
                    column_return_value = *(zend_hash_find(Z_ARRVAL_P(data_entry), Z_STR_P(column_value)));
                } else {
                    zend_error_noreturn(E_ERROR, "Column %s not exist in row %d and col %d", Z_STR_P(column_value), row_key, col_key);
                }
            } else if(zend_is_callable(column_value, IS_CALLABLE_CHECK_NO_ACCESS, 0)) {
                ZVAL_COPY(&args[0], data_entry);
                call_user_function_ex(EG(function_table), NULL, column_value, &column_return_value, 1, args, 0, NULL);
            } else {
                zend_error_noreturn(E_ERROR, "Unknown excel column value type %d", Z_TYPE_INFO_P(column_value));
            }
            column_type = zend_read_property(excel_column_ce, column_entry, ZEND_STRL("type"), 0, &rv);
            if (Z_LVAL_P(column_type) == EXCEL_COLUMN_TYPE_NUMBER) {
                if (Z_TYPE_INFO(column_return_value) == IS_LONG) {
                    worksheet_write_number(object->sheet, (lxw_row_t)(row_key + 1), (lxw_col_t)col_key, Z_LVAL(column_return_value), NULL);
                } else if(Z_TYPE_INFO(column_return_value)) {
                    worksheet_write_number(object->sheet, (lxw_row_t)(row_key + 1), (lxw_col_t)col_key, Z_DVAL(column_return_value), NULL);
                }
            } else {
                worksheet_write_string(object->sheet, (lxw_row_t)(row_key + 1), (lxw_col_t)col_key, Z_STRVAL(column_return_value), NULL);
            }
        } ZEND_HASH_FOREACH_END();
    } ZEND_HASH_FOREACH_END();
    efree(args);
}
/* {{{ */

/* {{{ ExcelSheet::getColumns */
PHP_METHOD(ExcelSheet, getColumns)
{
    excel_sheet_object *object= Z_EXCEL_SHEET_P(getThis());
    RETURN_ZVAL(&object->columns, 0, 1);
}
/* }}} */

/* {{{ ExcelBook对象创建的方法 */
static zend_object *excel_book_object_new(zend_class_entry *class_type)
{
    excel_book_object *intern;
    intern = ecalloc(1, sizeof(excel_book_object) + zend_object_properties_size(class_type));
    zend_object_std_init(&intern->std, class_type);
    object_properties_init(&intern->std, class_type);
    intern->std.handlers = &excel_book_object_handlers;
    return &intern->std;
}
/* }}} */

/* {{{ ExcelBook对象释放的方法 */
static void excel_book_object_free(zend_object *object)
{
    excel_book_object *intern = php_excel_book_fetch_obj(object);
    if (!intern) {
        return;
    }
    if (intern->book) {
        lxw_workbook_free(intern->book);
    }
    intern->book = NULL;
    zend_object_std_dtor(&intern->std);
}
/* }}} */

/* {{{ ExcelSheet对象创建方法 */
static zend_object *excel_sheet_object_new(zend_class_entry *class_type)
{
    excel_sheet_object *intern;
    intern = ecalloc(1, sizeof(excel_sheet_object) + zend_object_properties_size(class_type));
    zend_object_std_init(&intern->std, class_type);
    object_properties_init(&intern->std, class_type);
    intern->std.handlers = &excel_sheet_object_handlers;
    array_init(&intern->columns);
    return &intern->std;
}
/* }}} */

/* {{{ ExcelSheet对象释放方法 */
static void excel_sheet_object_free(zend_object *object)
{
    excel_sheet_object *intern = php_excel_sheet_fetch_obj(object);
    if (!intern) {
        return;
    }
    intern->sheet = NULL;
    zend_object_std_dtor(&intern->std);
}
/* }}} */

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO_EX(arginfo_excel_book_construct, 0, 0, 1)
    ZEND_ARG_INFO(0, filename)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_excel_book_add_sheet, 0, 0, 0)
    ZEND_ARG_INFO(0, sheet_name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_excel_sheet_add_column, 0, 0, 2)
    ZEND_ARG_INFO(0, column_name)
    ZEND_ARG_INFO(0, column_value)
    ZEND_ARG_INFO(0, column_type)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_excel_sheet_write_by_array, 0, 0, 1)
    ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ ExcelBook的方法 */
const zend_function_entry excel_book_methods[] = {
    PHP_ME(ExcelBook, __construct, arginfo_excel_book_construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
    PHP_ME(ExcelBook, addSheet,    arginfo_excel_book_add_sheet, ZEND_ACC_PUBLIC)
    PHP_ME(ExcelBook, save,        NULL,                         ZEND_ACC_PUBLIC)
    PHP_FE_END
};
/* }}} */

/* {{{ */
const zend_function_entry excel_sheet_methods[] = {
    PHP_ME(ExcelSheet, addColumn,    arginfo_excel_sheet_add_column,     ZEND_ACC_PUBLIC)
    PHP_ME(ExcelSheet, writeByArray, arginfo_excel_sheet_write_by_array, ZEND_ACC_PUBLIC)
    PHP_ME(ExcelSheet, getColumns,   NULL,                               ZEND_ACC_PUBLIC)
    PHP_FE_END
};
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(excelwriter)
{
    zend_class_entry ce;

    memcpy(&excel_book_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    memcpy(&excel_sheet_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

    INIT_CLASS_ENTRY(ce, "ExcelBook", excel_book_methods);
    ce.create_object = excel_book_object_new;
    excel_book_object_handlers.free_obj = excel_book_object_free;//重新定义析构方法
    excel_book_object_handlers.clone_obj = NULL;
    excel_book_ce = zend_register_internal_class(&ce);

    INIT_CLASS_ENTRY(ce, "ExcelSheet", excel_sheet_methods);
    ce.create_object = excel_sheet_object_new;
    excel_sheet_object_handlers.free_obj = excel_sheet_object_free;
    excel_sheet_object_handlers.clone_obj = NULL;
    excel_sheet_ce = zend_register_internal_class(&ce);

    INIT_CLASS_ENTRY(ce, "ExcelColumn", NULL);
    ce.create_object = NULL;
    excel_column_ce = zend_register_internal_class(&ce);
    zend_declare_property_null(excel_column_ce, ZEND_STRL("name"), ZEND_ACC_PUBLIC);
    zend_declare_property_null(excel_column_ce, ZEND_STRL("type"), ZEND_ACC_PUBLIC);
    zend_declare_property_null(excel_column_ce, ZEND_STRL("value"), ZEND_ACC_PUBLIC);
    zend_declare_class_constant_long(excel_column_ce, ZEND_STRL("TYPE_NUMBER"), EXCEL_COLUMN_TYPE_NUMBER);
    zend_declare_class_constant_long(excel_column_ce, ZEND_STRL("TYPE_STRING"), EXCEL_COLUMN_TYPE_STRING);
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION */
PHP_MSHUTDOWN_FUNCTION(excelwriter)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION */
PHP_MINFO_FUNCTION(excelwriter)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "excelwriter support", "enabled");
	php_info_print_table_end();
}
/* }}} */

/* {{{ excelwriter_module_entry */
zend_module_entry excelwriter_module_entry = {
	STANDARD_MODULE_HEADER,
	"excelwriter",
	NULL,
	PHP_MINIT(excelwriter),
	PHP_MSHUTDOWN(excelwriter),
	NULL,
	NULL,
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
