
/*
========================================================================================================
Name : 19.a, 19.b, 19.c
Author : Aaradhya Ghota
Description : Create a FIFO file by 
a. mknod command
b. mkfifo command
c. use strace command to find out, which command (mknod or mkfifo) is better.
Date: 14 Sept, 2024.
========================================================================================================
*/

// Using commands
//a. mknod --mode=0666 19_mknod_command p
//b. mkfifo 19_mkfifo_command --mode=0666

//c. strace mknod myfifo p
//   strace mkfifo myfifo


// Which is faster (better)?
// Answer: mkfifo calls mknod internally (see strace) => mknod is faster.
// Library functions may internally have mappings which may make them faster than system calls in some cases.
//mknod is a general-purpose system call that can create various types of special files (e.g., FIFOs, block devices, character devices).
//mkfifo is more specialized and directly intended for creating FIFOs. 
//Internally, mkfifo calls the mknod system call with the appropriate flags, so it’s a higher-level and more user-friendly function.


/*
========================================================================================================
Output:
========================================================================================================
*/

