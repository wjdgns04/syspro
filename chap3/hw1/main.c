#include <stdio.h>
#include "copy.h"

int get_length(char s[]) {
    int i = 0;
    while (s[i] != '\0') {
        i++;
    }
    return i;
}

int main() {
    char lines[5][MAXLINE];
    char temp[MAXLINE];
    int count = 0;

    while (count < 5 && gets(lines[count]) != NULL) {
        count++;
    }

    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (get_length(lines[i]) < get_length(lines[j])) {
                copy(lines[i], temp);
                copy(lines[j], lines[i]);
                copy(temp, lines[j]);
            }
        }
    }

    for (int i = 0; i < count; i++) {
        printf("%s\n", lines[i]);
    }

    return 0;
}
