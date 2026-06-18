#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int pipefd[2];
    pid_t pid;
    char message[] = "Hello from parent process!";
    char buffer[100];

    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }

    pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        close(pipefd[1]);

        read(pipefd[0], buffer, sizeof(buffer));
        printf("Child received: %s\n", buffer);

        close(pipefd[0]);
        return 0;
    } else {
        close(pipefd[0]);

        write(pipefd[1], message, strlen(message) + 1);
        printf("Parent sent: %s\n", message);

        close(pipefd[1]);
        wait(NULL);
    }

    return 0;
}