#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

volatile sig_atomic_t count_sigint = 0;
volatile sig_atomic_t number = 0;

enum
{
    COUNT = 3
};

int
is_prime(int n)
{
    for(int j = 2; j * j <= n; ++j) {
        if (n % j == 0) {
            return 0;
        }
    }
    return 1;
}

void
print_prime_number(int low, int high)
{
    if (low < 2) {
        low = 2;
    }

    for (int i = low; i < high; ++i) {
        if (is_prime(i)) {
            number = i;
        }
    }
}

void
handler1(int sig)
{
    if (count_sigint < COUNT) {
        printf("%d\n", number);
        fflush(stdout);
        ++count_sigint;
    } else {
        _exit(0);
    }
}

void
handler2(int sig)
{
    _exit(0);
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
    sigaction(SIGTERM, &str_sig2, NULL);

    int low, high;
    scanf("%d%d", &low, &high);

    printf("%d\n", getpid());
    fflush(stdout);

    print_prime_number(low, high);

    printf("-1\n");
    fflush(stdout);
    return 0;
}
