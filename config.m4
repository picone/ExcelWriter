dnl $Id$
dnl config.m4 for extension excelwriter

PHP_ARG_ENABLE(excelwriter, whether to enable excelwriter support,
[  --enable-excelwriter           Enable excelwriter support])

if test "$PHP_EXCELWRITER" != "no"; then
  PHP_NEW_EXTENSION(excelwriter, excelwriter.c libxlsxwriter/app.c libxlsxwriter/chart.c libxlsxwriter/content_types.c \
libxlsxwriter/core.c libxlsxwriter/custom.c libxlsxwriter/drawing.c libxlsxwriter/format.c libxlsxwriter/hash_table.c \
libxlsxwriter/packager.c libxlsxwriter/relationships.c libxlsxwriter/shared_strings.c libxlsxwriter/styles.c \
libxlsxwriter/theme.c libxlsxwriter/utility.c libxlsxwriter/workbook.c libxlsxwriter/worksheet.c libxlsxwriter/xmlwriter.c \
minizip/ioapi.c minizip/miniunz.c minizip/mztools.c minizip/unzip.c minizip/zip.c \
tmpfileplus/tmpfileplus.c,$ext_shared,, -I @ext_srcdir@/libxlsxwriter -I @ext_srcdir@/minizip -I @ext_srcdir@/tmpfileplus -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
  PHP_ADD_BUILD_DIR($ext_builddir/libxlsxwriter)
  AC_DEFINE(HAVE_LIBEXCEL, 1, [Whether you have libexcel])
fi
