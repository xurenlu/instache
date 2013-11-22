/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2013 The PHP Group                                |
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
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_instache.h"
#include "liteutil.h"
#include "hashfunc.h"

/* If you declare any globals in php_instache.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(instache)
*/
#define INSTACHE_BUCKET_SIZE 16384

/* True global resources - no need for thread safety here */
static int le_instache;
hl_list * instache_ht=NULL;
/* {{{ instache_functions[]
 *
 * Every user visible function must have an entry in instache_functions[].
 */
const zend_function_entry instache_functions[] = {
	PHP_FE(confirm_instache_compiled,	NULL)		/* For testing, remove later. */
	PHP_FE(instache_put,	NULL)		/* For testing, remove later. */
	PHP_FE(instache_get,	NULL)		/* For testing, remove later. */
       
	PHP_FE_END	/* Must be the last line in instache_functions[] */
};
/* }}} */

/* {{{ instache_module_entry
 */
zend_module_entry instache_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"instache",
	instache_functions,
	PHP_MINIT(instache),
	PHP_MSHUTDOWN(instache),
	PHP_RINIT(instache),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(instache),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(instache),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_INSTACHE
ZEND_GET_MODULE(instache)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("instache.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_instache_globals, instache_globals)
    STD_PHP_INI_ENTRY("instache.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_instache_globals, instache_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_instache_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_instache_init_globals(zend_instache_globals *instache_globals)
{
	instache_globals->global_value = 0;
	instache_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(instache)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
    instache_ht = hl_new_list ( INSTACHE_BUCKET_SIZE );
    int i=0;
    for(i=0;i<INSTACHE_BUCKET_SIZE;i++){
        hl_list_append(instache_ht, NULL);
    }
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(instache)
{
   
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
    int i=0;
    hl_link * tmp=NULL;
    hl_link * iter=NULL;
    hl_link * last=NULL;
    for(i=0;i<INSTACHE_BUCKET_SIZE;i++){
       hl_list_at(instache_ht,i,tmp);
       if(tmp!=NULL){
            hl_link_iter(tmp,iter){   
                last = iter;
                free(iter->key);
                free(iter->data);
            }
            if(iter!=NULL){
                free(iter->data);
                free(iter->key);
            }
       }
    }
    return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(instache)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(instache)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(instache)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "instache support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */


/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_instache_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_instache_compiled)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	len = spprintf(&strg, 0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "instache", arg);
	RETURN_STRINGL(strg, len, 0);
}
/* }}} */
/** {{{ */
PHP_FUNCTION(instache_put)
{
	char *key = NULL;
    char *val = NULL;
	int key_len,val_len;
	char *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &key, &key_len,&val,&val_len) == FAILURE) {
		return;
	}
    int result =0;
    char * keydup =NULL;
    char * valdup = NULL;
    keydup = strdup(key);
    valdup = strdup(val);
    if(keydup == NULL || valdup == NULL){
        RETURN_FALSE;
    }
    result = hl_ht_set(instache_ht,INSTACHE_BUCKET_SIZE,keydup,valdup);
    if(result>=0){
        RETURN_TRUE;
    }
    RETURN_FALSE;
}
/** }}} */
/** {{{ */
PHP_FUNCTION(instache_get)
{
	char *key = NULL;
    char *val = NULL;
	int key_len,val_len;
	char *strg;
    hl_link * result = NULL;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &key, &key_len) == FAILURE) {
		return;
	}
    result = hl_ht_get(instache_ht,INSTACHE_BUCKET_SIZE,key);
    if(result!=NULL){
        RETURN_STRING((char * ) result->data,1);
    }else{
       RETURN_STRING("",1); 
    }
}
/** }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
