/*
** 2017-05-12
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
** This SQLite extension implements inet_aton() and inet_ntoa() functions.
*/
#ifndef INET_H
#define INET_H
#include <sqlite3ext.h>
#ifndef SQLITE_EXTENSION_INIT1_DEF
#define SQLITE_EXTENSION_INIT1_DEF
SQLITE_EXTENSION_INIT1
#endif

#include <arpa/inet.h>

#include <assert.h>
#include <string.h>

static void inet_aton_impl(
    sqlite3_context *context,
    int argc,
    sqlite3_value **argv)
{
    const char *zIn;
    struct in_addr sInAddr;

    assert(argc == 1);
    if (sqlite3_value_type(argv[0]) == SQLITE_NULL)
    {
        return;
    }

    zIn = (const char*)sqlite3_value_text(argv[0]);

    if (inet_aton(zIn, &sInAddr) == 0)
    {
        return;
    }

    sqlite3_result_int64(context, ntohl(sInAddr.s_addr));
}

static void inet_ntoa_impl(
    sqlite3_context *context,
    int argc,
    sqlite3_value **argv)
{
    struct in_addr sInAddr;
    int nIn;
    char* zOut;
    int nOut = 0;

    assert(argc == 1);
    if (sqlite3_value_type(argv[0]) == SQLITE_NULL)
    {
        return;
    }

    nIn = sqlite3_value_int(argv[0]);
    sInAddr.s_addr = htonl(nIn);
    zOut = inet_ntoa(sInAddr);
    nOut = strlen(zOut);
    sqlite3_result_text(context, (char*)zOut, nOut, SQLITE_TRANSIENT);
}

#endif