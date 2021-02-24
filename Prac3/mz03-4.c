#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>

enum
{
    BUF_SIZE = 16,
    DEC = 10
};

int
main(void)
{
    char buf[BUF_SIZE];
    ssize_t  n;
    unsigned long long sum = 0, number = 0;
    char sign = '+';
    while ((n = read(STDIN_FILENO, buf, sizeof(buf))) != 0) {
        if (n == -1) {
            fprintf(stderr, "reading  error\n");
            return 1;
        }
        for (int i = 0; i < n; i++) {
            if (isdigit((unsigned char)buf[i])){
                number = number * DEC + buf[i] - '0';
            }
            if (buf[i] == '+' || buf[i] == '-') {
                sign = buf[i];
            }
            if (isspace((unsigned char)buf[i])) {
                if (sign == '-') { 
                    number = -number;
                }
                sum += number;
                number = 0;
                sign = '+';
            }
        }
    }
    if (sign == '-') {
        number = -number;
    }
    sum += number;
    printf("%lld\n", sum);
}
