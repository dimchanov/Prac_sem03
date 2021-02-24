#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>


int
main(void)
{
    int n;
    scanf("%d", &n);
    pid_t parent_pid = getpid();
    pid_t tmp_pid = 0;
    int tmp_print =  1;
    printf("%d", tmp_print);
    fflush(stdout);
    tmp_print++;
    while (tmp_print <= n) {
        tmp_pid = fork();
        if (tmp_pid) {
            break;
        }
        printf(" %d", tmp_print);
        fflush(stdout);
        tmp_print++;
    }
    if (tmp_pid ) {
        int status;
        waitpid(tmp_pid, &status, 0);
    }
    if (getpid() == parent_pid) {
        printf("\n");
    }
}
