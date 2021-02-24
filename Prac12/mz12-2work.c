#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <dirent.h>
#include <limits.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
 
int 
main(int argc, char **argv) 
{
    int count = strtol(argv[1], NULL, 10);
    int modulo = strtol(argv[2], NULL, 10);
    int p12[2];
    pipe(p12);
    int p24[2];
    pipe(p24);
    pid_t pid1, pid2, pid3, pid4;
    if ((pid1 = fork()) == 0) {
        close(p24[0]);
        if ((pid2 = fork()) == 0) {
            close(p12[1]);
            dup2(p12[0], 0);
            dup2(p24[1], 1);
            close(p12[0]);
            close(p24[1]);
            execlp(argv[3], argv[3], NULL);
            _exit(1);
        } else if (pid2 == -1) {
            _exit(1);
        }
        close(p12[0]);
        close(p24[1]);
        dup2(p12[1], 1);
        close(p12[1]);
        for (unsigned long long i = 1; i <= count; i++) {
            int res = (i * i) % modulo;
            printf("%d ", res);
            fflush(stdout);
        }
        close(1);
        _exit(0);
    } else if (pid1 == -1) {
        _exit(1);
    }
    close(p24[1]);
    close(p12[1]);
    close(p12[0]);
    if ((pid3 = fork()) == 0) {
        close(p12[1]);
        close(p12[0]);
        close(p24[1]);
        if ((pid4 = fork()) == 0) {
            int buf;
            dup2(p24[0], 0);
            close(p24[0]);
            while (scanf("%d", &buf) > 0) {
                printf("%d\n", buf);
                fflush(stdout);
            }
            _exit(0);
        } else if (pid4 == -1) {
            _exit(1);
        }
        close(p24[0]);
        wait(NULL);
        _exit(0);
    } else if (pid3 == -1) {
        _exit(1);
    }
    close(p24[0]);
    wait(NULL);
    wait(NULL);
    printf("0\n");
    fflush(stdout);
    return 0;
}
