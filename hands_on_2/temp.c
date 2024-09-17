#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

int sigint_count = 0;

void handle_sigint(int sig) {
    sigint_count++;
    printf("Caught SIGINT %d times\n", sigint_count);

    if (sigint_count >= 3) {
        printf("Exiting after 3 SIGINT signals\n");
        exit(0);
    }
}

int main() {
    // Register the SIGINT signal handler
    signal(SIGINT, handle_sigint);

    printf("Press Ctrl+C to send SIGINT (3 times to exit)...\n");

    // Infinite loop to keep the program running and waiting for signals
    while (1);

    return 0;
}

