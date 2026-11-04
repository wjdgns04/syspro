#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <getopt.h>

extern char **environ;

int main(int argc, char *argv[])
{
    int opt;
    char *env_name = NULL;
    int e_flag = 0;

    while ((opt = getopt(argc, argv, "e::ugip")) != -1) {
        switch (opt) {
            case 'e':
                e_flag = 1;
                if (optarg) {
                    env_name = optarg;
                } else if (optind < argc && argv[optind][0] != '-') {
                    env_name = argv[optind];
                }
                break;

            case 'u': {
                uid_t ruid = getuid();
                uid_t euid = geteuid();
                struct passwd *r_pw = getpwuid(ruid);
                struct passwd *e_pw = getpwuid(euid);

                printf("My Realistic User ID : %d(%s)\n", ruid, r_pw ? r_pw->pw_name : "unknown");
                printf("My Valid User ID : %d(%s)\n", euid, e_pw ? e_pw->pw_name : "unknown");
                break;
            }

            case 'g': {
                gid_t rgid = getgid();
                gid_t egid = getegid();
                struct group *r_gr = getgrgid(rgid);
                struct group *e_gr = getgrgid(egid);

                printf("My Realistic Group ID : %d(%s)\n", rgid, r_gr ? r_gr->gr_name : "unknown");
                printf("My Valid Group ID : %d(%s)\n", egid, e_gr ? e_gr->gr_name : "unknown");
                break;
            }

            case 'i':
                printf("my process number : [%d]\n", getpid());
                break;

            case 'p':
                printf("my parent's process number : [%d]\n", getppid());
                break;

            default:
                break;
        }
    }

    if (e_flag) {
        if (env_name != NULL) {
            char *val = getenv(env_name);
            if (val != NULL) {
                printf("%s = %s\n", env_name, val);
            }
        } else {
            for (char **env = environ; *env != NULL; env++) {
                printf("%s\n", *env);
            }
        }
    }

    return 0;
}
