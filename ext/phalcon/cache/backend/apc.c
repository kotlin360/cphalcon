
#ifdef HAVE_CONFIG_H
#include "../../../ext_config.h"
#endif

#include <php.h>
#include "../../../php_ext.h"
#include "../../../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/concat.h"
#include "kernel/fcall.h"
#include "kernel/operators.h"
#include "kernel/exception.h"
#include "kernel/hash.h"
#include "kernel/array.h"


/*
 +------------------------------------------------------------------------+
 | Phalcon Framework                                                      |
 +------------------------------------------------------------------------+
 | Copyright (c) 2011-2014 Phalcon Team (http://www.phalconphp.com)       |
 +------------------------------------------------------------------------+
 | This source file is subject to the New BSD License that is bundled     |
 | with this package in the file docs/LICENSE.txt.                        |
 |                                                                        |
 | If you did not receive a copy of the license and are unable to         |
 | obtain it through the world-wide-web, please send an email             |
 | to license@phalconphp.com so we can send you a copy immediately.       |
 +------------------------------------------------------------------------+
 | Authors: Andres Gutierrez <andres@phalconphp.com>                      |
 |          Eduar Carvajal <eduar@phalconphp.com>                         |
 +------------------------------------------------------------------------+
 */
/**
 * Phalcon\Cache\Backend\Apc
 *
 * Allows to cache output fragments, PHP data and raw data using an APC backend
 *
 *<code>
 *	//Cache data for 2 days
 *	$frontCache = new \Phalcon\Cache\Frontend\Data(array(
 *		'lifetime' => 172800
 *	));
 *
 *  $cache = new \Phalcon\Cache\Backend\Apc($frontCache, array(
 *      'prefix' => 'app-data'
 *  ));
 *
 *	//Cache arbitrary data
 *	$cache->save('my-data', array(1, 2, 3, 4, 5));
 *
 *	//Get data
 *	$data = $cache->get('my-data');
 *
 *</code>
 */
ZEPHIR_INIT_CLASS(Phalcon_Cache_Backend_Apc) {

	ZEPHIR_REGISTER_CLASS_EX(Phalcon\\Cache\\Backend, Apc, phalcon, cache_backend_apc, phalcon_cache_backend_ce, phalcon_cache_backend_apc_method_entry, 0);

	zend_class_implements(phalcon_cache_backend_apc_ce TSRMLS_CC, 1, phalcon_cache_backendinterface_ce);
	return SUCCESS;

}

/**
 * Returns a cached content
 *
 * @param 	string|long keyName
 * @param   long lifetime
 * @return  mixed
 */
PHP_METHOD(Phalcon_Cache_Backend_Apc, get) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *keyName, *lifetime = NULL, *frontend, *prefixedKey, *cachedContent = NULL, *_0;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &keyName, &lifetime);

	if (!lifetime) {
		lifetime = ZEPHIR_GLOBAL(global_null);
	}


	ZEPHIR_OBS_VAR(frontend);
	zephir_read_property_this(&frontend, this_ptr, SL("_frontend"), PH_NOISY_CC);
	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_prefix"), PH_NOISY_CC);
	ZEPHIR_INIT_VAR(prefixedKey);
	ZEPHIR_CONCAT_SVV(prefixedKey, "_PHCA", _0, keyName);
	zephir_update_property_this(this_ptr, SL("_lastKey"), prefixedKey TSRMLS_CC);
	ZEPHIR_CALL_FUNCTION(&cachedContent, "apc_fetch", NULL, prefixedKey);
	zephir_check_call_status();
	if (ZEPHIR_IS_FALSE(cachedContent)) {
		RETURN_MM_NULL();
	}
	ZEPHIR_RETURN_CALL_METHOD(frontend, "afterretrieve", NULL, cachedContent);
	zephir_check_call_status();
	RETURN_MM();

}

/**
 * Stores cached content into the APC backend and stops the frontend
 *
 * @param string|long keyName
 * @param string content
 * @param long lifetime
 * @param boolean stopBuffer
 */
