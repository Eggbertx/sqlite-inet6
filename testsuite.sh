#!/bin/bash

ANY_FAILED=0

main() {
    ANY_FAILED="0"

    testcase "inet_ntoa and inet_aton are inverse" \
        "select inet_ntoa(inet_aton('33.44.55.66'));" \
        "33.44.55.66"

    if [ "$?" != "0" ]; then ANY_FAILED="1"; fi

    testcase "inet_aton converts valid string address to number" \
        "select inet_aton('12.34.56.78');" \
        "203569230"

    if [ "$?" != "0" ]; then ANY_FAILED="1"; fi

    testcase "inet_ntoa converts valid numeric address to string" \
        "select inet_ntoa(1924046976);" \
        "114.174.160.128"

    if [ "$?" != "0" ]; then ANY_FAILED="1"; fi

    testcase "inet_ntoa ignores NULLs" \
        "select inet_ntoa(NULL);" \
        ""

    if [ "$?" != "0" ]; then ANY_FAILED="1"; fi

    testcase "inet_aton ignores NULLs" \
        "select inet_aton(NULL);" \
        ""

    if [ "$?" != "0" ]; then ANY_FAILED="1"; fi

    testcase "inet_aton returns null when fed arbitrary strings" \
        "select (inet_aton('999.999.999.999'));" \
        ""

    if [ "$?" != "0" ]; then ANY_FAILED="1"; fi

    testcase "inet_aton returns unsigned integers" \
        "select inet_aton('192.168.1.1');" \
        "3232235777"

    if [ "$?" != "0" ]; then ANY_FAILED="1"; fi

    testcase "inet_aton handles 0.0.0.0" \
        "select inet_aton('0.0.0.0');" \
        "0"

    if [ "$?" != "0" ]; then ANY_FAILED="1"; fi

    testcase "inet_aton handles 255.255.255.255" \
        "select inet_aton('255.255.255.255');" \
        "4294967295"

    if [ "$?" != "0" ]; then ANY_FAILED="1"; fi

    testcase "inet6_ntoa and inet_aton are inverse (IPv4)" \
        "select inet6_ntoa(inet_aton('33.44.55.66'));" \
        "33.44.55.66"

    if [ "$?" != "0" ]; then ANY_FAILED="1"; fi

    testcase "inet6_ntoa and inet6_aton are inverse (IPv6)" \
        "select inet6_ntoa(inet6_aton('2301::0:1'));" \
        "2301::1"

    if [ "$?" != "0" ]; then ANY_FAILED="1"; fi

    testcase "inet6_aton converts valid string address to blob" \
        "select typeof(inet6_aton('12.34.56.78'));" \
        "blob"

    if [ "$?" != "0" ]; then ANY_FAILED="1"; fi

    testcase "inet6_ntoa converts valid numeric address to string" \
        "select inet6_ntoa(1924046976);" \
        "114.174.160.128"

    if [ "$?" != "0" ]; then ANY_FAILED="1"; fi

    testcase "inet6_aton results can be compared" \
        "create table ip_addrs (ip varbinary(16));insert into ip_addrs values (inet6_aton('2607:f8b0:400a:808::200e')), (inet6_aton('2607::1')), (inet6_aton('192.168.1.1'));select inet6_ntoa(ip) from ip_addrs where ip > inet6_aton('2607:f800::');" \
        "2607:f8b0:400a:808::200e"

    if [ "$?" != "0" ]; then ANY_FAILED="1"; fi


    return $ANY_FAILED
}

testcase() {
    CASE_NAME="$1"
    CODE="$2"
    EXPECTED="$3"

    FAILED="0"

    printf "$CASE_NAME\t-\t"

    OUTPUT=$(
        sqlite3 2>&1 << EOF
.mode list
.load bin/inet6
$CODE
EOF
    )

    RC="$?"

    if [[ $EXPECTED != $OUTPUT ]]; then
        printf "FAILED\n"
        printf "\tAssertion failed. Expected: '$EXPECTED', Actual: '$OUTPUT'\n"
        FAILED="1"
    else
        printf "OK\n"
    fi

    return $FAILED
}

make clean && make
main
