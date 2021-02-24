#include <stdio.h>
#include <unistd.h>
#include <signal.h>

volatile sig_atomic_t counter = 0;

enum
{
    COUNT = 6
};

void
handler(int sig)
{
    if (counter == COUNT - 1) {
        _exit(0);
    } else {
        printf("%d\n", counter);
        fflush(stdout);
        ++counter;
    }
}

int
main(void)
{
    sigaction(SIGHUP, &(struct sigaction) {.sa_handler = handler, .sa_flags = SA_RESTART }, NULL);
    printf("%d\n", getpid());
    fflush(stdout);
    while(1) {
        pause();
    }
}
