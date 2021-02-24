#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/types.h>
#include <wait.h>
#include <string.h>

struct tm
make_str_tm(int fd[2])
{  
    struct tm str_tm;
    memset(&str_tm, 0 , sizeof(str_tm));
    time_t t;
    close(fd[1]);
    read(fd[0], &t, sizeof(t));
    close(fd[0]);
    str_tm = *localtime_r(&t, &str_tm);
    return str_tm;
}

int
main(void)
{
    int fd[2];
    pipe(fd);
    pid_t pid_1;
    pid_1 = fork();
    if (!pid_1) {
        pid_t pid_2;
        pid_2 = fork();
        if (!pid_2) {
            pid_t pid_3;
            pid_3 = fork();
            if (!pid_3) {
                time_t t_time = time(NULL);
                close(fd[0]);
                write(fd[1], &t_time, sizeof(t_time));
                write(fd[1], &t_time, sizeof(t_time));
                write(fd[1], &t_time, sizeof(t_time));
                close(fd[1]);
                _exit(0);
            } else {
                int status_3;
                waitpid(pid_3, &status_3, 0);
                struct tm pr = make_str_tm(fd);
                printf("D:%02d\n", pr.tm_mday);
                fflush(stdout);
                _exit(0);
            }
        } else {
            int status_2;
            waitpid(pid_2, &status_2, 0);
            struct tm pr = make_str_tm(fd);
            printf("M:%02d\n", pr.tm_mon + 1);
            fflush(stdout);
            _exit(0);
        }
    } else {
        int status_1;
        waitpid(pid_1, &status_1, 0);
        struct tm pr = make_str_tm(fd);
        printf("Y:%04d\n", pr.tm_year + 1900);
        fflush(stdout);
        _exit(0);
    }
    return 0;
}
