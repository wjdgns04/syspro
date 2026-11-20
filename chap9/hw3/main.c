
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAXARG 100
#define MAXLINE 1024

void execute_command(char *cmd)
{
    char *args[MAXARG];
    char *token;
    char *saveptr;

    char *input_file = NULL;
    char *output_file = NULL;

    int background = 0;
    int i = 0;

    // 명령어 끝의 & 확인
    int len = strlen(cmd);

    while (len > 0 && cmd[len - 1] == ' ')
        cmd[--len] = '\0';

    if (len > 0 && cmd[len - 1] == '&') {
        background = 1;
        cmd[--len] = '\0';
    }

    // 명령어와 인수 분리
    token = strtok_r(cmd, " \t", &saveptr);

    while (token != NULL && i < MAXARG - 1) {

        if (strcmp(token, "<") == 0) {
            token = strtok_r(NULL, " \t", &saveptr);
            if (token != NULL)
                input_file = token;
        }
        else if (strcmp(token, ">") == 0) {
            token = strtok_r(NULL, " \t", &saveptr);
            if (token != NULL)
                output_file = token;
        }
        else {
            args[i++] = token;
        }

        token = strtok_r(NULL, " \t", &saveptr);
    }

    args[i] = NULL;

    if (args[0] == NULL)
        return;

    printf("[%d] Parent process start\n", getpid());

    int pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return;
    }

    if (pid == 0) {

        printf("[%d] child process start\n", getpid());

        // 입력 리다이렉션
        if (input_file != NULL) {
            int fd = open(input_file, O_RDONLY);

            if (fd < 0) {
                perror("input file");
                _exit(1);
            }

            dup2(fd, STDIN_FILENO);
            close(fd);
        }

        // 출력 리다이렉션
        if (output_file != NULL) {
            int fd = open(output_file,
                          O_WRONLY | O_CREAT | O_TRUNC, 0644);

            if (fd < 0) {
                perror("output file");
                _exit(1);
            }

            dup2(fd, STDOUT_FILENO);
            close(fd);
        }

        execvp(args[0], args);

        perror("command execution failed");
        _exit(127);

    } else {

        if (background == 0) {
            int status;
            int child = waitpid(pid, &status, 0);

            printf("[%d] child process end %d\n",
                   getpid(), child);
        }
        else {
            printf("[%d] background process started\n", pid);
        }

        printf("SUCCESS\n");
    }
}

int main()
{
    char buf[MAXLINE];
    char *command;
    char *saveptr;

    while (1) {

        printf("Pls input cmd : ");
        fflush(stdout);

        if (fgets(buf, sizeof(buf), stdin) == NULL)
            break;

        buf[strcspn(buf, "\n")] = '\0';

        if (strlen(buf) == 0)
            continue;

        if (strcmp(buf, "exit") == 0) {
            printf("Exit\n");
            break;
        }

        // 세미콜론으로 명령어 분리 및 순차 실행
        command = strtok_r(buf, ";", &saveptr);

        while (command != NULL) {

            while (*command == ' ' || *command == '\t')
                command++;

            if (strlen(command) > 0)
                execute_command(command);

            command = strtok_r(NULL, ";", &saveptr);
        }
    }

    return 0;
}

