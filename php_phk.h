/*
  +----------------------------------------------------------------------+
  | PHK accelerator extension <http://phk.tekwire.net>                   |
  +----------------------------------------------------------------------+
  | Copyright (c) 2005-2007 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt.                                 |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Francois Laupretre <francois@tekwire.net>                    |
  +----------------------------------------------------------------------+
*/

/* #define PHK_DEBUG */

#ifndef __PHP_PHK_H
#define __PHP_PHK_H

/*============================================================================*/

#ifdef PHK_DEBUG
#define UT_DEBUG
#endif

#ifdef HAVE_STDARG_H
#include <stdarg.h>
#endif

#ifdef HAVE_STDLIB_H
#	include <stdlib.h>
#endif

#include <stdio.h>

#ifdef HAVE_SYS_TYPES_H
#	include <sys/types.h>
#endif

#ifdef HAVE_SYS_STAT_H
#	include <sys/stat.h>
#endif

#if HAVE_STRING_H
#	include <string.h>
#endif

#if HAVE_UNISTD_H
#	include <unistd.h>
#endif

#ifndef S_ISDIR
#	define S_ISDIR(mode)	(((mode)&S_IFMT) == S_IFDIR)
#endif
#ifndef S_ISREG
#	define S_ISREG(mode)	(((mode)&S_IFMT) == S_IFREG)
#endif

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "SAPI.h"
#include "TSRM/TSRM.h"
#include "ext/standard/basic_functions.h"
#include "ext/standard/info.h"
#include "ext/standard/php_string.h"
#include "ext/standard/php_versioning.h"
#include "ext/standard/url.h"
#include "php_ini.h"
#include "php_streams.h"
#include "php_variables.h"
#include "php_version.h"
#include "streams/php_streams_int.h"
#include "zend_constants.h"
#include "zend_execute.h"
#include "zend_errors.h"
#include "zend_API.h"
#include "zend_alloc.h"
#include "zend_compile.h"
#include "zend_extensions.h"
#include "zend_hash.h"
#include "zend_objects_API.h"
#include "zend_operators.h"
#include "TSRM/tsrm_virtual_cwd.h"

#include "utils.h"

#include "Automap_Handlers.h"
#include "Automap_Instance.h"
#include "Automap_Key.h"
#include "Automap_Loader.h"
#include "Automap_Pmap.h"
#include "Automap_Mnt.h"
#include "Automap_Type.h"
#include "Automap_Util.h"
#include "PHK_Cache.h"
#include "PHK_Stream.h"
#include "PHK_Mgr.h"
#include "PHK.h"

/*---------------------------------------------------------------*/

#define PHP_PHK_VERSION "2.1.0" /* The extension version */

#define AUTOMAP_RUNTIME_VERSION "2.1.0"

#define AUTOMAP_API "2.1.0"

#define AUTOMAP_MIN_MAP_VERSION "1.1.0"

#define PHK_ACCEL_VERSION "2.1.0"

#define PHK_ACCEL_MIN_VERSION "2.1.0"

#define PHP_PHK_EXTNAME "phk"

zend_module_entry phk_module_entry;

#define phpext_phk_ptr &phk_module_entry

/*---------------------------------------------------------------*/

static DECLARE_CZVAL(false);
static DECLARE_CZVAL(true);
static DECLARE_CZVAL(null);

static DECLARE_CZVAL(Automap);
static DECLARE_CZVAL(spl_autoload_register);
static DECLARE_CZVAL(Automap__autoload_hook);

/* Hash keys */

static DECLARE_HKEY(map);
static DECLARE_HKEY(options);
static DECLARE_HKEY(automap);
static DECLARE_HKEY(mp_property_name);

static DECLARE_HKEY(no_cache);
static DECLARE_HKEY(no_opcode_cache);
static DECLARE_HKEY(required_extensions);
static DECLARE_HKEY(map_defined);
static DECLARE_HKEY(mount_script);
static DECLARE_HKEY(umount_script);
static DECLARE_HKEY(plugin_class);
static DECLARE_HKEY(web_access);
static DECLARE_HKEY(min_php_version);
static DECLARE_HKEY(max_php_version);
static DECLARE_HKEY(mime_types);
static DECLARE_HKEY(web_run_script);
static DECLARE_HKEY(mp_property_name);
static DECLARE_HKEY(web_main_redirect);
static DECLARE_HKEY(_PHK_path);
static DECLARE_HKEY(ORIG_PATH_INFO);
static DECLARE_HKEY(phk_backend);
static DECLARE_HKEY(lib_run_script);
static DECLARE_HKEY(cli_run_script);
static DECLARE_HKEY(auto_umount);
static DECLARE_HKEY(argc);
static DECLARE_HKEY(argv);
static DECLARE_HKEY(automap);
static DECLARE_HKEY(phk_stream_backend);
static DECLARE_HKEY(eaccelerator_get);
static DECLARE_HKEY(phk);

/*============================================================================*/

ZEND_BEGIN_MODULE_GLOBALS(phk)

/*-- Automap --*/

HashTable mnttab;
Automap_Mnt **mount_order; /* Array of (Automap_Mnt *)|NULL */
int mcount;						/* Size of the mount_order table */

zval **failure_handlers;
int fh_count;					/* Failure handler count */

zval **success_handlers;
int sh_count;					/* Success handler count */

/*-- PHK --*/

HashTable *mtab;		/* PHK_Mgr - Null until initialized */
PHK_Mnt  **mount_order;	/* Array of (PHK_Mnt *)|NULL */
int mcount;				/* Size of the mount_order table */

zval caching;			/* PHK_Mgr - Can be null/true/false */

char root_package[UT_PATH_MAX + 1];

int php_runtime_is_loaded;

zval *mime_table;

ZEND_END_MODULE_GLOBALS(phk)

#ifdef ZTS
#	define PHK_G(v) TSRMG(phk_globals_id, zend_phk_globals *, v)
#else
#	define PHK_G(v) (phk_globals.v)
#endif

/*---------------------------------------------------------------*/

/* We need a private property here, so that it cannot be accessed nor
   modified by a malicious PHP script */

#define MP_PROPERTY_NAME "\0\0m"

/*============================================================================*/
#endif /* __PHP_PHK_H */