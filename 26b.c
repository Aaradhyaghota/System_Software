/*
========================================================================================================
Question No. 26b
Author: Aaradhya Ghota
Description: Write a program to execute an executable program.
	a. use some executable program
	b. pass some input to an executable program. (for example execute an executable of $./a.out name)
Date: 30th August, 2024

========================================================================================================
*/

#include<stdio.h>
#include <unistd.h>

int main (int argc, char ** argv){
        if(argc !=2)
        {
          printf("Error- INvalid input");
          return 1;
        }
        printf("Executing executable program\n");

        if (execl("/home/aaradhya-ghota/System_Software/26exe","./26exe",argv[1], NULL) == -1) {
                printf("Error - in execl fnction");
                return 1;
        }

        printf("This will be printed if execl doesn't run executable file\n");

        return 0;

} 

/*
========================================================================================================
Output:

gcc -o 26exe 3.c

./a.out 26.txt
Executing executable program
Succesful
File discripter value is 3 

========================================================================================================
*/
