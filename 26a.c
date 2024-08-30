#include<stdio.h>
#include <unistd.h>

int main (){
	printf("Executing executable program\n");

    	if (execl("./20priority","./20priority", NULL) == -1) {
        	printf("Error - in execl fnction");
        	return 1;
    	}
	
	printf("This will be printed if execl doesn't run executable file\n");
	
	return 0;
}
