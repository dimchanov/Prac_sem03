#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

enum
{
    SC_COUNT = 1
};

int
main(void)
{
    pid_t f_pid = getpid();
    pid_t pid;
    int tmp_number;
    int ex_status = 0;
    while(scanf("%d", &tmp_number) == SC_COUNT) {
        pid = fork();
        if (pid > 0) {
            int status;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status)) {
                if (WEXITSTATUS(status) == 0) {
                    printf("%d\n", tmp_number);
                    fflush(stdout);
                    ex_status = 0;
                } else {
                    ex_status = 1;
                }
            }
            break;
        } 
        if (pid < 0) {
            printf("%d\n", -1);
            fflush(stdout);
            ex_status = 1;
            break;
        }
    }
    if (getpid() == f_pid) {
        return 0;
    } else {
        _exit(ex_status);
    }
}
