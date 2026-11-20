#include <stdio.h>
#include "copy.h"

char line[MAXLINE];
char longest[MAXLINE];
char shortest[MAXLINE];

int get_length(char s[]) {
    int i = 0;
    while (s[i] != '\0') {
        i++;
    }
    return i;
}

int main() {
    int len;
    int max = 0;
    int min = MAXLINE;

    while (gets(line) != NULL) {
        len = get_length(line);

        if (len > max) {
            max = len;
            copy(line, longest);
        }

        if (len < min && len > 0) {
            min = len;
            copy(line, shortest);
        }
    }

    if (max > 0) {
        printf("longest: %s\n", longest);
    }
    
    if (min < MAXLINE) {
        printf("shortest: %s\n", shortest);
    }

    return 0;
}
