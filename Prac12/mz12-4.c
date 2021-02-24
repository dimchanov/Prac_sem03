#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

enum
{
    DEC = 10
};

int
main(int argc, char **argv)
{
    int mod;
    mod = (int)strtol(argv[2], NULL, DEC);
    int fd;
    fd = open(argv[1], O_CREAT | O_TRUNC | O_WRONLY, 0751 );
    if (fd < 0) {
        return 1;
    }
    dup2(fd, 1);
    close(fd);
    printf("#! /usr/bin/python3\n");
    printf("num = int(input())\n");
    printf("num = (num *(num + 1))//2\n");
    printf("print(num %% %d)\n", mod);
    fflush(stdout);
    return 0;
}
