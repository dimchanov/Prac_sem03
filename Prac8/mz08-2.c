#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int
main(void)
{
    pid_t pid_son = fork();
    if (!pid_son) {
        pid_t pid_gr_son = fork();
        if (!pid_gr_son) {
            printf("3 ");
            fflush(stdout);
        } else {
            int status_gr_son;
            wait(&status_gr_son);
            printf("2 ");
            fflush(stdout);
        }
    } else {
        int status_son;
        wait(&status_son);
        printf("1\n");
        fflush(stdout);
    }
}
