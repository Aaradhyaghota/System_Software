/*
========================================================================================================

Question No.: 27c
Author: Aaradhya Ghota
Description: Write a program to execute ls -Rl by the following system calls
        c. execle
Date: 30th August, 2024

========================================================================================================
*/
#include <stdio.h>
#include <unistd.h>

int main() {
    //execle(): Similar to execl() but also allows you to specify a custom environment.

    // Custom environment for execle
    extern char** environ;

    if (execle("/bin/ls", "ls", "-Rl", NULL, environ) == -1) {
        printf("Error - executing execle");
        return 1;
    }

    return 0;
}


/*
========================================================================================================
Output:

./a.out
.:
total 328
-rw-rw-r-- 1 aaradhya-ghota aaradhya-ghota  2063 Aug 30 19:36 10.c
-rw-r--r-- 1 aaradhya-ghota aaradhya-ghota    30 Aug 30 19:30 10.txt
-rw-rw-r-- 1 aaradhya-ghota aaradhya-ghota  2137 Aug 30 19:43 11.c
-rw-r--r-- 1 aaradhya-ghota aaradhya-ghota    64 Aug 30 19:41 11_file1.txt
-rw-r--r-- 1 aaradhya-ghota aaradhya-ghota    66 Aug 30 19:41 11_file2.txt
-rw-r--r-- 1 aaradhya-ghota aaradhya-ghota    68 Aug 30 19:41 11_file3.txt
-rw-rw-r-- 1 aaradhya-ghota aaradhya-ghota  1384 Aug 30 19:48 12.c
-rw-rw-r-- 1 aaradhya-ghota aaradhya-ghota  1438 Aug 30 19:54 13.c
-rw-rw-r-- 1 aaradhya-ghota aaradhya-ghota  1902 Aug 30 19:57 14.c
-rw-rw-r-- 1 aaradhya-ghota aaradhya-ghota  4180 Aug 30 20:00 15.c
-rw-rw-r-- 1 aaradhya-ghota aaradhya-ghota  1314 Aug 30 20:17 16a.c
-rw-rw-r-- 1 aaradhya-ghota aaradhya-ghota  1468 Aug 30 20:16 16b.c
-rwxr--r-- 1 aaradhya-ghota aaradhya-ghota     4 Aug 29 18:13 16db
-rwxrwxr-x 1 aaradhya-ghota aaradhya-ghota 16176 Aug 30 20:14 16reader
-rwxrwxr-x 1 aaradhya-ghota aaradhya-ghota 16176 Aug 30 20:14 16writer
-rw-rw-r-- 1 aaradhya-ghota aaradhya-ghota  1185 Aug 30 20:20 17a.c
-rw-rw-r-- 1 aaradhya-ghota aaradhya-ghota  2522 Aug 30 20:27 17b.c
-rwxr--r-- 1 aaradhya-ghota aaradhya-ghota     4 Aug 30 20:26 17db.txt
-rwxrwxr-x 1 aaradhya-ghota aaradhya-ghota 16344 Aug 30 20:22 17upd_tk
-rw-rw-r-- 1 aaradhya-ghota aaradhya-ghota  1135 Aug 30 20:33 19.c
-rw-rw-r-- 1 aaradhya-ghota aaradhya-ghota   876 Aug 30 18:14 1a.c
-rw-rw-r-- 2 aaradhya-ghota aaradhya-ghota  1292 Aug 30 18:14 1b.c
-rw-rw-r-- 1 aaradhya-ghota aaradhya-ghota  1118 Aug 30 18:07 1c.c
prw-rw-r-- 1 aaradhya-ghota aaradhya-ghota     0 Aug 30 18:06 1cfifo
-rw-rw-r-- 1 aaradhya-ghota aaradhya-ghota  1952 Aug 30 20:37 20.c
-rwxrwxr-x 1 aaradhya-ghota aaradhya-ghota 16184 Aug 30 12:46 20priority
-rw-rw-r-- 1 aaradhya-ghota aaradhya-ghota  1070 Aug 30 20:42 21.c
-rw-rw-r-- 1 aaradhya-ghota aaradhya-ghota  1357 Aug 30 20:45 22.c
-rw-r--r-- 1 aaradhya-ghota aaradhya-ghota   114 Aug 30 20:46 22.txt
-rw-rw-r-- 1 aaradhya-ghota aaradhya-ghota  2051 Aug 30 20:56 23.c
-rw-rw-r-- 1 aaradhya-ghota aaradhya-ghota  1115 Aug 30 21:00 24.c
-rw-rw-r-- 1 aaradhya-ghota aaradhya-ghota  1637 Aug 30 21:06 25.c
-rw-rw-r-- 1 aaradhya-ghota aaradhya-ghota  1215 Aug 30 22:08 26a.c
-rw-rw-r-- 1 aaradhya-ghota aaradhya-ghota  1313 Aug 30 22:23 26b.c
-rwxrwxr-x 1 aaradhya-ghota aaradhya-ghota 16040 Aug 30 22:19 26exe
-rwxrwxr-- 1 aaradhya-ghota aaradhya-ghota     0 Aug 30 22:21 26.txt
-rw-rw-r-- 1 aaradhya-ghota aaradhya-ghota  5449 Aug 30 22:25 27a.c
-rw-rw-r-- 1 aaradhya-ghota aaradhya-ghota  5580 Aug 30 22:27 27b.c
-rw-rw-r-- 1 aaradhya-ghota aaradhya-ghota   729 Aug 30 22:29 27c.c
-rw-rw-r-- 1 aaradhya-ghota aaradhya-ghota   343 Aug 30 15:32 27d.c
-rw-rw-r-- 1 aaradhya-ghota aaradhya-ghota   337 Aug 30 15:34 27e.c
-rw-rw-r-- 1 aaradhya-ghota aaradhya-ghota  1011 Aug 30 15:55 28.c
-rw-rw-r-- 1 aaradhya-ghota aaradhya-ghota  1576 Aug 30 16:38 29.c
-rw-rw-r-- 1 aaradhya-ghota aaradhya-ghota  2180 Aug 30 18:21 2.c
-rw-rw-r-- 1 aaradhya-ghota aaradhya-ghota   799 Aug 30 17:50 30.c
-rw-rw-r-- 1 aaradhya-ghota aaradhya-ghota   954 Aug 30 18:24 3.c
-rw-rw-r-- 1 aaradhya-ghota aaradhya-ghota   886 Aug 30 18:31 4.c
-rw-rw-r-- 1 aaradhya-ghota aaradhya-ghota  2100 Aug 30 18:45 5.c
-rw-rw-r-- 1 aaradhya-ghota aaradhya-ghota  1332 Aug 30 18:50 6.c
-rw-rw-r-- 1 aaradhya-ghota aaradhya-ghota  2582 Aug 30 19:21 7.c
-rwxrwxr-x 1 aaradhya-ghota aaradhya-ghota   347 Aug 13 23:44 7_file1.txt
-rwxrwxr-x 1 aaradhya-ghota aaradhya-ghota   347 Aug 30 18:56 7_file2.txt
-rw-rw-r-- 1 aaradhya-ghota aaradhya-ghota  2341 Aug 30 19:24 8.c
-rw-rw-r-- 1 aaradhya-ghota aaradhya-ghota   510 Aug 28 18:31 8.txt
-rw-rw-r-- 1 aaradhya-ghota aaradhya-ghota  2103 Aug 30 19:28 9.c
-rwxrwxr-x 1 aaradhya-ghota aaradhya-ghota 16072 Aug 30 22:29 a.out
-rw-rw-r-- 1 aaradhya-ghota aaradhya-ghota     0 Aug  8 09:24 file
-rw-rw-r-- 1 aaradhya-ghota aaradhya-ghota    45 Aug 30 11:29 inf.c
lrwxrwxrwx 1 aaradhya-ghota aaradhya-ghota     4 Aug 30 18:15 linkto1a -> 1a.c
-rw-rw-r-- 2 aaradhya-ghota aaradhya-ghota  1292 Aug 30 18:14 linkto1b
-rwxrwxr-x 1 aaradhya-ghota aaradhya-ghota     0 Aug 30 18:30 nwfile
-rwxrwxr-- 1 aaradhya-ghota aaradhya-ghota     0 Aug 30 18:43 nwfile_1
-rwxrwxr-- 1 aaradhya-ghota aaradhya-ghota     0 Aug 30 18:43 nwfile_2
-rwxrwxr-- 1 aaradhya-ghota aaradhya-ghota     0 Aug 30 18:43 nwfile_3
-rwxrwxr-- 1 aaradhya-ghota aaradhya-ghota     0 Aug 30 18:43 nwfile_4
-rwxrwxr-- 1 aaradhya-ghota aaradhya-ghota     0 Aug 30 18:43 nwfile_5
-rwxrwxr-x 1 aaradhya-ghota aaradhya-ghota 15968 Aug 30 13:09 print_name
-rw-rw-r-- 1 aaradhya-ghota aaradhya-ghota   174 Aug 30 13:07 printName.c

========================================================================================================
*/
