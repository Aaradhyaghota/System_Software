#include <stdio.h>
#include <unistd.h>

int main() {
	//execv(): Takes an array of arguments and requires the full path to the executable.
	
    	// Argument array
    	char *args[] = { "ls", "-Rl", NULL };

    	if (execv("/bin/ls", args) == -1) {
        	printf("Error -Executing execv() call");
        	return 1;
    	}

    	return 0;
}
