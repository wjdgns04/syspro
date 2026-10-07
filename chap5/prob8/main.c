#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

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

    printf("File read success\n");

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

    printf("Total Line : %d\n", line_count);
    printf("You can choose 1 ~ %d Line\n", line_count);
    printf("Pls 'Enter' the line to select : ");

    char input[50];
    if (scanf("%s", input) == EOF) return 0;

    if (strcmp(input, "*") == 0) {
        for (int i = 0; i < line_count; i++) {
            printf("%s\n", savedText[i]);
        }
    }
    else if (strchr(input, '-') != NULL) {
        int start, end;
        sscanf(input, "%d-%d", &start, &end);
        for (int i = start; i <= end; i++) {
            if (i >= 1 && i <= line_count) {
                printf("%s\n", savedText[i - 1]);
            }
        }
    }
    else if (strchr(input, ',') != NULL) {
        char *token = strtok(input, ",");
        while (token != NULL) {
            int line_num = atoi(token);
            if (line_num >= 1 && line_num <= line_count) {
                printf("%s\n", savedText[line_num - 1]);
            }
            token = strtok(NULL, ",");
        }
    }
    else {
        int line_num = atoi(input);
        if (line_num >= 1 && line_num <= line_count) {
            printf("%s\n", savedText[line_num - 1]);
        }
    }

    return 0;
}
