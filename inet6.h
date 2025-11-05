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

#ifndef INET6_H
#define INET6_H
#include <sqlite3ext.h>
#ifndef SQLITE_EXTENSION_INIT1_DEF
#define SQLITE_EXTENSION_INIT1_DEF
SQLITE_EXTENSION_INIT1
#endif

#include <arpa/inet.h>

#include <assert.h>
#include <string.h>
#include "inet.h"

static void inet6_aton_impl(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    const char *zIn;
    struct in6_addr ipv6;
    struct in_addr ipv4;
    unsigned char ipBytes[16] = {0};

    assert(argc == 1);

    if (sqlite3_value_type(argv[0]) == SQLITE_NULL)
    {
        return;
    }

    zIn = (const char*)sqlite3_value_text(argv[0]);
    if (inet_pton(AF_INET6, zIn, &ipv6) == 1)
    {
        /* recognized as IPv6 */
        memcpy(ipBytes, ipv6.s6_addr, 16);
    }
    else if (inet_pton(AF_INET, zIn, &ipv4))
    {
        /* recognized as IPv4 */
        memcpy(ipBytes + 12, &ipv4, 4);
    }
    else
    {
        /* assumed to be invalid IP */
        sqlite3_result_null(context);
        return;
    }
    sqlite3_result_blob(context, ipBytes, 16, SQLITE_TRANSIENT);
}

static void inet6_ntoa_impl(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    char zOut[INET6_ADDRSTRLEN] = {0};
    const unsigned char* blob;
    int nIn;
    int nOut;
    int blobLength;
    struct in6_addr ipv6;
    struct in_addr ipv4;
    int vType;

    assert(argc == 1);
    vType = sqlite3_value_type(argv[0]);
    if (vType == SQLITE_NULL)
    {
        return;
    }
    else if (vType == SQLITE_INTEGER)
    {
        nIn = sqlite3_value_int(argv[0]);
        ipv4.s_addr = htonl(nIn);
        blob = (const unsigned char*)inet_ntoa(ipv4);
        nOut = strlen((const char*)blob);
        sqlite3_result_text(context, (char*)blob, nOut, SQLITE_TRANSIENT);
        return;
    }

    blob = sqlite3_value_blob(argv[0]);
    blobLength = sqlite3_value_bytes(argv[0]);
    if (blobLength == 16)
    {
        memcpy(&ipv6, blob, 16);
        if (IN6_IS_ADDR_V4MAPPED(&ipv6) || IN6_IS_ADDR_V4COMPAT(&ipv6))
        {
            /* argument is an IPv4 number */
            memcpy(&ipv4, blob + 12, 4);
            inet_ntop(AF_INET, &ipv4, zOut, sizeof(zOut));
        }
        else
        {
            /* argument is an IPv6 number */
            inet_ntop(AF_INET6, &ipv6, zOut, sizeof(zOut));
        }
        sqlite3_result_text(context, zOut, -1, SQLITE_TRANSIENT);
    }
    else
    {
        /* not a valid IP number */
        sqlite3_result_null(context);
    }
}

#endif