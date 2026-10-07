#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_LINES 100
#define MAX_LEN 100

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        perror("File open error");
        return 1;
    }

    char savedText[MAX_LINES][MAX_LEN];
    char buf;
    int line_count = 0;
    int char_count = 0;

    while (read(fd, &buf, 1) > 0) {
        if (buf == '\n') {
            savedText[line_count][char_count] = '\0';
            line_count++;
            char_count = 0;
        } else {
            savedText[line_count][char_count++] = buf;
        }
    }

    if (char_count > 0) {
        savedText[line_count][char_count] = '\0';
        line_count++;
    }

    close(fd);

    for (int i = line_count - 1; i >= 0; i--) {
        printf("%s\n", savedText[i]);
    }

    return 0;
}
