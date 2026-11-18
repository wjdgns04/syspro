#include <stdio.h>

int main(int argc, char *argv[])
{
    FILE *fp;
    int c;
    int line = 1;
    int show_line = 0;
    int start = 1;
    int print_num = 1;

    if (argc < 2) {
        fp = stdin;
        c = getc(fp);
        while (c != EOF) {
            putc(c, stdout);
            c = getc(fp);
        }
        return 0;
    }

    if (argv[1][0] == '-' && argv[1][1] == 'n' && argv[1][2] == '\0') {
        show_line = 1;
        start = 2;
    }

    for (int i = start; i < argc; i++) {
        fp = fopen(argv[i], "r");
        if (fp == NULL)
            continue;

        c = getc(fp);
        while (c != EOF) {
            if (show_line && print_num) {
                printf("%3d ", line++);
                print_num = 0;
            }
            putc(c, stdout);
            if (c == '\n')
                print_num = 1;
            c = getc(fp);
        }
        fclose(fp);
    }

    return 0;
}
