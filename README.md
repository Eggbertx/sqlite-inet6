# sqlite-inet

This SQLite extension provides `inet_aton()`, `inet_ntoa`, `inet6_aton()`,
and `inet6_ntoa()` functions for converting IP addresses to and from strings.
The function `inet_ntoa` accepts integers as input, but `inet6_ntoa` does not.

## Usage

```
$ make
$ sqlite3
sqlite> .load bin/inet6
sqlite> select inet_ntoa(3232235777);
192.168.1.1
sqlite> select inet6_aton('192.168.1.3') > inet6_aton('192.168.1.1');
1
sqlite> select inet6_aton('192.168.1.3') < inet6_aton('192.168.1.1');
0
sqlite> create table ip_addrs (id integer primary key, ip varbinary(16));
sqlite> insert into ip_addrs values (1, inet6_aton('2607:f8b0:400a:808::200e'));
sqlite> insert into ip_addrs values (2, inet6_aton('2607::1'));
sqlite> insert into ip_addrs values (3, inet6_aton('192.168.1.1'));
sqlite> select inet6_ntoa(ip) from ip_addrs where ip > inet6_aton('2607:f800::');
2607:f8b0:400a:808::200e
```


## Misc

The functions behave like their MariaDB analogs, that is,
they expect integers in host byte order,
rather than network byte order as expected by [C functions](https://linux.die.net/man/3/inet_ntoa).
For more info, see [this bug report](https://bugs.mysql.com/bug.php?id=34030#c197546).

Like their MariaDB analogs, when invoked with NULL or incorrect data (invalid IP string or incorrect type), NULL is returned.