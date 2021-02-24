#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

volatile int sig_counter = 0;

void
handler(int sig)
{
    if (sig == SIGUSR1) {
        sig_counter += 5;
        printf("%d %d\n", SIGUSR1, sig_counter);
        fflush(stdout);
    } else if (sig == SIGUSR2) {
        sig_counter -= 4;
        printf("%d %d\n", SIGUSR2, sig_counter);
        fflush(stdout);
        if (sig_counter < 0) {
            _exit(0);
        }
    }
}

int
main(void) 
{
    struct sigaction str_sig1;
    str_sig1.sa_handler = handler;
    str_sig1.sa_flags = SA_RESTART;
    sigaction(SIGUSR1, &str_sig1, NULL);
    sigaction(SIGUSR2, &str_sig1, NULL);
    printf("%d\n", getpid());
    fflush(stdout);

    while(1) {
        pause();
    }
    return 0;
}