PHP_METHOD(Phalcon_Cache_Backend_Apc, save) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *keyName = NULL, *content = NULL, *lifetime = NULL, *stopBuffer = NULL, *lastKey, *frontend, *cachedContent = NULL, *preparedContent = NULL, *ttl = NULL, *isBuffering = NULL, *_0;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 4, &keyName, &content, &lifetime, &stopBuffer);

	if (!keyName) {
		keyName = ZEPHIR_GLOBAL(global_null);
	}
	if (!content) {
		content = ZEPHIR_GLOBAL(global_null);
	}
	if (!lifetime) {
		ZEPHIR_CPY_WRT(lifetime, ZEPHIR_GLOBAL(global_null));
	} else {
		ZEPHIR_SEPARATE_PARAM(lifetime);
	}
	if (!stopBuffer) {
		stopBuffer = ZEPHIR_GLOBAL(global_true);
	}


	ZEPHIR_INIT_VAR(lastKey);
	if (Z_TYPE_P(keyName) == IS_NULL) {
		zephir_read_property_this(&lastKey, this_ptr, SL("_lastKey"), PH_NOISY_CC);
	} else {
		_0 = zephir_fetch_nproperty_this(this_ptr, SL("_prefix"), PH_NOISY_CC);
		ZEPHIR_CONCAT_SVV(lastKey, "_PHCA", _0, keyName);
	}
	if (!(zephir_is_true(lastKey))) {
		ZEPHIR_THROW_EXCEPTION_DEBUG_STR(phalcon_cache_exception_ce, "The cache must be started first", "phalcon/cache/backend/apc.zep", 92);
		return;
	}
	ZEPHIR_OBS_VAR(frontend);
	zephir_read_property_this(&frontend, this_ptr, SL("_frontend"), PH_NOISY_CC);
	if (Z_TYPE_P(content) == IS_NULL) {
		ZEPHIR_CALL_METHOD(&cachedContent, frontend, "getcontent",  NULL);
		zephir_check_call_status();
	} else {
		ZEPHIR_CPY_WRT(cachedContent, content);
	}
	ZEPHIR_CALL_METHOD(&preparedContent, frontend, "beforestore", NULL, cachedContent);
	zephir_check_call_status();
	if (Z_TYPE_P(lifetime) == IS_NULL) {
		ZEPHIR_OBS_NVAR(lifetime);
		zephir_read_property_this(&lifetime, this_ptr, SL("_lastLifetime"), PH_NOISY_CC);
		if (Z_TYPE_P(lifetime) == IS_NULL) {
			ZEPHIR_CALL_METHOD(&ttl, frontend, "getlifetime",  NULL);
			zephir_check_call_status();
		} else {
			ZEPHIR_CPY_WRT(ttl, lifetime);
		}
	} else {
		ZEPHIR_CPY_WRT(ttl, lifetime);
	}
	ZEPHIR_CALL_FUNCTION(NULL, "apc_store", NULL, lastKey, preparedContent, ttl);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(&isBuffering, frontend, "isbuffering",  NULL);
	zephir_check_call_status();
	if (ZEPHIR_IS_TRUE(stopBuffer)) {
		ZEPHIR_CALL_METHOD(NULL, frontend, "stop", NULL);
		zephir_check_call_status();
	}
	if (ZEPHIR_IS_TRUE(isBuffering)) {
		zend_print_zval(cachedContent, 0);
	}
	zephir_update_property_this(this_ptr, SL("_started"), (0) ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

/**
 * Deletes a value from the cache by its key
 *
 * @param string keyName
 * @return boolean
 */
PHP_METHOD(Phalcon_Cache_Backend_Apc, delete) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *keyName_param = NULL, *_0, *_1;
	zval *keyName = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &keyName_param);

	zephir_get_strval(keyName, keyName_param);


	_0 = zephir_fetch_nproperty_this(this_ptr, SL("_prefix"), PH_NOISY_CC);
	ZEPHIR_INIT_VAR(_1);
	ZEPHIR_CONCAT_SVV(_1, "_PHCA", _0, keyName);
	ZEPHIR_RETURN_CALL_FUNCTION("apc_delete", NULL, _1);
	zephir_check_call_status();
	RETURN_MM();

}

/**
 * Query the existing cached keys
 *
 * @param string prefix
 * @return array
 */
