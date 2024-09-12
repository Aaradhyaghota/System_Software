/*
========================================================================================================

Question No: 5
Author: Aaradhya Ghota
Description: Write a program to create five new files with infinite loop. Execute the program in the background and check the file descriptor table at /proc/pid/fd.
Date: 16th August, 2024

========================================================================================================
*/


#include <stdio.h>
#include <fcntl.h>
#include <stdbool.h>

int main (){
	int cnt = 1;
	bool flag = true;
	while(1){
	    if(flag){
		    creat("nwfile_1",0774); printf("File-1 created\n"); 
		    creat("nwfile_2",0774); printf("File-2 created\n");
		    creat("nwfile_3",0774); printf("File-3 created\n");
		    creat("nwfile_4",0774); printf("File-4 created\n");
		    creat("nwfile_5",0774); printf("File-5 created\n");
		    flag = false;
	    }
	}
return 0;
}

/*
========================================================================================================
Output:

./a.out & ..to get pid
[1] 16074

ls -la  /proc/16074/fd

total 0
dr-x------ 2 aaradhya-ghota aaradhya-ghota  8 Aug 30 18:44 .
dr-xr-xr-x 9 aaradhya-ghota aaradhya-ghota  0 Aug 30 18:44 ..
lrwx------ 1 aaradhya-ghota aaradhya-ghota 64 Aug 30 18:44 0 -> /dev/pts/0
lrwx------ 1 aaradhya-ghota aaradhya-ghota 64 Aug 30 18:44 1 -> /dev/pts/0
lrwx------ 1 aaradhya-ghota aaradhya-ghota 64 Aug 30 18:44 2 -> /dev/pts/0
l-wx------ 1 aaradhya-ghota aaradhya-ghota 64 Aug 30 18:44 3 -> /home/aaradhya-ghota/System_Software/nwfile_1
l-wx------ 1 aaradhya-ghota aaradhya-ghota 64 Aug 30 18:44 4 -> /home/aaradhya-ghota/System_Software/nwfile_2
l-wx------ 1 aaradhya-ghota aaradhya-ghota 64 Aug 30 18:44 5 -> /home/aaradhya-ghota/System_Software/nwfile_3
l-wx------ 1 aaradhya-ghota aaradhya-ghota 64 Aug 30 18:44 6 -> /home/aaradhya-ghota/System_Software/nwfile_4
l-wx------ 1 aaradhya-ghota aaradhya-ghota 64 Aug 30 18:44 7 -> /home/aaradhya-ghota/System_Software/nwfile_5





========================================================================================================
*/
