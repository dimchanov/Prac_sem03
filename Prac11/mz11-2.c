#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

volatile int sig_flag = 1;

void
handler1(int sig)
{
    sig_flag = 1;
}

void
handler2(int sig)
{
    sig_flag = 0;
}

int
main(void) 
{
    struct sigaction str_sig1;
    str_sig1.sa_handler = handler1;
    str_sig1.sa_flags = SA_RESTART;
    sigaction(SIGINT, &str_sig1, NULL);

    struct sigaction str_sig2;
    str_sig2.sa_handler = handler2;
    str_sig2.sa_flags = SA_RESTART;
    sigaction(SIGQUIT, &str_sig2, NULL);

    printf("%d\n", getpid());
    fflush(stdout);

    int number;
    int full = 0;
    while(scanf("%d", &number) == 1) {
        if (sig_flag) {
            __builtin_sadd_overflow(full, number, &full);
            printf("%d\n", full);
            fflush(stdout);
        } else {
            __builtin_smul_overflow(full, number, &full);
            printf("%d\n", full);
            fflush(stdout);
        }
    }
    return 0;
}
