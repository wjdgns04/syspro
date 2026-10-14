#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_stat(const char *pathname, const char *file, int opt_i, int opt_p, int opt_Q);

int main(int argc, char *argv[]) {
    DIR *dp;
    struct dirent *dirp;
    char *dirName = ".";
    
    int opt_i = 0;
    int opt_p = 0;
    int opt_Q = 0;

    // 인자 분석 (옵션 처리)
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            for (int j = 1; argv[i][j] != '\0'; j++) {
                if (argv[i][j] == 'i') opt_i = 1;
                else if (argv[i][j] == 'p') opt_p = 1;
                else if (argv[i][j] == 'Q') opt_Q = 1;
            }
        } else {
            dirName = argv[i];
        }
    }

    if ((dp = opendir(dirName)) == NULL) {
        perror("opendir error");
        exit(1);
    }

    // 디렉토리 읽기 및 파일 정보 출력
    while ((dirp = readdir(dp)) != NULL) {
        if (dirp->d_ino == 0) continue;
        print_stat(dirName, dirp->d_name, opt_i, opt_p, opt_Q);
    }

    closedir(dp);
    return 0;
}

void print_stat(const char *dirName, const char *file, int opt_i, int opt_p, int opt_Q) {
    char pathname[1024];
    struct stat buf;
    struct passwd *pwd;
    struct group *grp;
    struct tm *t;
    char perm[11];
    char time_str[20];

    snprintf(pathname, sizeof(pathname), "%s/%s", dirName, file);

    if (lstat(pathname, &buf) < 0) {
        perror("lstat error");
        return;
    }

    // 1. -i 옵션: inode 번호 출력
    if (opt_i) {
        printf("%9lu ", (unsigned long)buf.st_ino);
    }

    // 파일 타입 및 퍼미션 설정
    strcpy(perm, "----------");
    if (S_ISDIR(buf.st_mode)) perm[0] = 'd';
    else if (S_ISLNK(buf.st_mode)) perm[0] = 'l';
    else if (S_ISCHR(buf.st_mode)) perm[0] = 'c';
    else if (S_ISBLK(buf.st_mode)) perm[0] = 'b';
    else if (S_ISFIFO(buf.st_mode)) perm[0] = 'p';
    else if (S_ISSOCK(buf.st_mode)) perm[0] = 's';

    if (buf.st_mode & S_IRUSR) perm[1] = 'r';
    if (buf.st_mode & S_IWUSR) perm[2] = 'w';
    if (buf.st_mode & S_IXUSR) perm[3] = 'x';
    if (buf.st_mode & S_IRGRP) perm[4] = 'r';
    if (buf.st_mode & S_IWGRP) perm[5] = 'w';
    if (buf.st_mode & S_IXGRP) perm[6] = 'x';
    if (buf.st_mode & S_IROTH) perm[7] = 'r';
    if (buf.st_mode & S_IWOTH) perm[8] = 'w';
    if (buf.st_mode & S_IXOTH) perm[9] = 'x';

    // 블록 크기 및 퍼미션, 링크 수, 소유자, 그룹, 크기 출력
    printf("%ld %s %3lu ", (long)buf.st_blocks / 2, perm, (unsigned long)buf.st_nlink);

    pwd = getpwuid(buf.st_uid);
    printf("%s ", pwd ? pwd->pw_name : "unknown");

    grp = getgrgid(buf.st_gid);
    printf("%s ", grp ? grp->gr_name : "unknown");

    printf("%5ld ", (long)buf.st_size);

    t = localtime(&buf.st_mtime);
    strftime(time_str, sizeof(time_str), "%b %d %H:%M", t);
    printf("%s ", time_str);

    // 파일 이름 출력 가공 (-p 및 -Q 옵션 적용)
    char display_name[512];
    strcpy(display_name, file);

    // 2. -p 옵션: 디렉토리인 경우 이름 뒤에 '/' 추가
    if (opt_p && S_ISDIR(buf.st_mode)) {
        strcat(display_name, "/");
    }

    // 3. -Q 옵션: 이름 전체를 큰따옴표로 감싸기
    if (opt_Q) {
        printf("\"%s\"\n", display_name);
    } else {
        printf("%s\n", display_name);
    }
}
