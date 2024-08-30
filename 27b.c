#include <stdio.h>
#include <unistd.h>

int main (){
	//execlp()--> Takes a list of arguments and searches the PATH for the executable.
	if (execlp("ls", "ls", "-Rl", NULL) == -1) {
        	printf("Error-ececuting through execlp");
        	return 1;
   	}

    return 0;	
}
