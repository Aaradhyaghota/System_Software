#include <stdio.h>
#include <unistd.h>

int main() {
	//execvp(): Takes an array of arguments and searches the PATH for the executable.

    	// Argument array
    	char *args[] = { "ls", "-Rl", NULL };

    	if (execvp("ls", args) == -1) {
        	printf("Error - executing execvp() call");
        	return 1;
    	}

    	return 0;
}
