#include <stdio.h>
#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t keep_running = 1;

void handle_sigint(int sig) {
    printf("\nCaught SIGINT. Shutting down gracefully...\n");
    keep_running = 0;
}

void handle_sigusr1(int sig) {
    printf("\nCaught SIGUSR1. Still running...\n");
}

int main() {
    signal(SIGINT, handle_sigint);
    signal(SIGUSR1, handle_sigusr1);

    printf("Signal handler program running.\n");
    printf("PID: %d\n", getpid());
    printf("Press Ctrl+C to stop, or send SIGUSR1 from another terminal.\n");

    while (keep_running) {
        printf("Working...\n");
        sleep(2);
    }

    printf("Program exited cleanly.\n");
    return 0;
}