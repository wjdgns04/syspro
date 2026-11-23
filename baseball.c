#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define FILE_NAME "baseball.dat"
#define TABLE_SIZE 101
#define NAME_SIZE 50

typedef struct {
    int used;
    char name[NAME_SIZE];
    int games;
    int atBats;
    int hits;
    int homeRuns;
} Player;

void clearInput(void)
{
    int ch;

    while ((ch = getchar()) != '\n' && ch != EOF)
        ;
}

/* 이름의 각 문자를 이용해서 저장 위치를 구한다. */
unsigned int hashName(const char *name)
{
    unsigned int hash = 0;

    while (*name != '\0') {
        hash = hash * 31 + (unsigned char)*name;
        name++;
    }

    return hash % TABLE_SIZE;
}

void readPlayer(FILE *fp, int index, Player *player)
{
    if (fseek(fp, (long)index * sizeof(Player), SEEK_SET) != 0 ||
        fread(player, sizeof(Player), 1, fp) != 1) {
        fprintf(stderr, "파일을 읽지 못했습니다.\n");
        exit(EXIT_FAILURE);
    }
}

void savePlayer(FILE *fp, int index, const Player *player)
{
    if (fseek(fp, (long)index * sizeof(Player), SEEK_SET) != 0 ||
        fwrite(player, sizeof(Player), 1, fp) != 1 ||
        fflush(fp) != 0) {
        fprintf(stderr, "파일을 저장하지 못했습니다.\n");
        exit(EXIT_FAILURE);
    }
}

/* 같은 위치에 다른 선수가 있으면 다음 칸을 확인한다. */
int findPlayer(FILE *fp, const char *name, Player *player)
{
    int start = (int)hashName(name);
    int index;
    int i;

    for (i = 0; i < TABLE_SIZE; i++) {
        index = (start + i) % TABLE_SIZE;
        readPlayer(fp, index, player);

        if (!player->used || strcmp(player->name, name) == 0)
            return index;
    }

    return -1;
}

void printPlayer(const Player *player)
{
    double average = 0.0;

    if (player->atBats > 0)
        average = (double)player->hits / player->atBats;

    printf("\n이름: %s\n", player->name);
    printf("출전 게임: %d\n", player->games);
    printf("타수: %d\n", player->atBats);
    printf("안타: %d\n", player->hits);
    printf("평균 타율: %.3f\n", average);
    printf("홈런: %d\n", player->homeRuns);
}

int main(void)
{
    FILE *fp;
    Player player;
    Player empty = {0};
    char name[NAME_SIZE];
    int menu, index, i;
    int atBats, hits, homeRuns;

    fp = fopen(FILE_NAME, "rb+");

    /* 처음 실행했을 때만 빈 레코드로 파일을 만든다. */
    if (fp == NULL) {
        if (errno != ENOENT) {
            perror("파일 열기 실패");
            return 1;
        }

        fp = fopen(FILE_NAME, "wb+");

        if (fp == NULL) {
            perror("파일 생성 실패");
            return 1;
        }

        for (i = 0; i < TABLE_SIZE; i++)
            savePlayer(fp, i, &empty);
    }

    while (1) {
        printf("\n===== 프로 야구 선수 기록 =====\n");
        printf("1. 선수 등록\n");
        printf("2. 선수 검색\n");
        printf("3. 경기 기록 추가\n");
        printf("0. 종료\n");
        printf("선택: ");

        if (scanf("%d", &menu) != 1)
            break;

        clearInput();

        if (menu == 0)
            break;

        if (menu < 1 || menu > 3) {
            printf("메뉴를 다시 선택하세요.\n");
            continue;
        }

        printf("선수 이름: ");

        if (fgets(name, sizeof(name), stdin) == NULL)
            break;

        /* fgets로 입력받으면 끝에 붙는 줄바꿈을 지운다. */
        if (strchr(name, '\n') == NULL && !feof(stdin)) {
            clearInput();
            printf("이름이 너무 깁니다.\n");
            continue;
        }

        name[strcspn(name, "\n")] = '\0';

        if (name[0] == '\0') {
            printf("이름을 입력하세요.\n");
            continue;
        }

        index = findPlayer(fp, name, &player);

        if (menu == 1) {
            if (index == -1) {
                printf("더 이상 선수를 등록할 수 없습니다.\n");
                continue;
            }

            if (player.used) {
                printf("이미 등록된 이름입니다.\n");
                continue;
            }

            player = empty;
            player.used = 1;
            strcpy(player.name, name);

            savePlayer(fp, index, &player);
            printf("등록되었습니다.\n");
        } else {
            if (index == -1 || !player.used) {
                printf("등록되지 않은 선수입니다.\n");
                continue;
            }

            if (menu == 2) {
                printPlayer(&player);
            } else {
                printf("이번 경기의 타수 안타 홈런: ");

                if (scanf("%d %d %d", &atBats, &hits, &homeRuns) != 3) {
                    clearInput();
                    printf("숫자 3개를 입력하세요.\n");
                    continue;
                }

                clearInput();

                if (atBats < 0 || hits < 0 || homeRuns < 0 ||
                    hits > atBats || homeRuns > hits) {
                    printf("0 <= 홈런 <= 안타 <= 타수여야 합니다.\n");
                    continue;
                }

                /* 이번 경기의 기록을 기존 기록에 더한다. */
                player.games++;
                player.atBats += atBats;
                player.hits += hits;
                player.homeRuns += homeRuns;

                savePlayer(fp, index, &player);
                printf("기록이 수정되었습니다.\n");
                printPlayer(&player);
            }
        }
    }

    fclose(fp);
    return 0;
}

