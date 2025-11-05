/*
** 2025-11-05
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
******************************************************************************
**
** This SQLite extension implements inet_aton(), and inet_ntoa(), inet6_aton(), and inet6_ntoa() functions.
*/

#include "inet.h"
#include "inet6.h"
#include <sqlite3ext.h>
#ifndef SQLITE_EXTENSION_INIT1_DEF
#define SQLITE_EXTENSION_INIT1_DEF
SQLITE_EXTENSION_INIT1
#endif

int sqlite3_inet_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi)
{
    int rc = SQLITE_OK;

    SQLITE_EXTENSION_INIT2(pApi);
    (void)pzErrMsg;  /* Unused parameter */

    rc = sqlite3_create_function(db, "inet_aton", 1, SQLITE_UTF8, 0, inet_aton_impl, 0, 0);
    if (rc != SQLITE_OK)
    {
        return rc;
    }

    rc = sqlite3_create_function(db, "inet_ntoa", 1, SQLITE_UTF8, 0, inet_ntoa_impl, 0, 0);
    if (rc != SQLITE_OK)
    {
        return rc;
    }

    rc = sqlite3_create_function(db, "inet6_aton", 1, SQLITE_UTF8, 0, inet6_aton_impl, 0, 0);
    if (rc != SQLITE_OK)
    {
        return rc;
    }

    rc = sqlite3_create_function(db, "inet6_ntoa", 1, SQLITE_UTF8, 0, inet6_ntoa_impl, 0, 0);

    return rc;
}