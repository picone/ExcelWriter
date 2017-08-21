cmake_minimum_required(VERSION 3.7)
project(excelwriter)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        config.h
        excelwriter.c
        php_excelwriter.h)

add_executable(excelwriter ${SOURCE_FILES})

set(PHP_SOURCE /usr/local/include/php)

INCLUDE_DIRECTORIES(${PHP_SOURCE}/main)
INCLUDE_DIRECTORIES(${PHP_SOURCE}/Zend)
INCLUDE_DIRECTORIES(${PHP_SOURCE}/sapi)
INCLUDE_DIRECTORIES(${PHP_SOURCE}/pear)
INCLUDE_DIRECTORIES(${PHP_SOURCE}/TSRM)
INCLUDE_DIRECTORIES(${PHP_SOURCE})
INCLUDE_DIRECTORIES(include)

#add_custom_target(makefile COMMAND phpize && ./configure --with-php-config=php-config && make WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
add_custom_target(makefile COMMAND make WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} COMMAND make install)