PHP_METHOD(Phalcon_Cache_Backend_Apc, queryKeys) {

	zephir_nts_static zephir_fcall_cache_entry *_9 = NULL;
	HashTable *_5;
	HashPosition _4;
	int ZEPHIR_LAST_CALL_STATUS;
	zend_class_entry *_1;
	zval *prefix_param = NULL, *prefixPattern = NULL, *apc, *keys, *key = NULL, *item = NULL, *_2, *_3 = NULL, **_6, _7 = zval_used_for_init, *_8 = NULL;
	zval *prefix = NULL, *_0;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &prefix_param);

	if (!prefix_param) {
		ZEPHIR_INIT_VAR(prefix);
		ZVAL_EMPTY_STRING(prefix);
	} else {
		zephir_get_strval(prefix, prefix_param);
	}


	ZEPHIR_INIT_VAR(prefixPattern);
	if (!(prefix && Z_STRLEN_P(prefix))) {
		ZVAL_STRING(prefixPattern, "/^_PHCA/", 1);
	} else {
		ZEPHIR_INIT_VAR(_0);
		ZEPHIR_CONCAT_SV(_0, "/^_PHCA/", prefix);
		ZEPHIR_CPY_WRT(prefixPattern, _0);
	}
	ZEPHIR_INIT_VAR(keys);
	array_init(keys);
	ZEPHIR_INIT_VAR(apc);
	_1 = zend_fetch_class(SL("APCIterator"), ZEND_FETCH_CLASS_AUTO TSRMLS_CC);
	object_init_ex(apc, _1);
	if (zephir_has_constructor(apc TSRMLS_CC)) {
		ZEPHIR_INIT_VAR(_2);
		ZVAL_STRING(_2, "user", 0);
		ZEPHIR_CALL_METHOD(NULL, apc, "__construct", NULL, _2, prefixPattern);
		zephir_check_temp_parameter(_2);
		zephir_check_call_status();
	}
	ZEPHIR_CALL_FUNCTION(&_3, "iterator", NULL, apc);
	zephir_check_call_status();
	zephir_is_iterable(_3, &_5, &_4, 0, 0);
	for (
	  ; zephir_hash_get_current_data_ex(_5, (void**) &_6, &_4) == SUCCESS
	  ; zephir_hash_move_forward_ex(_5, &_4)
	) {
		ZEPHIR_GET_HMKEY(key, _5, _4);
		ZEPHIR_GET_HVALUE(item, _6);
		ZEPHIR_SINIT_NVAR(_7);
		ZVAL_LONG(&_7, 5);
		ZEPHIR_CALL_FUNCTION(&_8, "substr", &_9, key, &_7);
		zephir_check_call_status();
		zephir_array_append(&keys, _8, PH_SEPARATE);
	}
	RETURN_CCTOR(keys);

}

/**
 * Checks if cache exists and it hasn't expired
 *
 * @param  string|long keyName
 * @param  long lifetime
 * @return boolean
 */
PHP_METHOD(Phalcon_Cache_Backend_Apc, exists) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *keyName = NULL, *lifetime = NULL, *lastKey, *_0, *_1 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 2, &keyName, &lifetime);

	if (!keyName) {
		keyName = ZEPHIR_GLOBAL(global_null);
	}
	if (!lifetime) {
		lifetime = ZEPHIR_GLOBAL(global_null);
	}


	ZEPHIR_INIT_VAR(lastKey);
	if (Z_TYPE_P(keyName) == IS_NULL) {
		zephir_read_property_this(&lastKey, this_ptr, SL("_lastKey"), PH_NOISY_CC);
	} else {
		_0 = zephir_fetch_nproperty_this(this_ptr, SL("_prefix"), PH_NOISY_CC);
		ZEPHIR_CONCAT_SVV(lastKey, "_PHCA", _0, keyName);
	}
	if (zephir_is_true(lastKey)) {
		ZEPHIR_CALL_FUNCTION(&_1, "apc_exists", NULL, lastKey);
		zephir_check_call_status();
		if (!ZEPHIR_IS_FALSE(_1)) {
			RETURN_MM_BOOL(1);
		}
	}
	RETURN_MM_BOOL(0);

}
