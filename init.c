#include "inet.h"
#include "inet6.h"
#include <sqlite3ext.h>
#ifndef SQLITE_EXTENSION_INIT1_DEF
#define SQLITE_EXTENSION_INIT1_DEF
SQLITE_EXTENSION_INIT1
#endif

int sqlite3_inet_init(
    sqlite3 *db,
    char **pzErrMsg,
    const sqlite3_api_routines *pApi)
{
    int rc = SQLITE_OK;

    SQLITE_EXTENSION_INIT2(pApi);
    (void)pzErrMsg;  /* Unused parameter */

    rc = sqlite3_create_function(
        db,
        "inet_aton",
        1,
        SQLITE_UTF8,
        0,
        inet_aton_impl,
        0,
        0);

    if (rc != SQLITE_OK) {
        return rc;
    }

    rc = sqlite3_create_function(
        db,
        "inet_ntoa",
        1,
        SQLITE_UTF8,
        0,
        inet_ntoa_impl,
        0,
        0);

    rc = sqlite3_create_function(
        db,
        "inet6_aton",
        1,
        SQLITE_UTF8,
        0,
        inet6_aton_impl,
        0,
        0);

    if (rc != SQLITE_OK) {
        return rc;
    }

    rc = sqlite3_create_function(
        db,
        "inet6_ntoa",
        1,
        SQLITE_UTF8,
        0,
        inet6_ntoa_impl,
        0,
        0);

    return rc;
}