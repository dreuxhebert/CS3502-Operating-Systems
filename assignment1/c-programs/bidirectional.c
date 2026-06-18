#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int parent_to_child[2];
    int child_to_parent[2];
    pid_t pid;

    char parent_message[] = "Hello child, this is the parent.";
    char child_message[] = "Hello parent, this is the child.";
    char buffer[100];

    if (pipe(parent_to_child) == -1 || pipe(child_to_parent) == -1) {
        perror("pipe");
        return 1;
    }

    pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        close(parent_to_child[1]);
        close(child_to_parent[0]);

        read(parent_to_child[0], buffer, sizeof(buffer));
        printf("Child received: %s\n", buffer);

        write(child_to_parent[1], child_message, strlen(child_message) + 1);

        close(parent_to_child[0]);
        close(child_to_parent[1]);
        return 0;
    } else {
        close(parent_to_child[0]);
        close(child_to_parent[1]);

        write(parent_to_child[1], parent_message, strlen(parent_message) + 1);
        printf("Parent sent: %s\n", parent_message);

        read(child_to_parent[0], buffer, sizeof(buffer));
        printf("Parent received: %s\n", buffer);

        close(parent_to_child[1]);
        close(child_to_parent[0]);

        wait(NULL);
    }

    return 0;
}