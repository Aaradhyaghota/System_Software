#include <stdio.h>
#include <unistd.h>

int main() {
    //execle(): Similar to execl() but also allows you to specify a custom environment.

    // Custom environment for execle
    char *env[] = { "PATH=/bin", NULL };

    if (execle("/bin/ls", "ls", "-Rl", NULL, env) == -1) {
        printf("Error - executing execle");
        return 1;
    }

    return 0;
}
