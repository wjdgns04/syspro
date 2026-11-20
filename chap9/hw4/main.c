
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAXARG 100
#define MAXLINE 1024

int main()
{
    char buf[MAXLINE];
    char *args[MAXARG];
    int pid, status;
    int background;

    while (1) {

        printf("Pls input cmd : ");

        if (fgets(buf, sizeof(buf), stdin) == NULL)
            break;

        buf[strcspn(buf, "\n")] = '\0';

        if (strlen(buf) == 0)
            continue;

        if (strcmp(buf, "exit") == 0) {
            printf("Exit\n");
            break;
        }

        background = 0;

        if (buf[strlen(buf) - 1] == '&') {
            background = 1;
            buf[strlen(buf) - 1] = '\0';
        }

        int i = 0;
        char *token = strtok(buf, " ");

        while (token != NULL && i < MAXARG - 1) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }

        args[i] = NULL;

        if (args[0] == NULL)
            continue;

        printf("[%d] Parent process start\n", getpid());

        pid = fork();

        if (pid < 0) {
            perror("fork failed");
            exit(1);
        }

        if (pid == 0) {

            printf("[%d] child process start\n", getpid());

            execvp(args[0], args);

            perror("command execution failed");
            _exit(127);

        } else {

            if (background == 0) {

                int child = waitpid(pid, &status, 0);

                printf("[%d] child process end %d\n",
                       getpid(), child);

            } else {

                printf("[%d] background process started\n", pid);

            }

            printf("SUCCESS\n");
        }
    }

    return 0;
}

