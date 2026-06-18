#include <stdio.h>

int main() {
    char buffer[256];
    int line_count = 0;

    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        line_count++;
    }

    printf("Total lines received: %d\n", line_count);

    return 0;
}