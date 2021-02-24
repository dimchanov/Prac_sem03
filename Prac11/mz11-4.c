#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

volatile sig_atomic_t flag = 0;

enum
{
    DEC = 10
};

void
handler(int sig)
{
    flag = 1;
}

void
ch_print(FILE *in, FILE *out, int max, int num)
{
    while(1)
    {
        if (!flag) {
            pause();
        } else {
            flag = 0;
            pid_t next_pid;
            next_pid = getpid();
            int counter;
            pid_t pid;
            fscanf(in, "%d%d", &counter, &pid);
            if (counter == max) {
                fprintf(out, "%d %d ", counter, next_pid);
                fflush(out);
                fclose(in);
                fclose(out);
                _exit(0);
            }
            printf("%d %d\n", num, counter);
            fflush(stdout);
            ++counter;
            fprintf(out, "%d %d ", counter, next_pid);
            fflush(out);
            kill(pid, SIGUSR1);
        }
    }
}

int
main(int argc, char **argv)
{
    int maxcount = (int)strtol(argv[1], NULL, DEC);
    int fd[2];
    pipe(fd);
    FILE *output = fdopen(fd[1], "w");
    FILE *input = fdopen(fd[0], "r");
    
    struct sigaction sig_us;
    sig_us.sa_flags = SA_RESTART;
    sig_us.sa_handler = handler;
    sigaction(SIGUSR1, &sig_us, NULL);

    pid_t pid1, pid2;
    pid1 = fork();
    if (!pid1) {
        ch_print(input, output, maxcount, 1);
    } else {
        pid2 = fork();
        if (!pid2) {
            ch_print(input, output, maxcount, 2);
        } else {
            //printf("[%d] [%d]\n", pid1, pid2);
            fflush(stdout);
            int counter = 1;
            fprintf(output, "%d %d ", counter, pid2);
            fflush(output);
            fclose(input);
            fclose(output);
            kill(pid1, SIGUSR1);
            pid_t first_exited;
            first_exited = wait(NULL);
            pid_t wait_pid;
            if (first_exited == pid1) {
                wait_pid = pid2;
            } else {
                wait_pid = pid1;
            }
            kill(wait_pid, SIGUSR1);
            wait(NULL);
            printf("Done\n");
            fflush(stdout);
        }
    }
    return 0;
}
