#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAXARG 100
#define MAXLINE 1024

int main() {
    char buf[MAXLINE];
    char *args[MAXARG];
    int pid, status;
    int background;

    while (1) {
        printf("Pls input cmd : ");
        if (fgets(buf, sizeof(buf), stdin) == NULL)
            break;
        
        buf[strlen(buf) - 1] = '\0'; // 개행 문자 제거
        if (strlen(buf) == 0)
            continue;

        if (strcmp(buf, "exit") == 0) {
            printf("Exit\n");
            break;
        }

        background = 0;
        if (buf[strlen(buf) - 1] == '&') {
            background = 1;
            buf[strlen(buf) - 1] = '\0'; // '&' 제거
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
            fprintf(stderr, "%s:실행 불가\n", args[0]);
            _exit(127);
        } else {
            if (background == 0) {
                int child = wait(&status);
                printf("[%d] child process end %d\n", getpid(), child);
            } else {
                // 백그라운드 실행 시 부모가 바로 기다리지 않고 프롬프트로 복귀
                printf("[%d] background process started\n", pid);
            }
            printf("SUCCESS\n");
        }
    }
    return 0;
}
