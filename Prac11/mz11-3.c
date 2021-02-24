#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

enum
{
    DEC = 10
};

int
main(int argc, char **argv)
{
    if (argc < 2) {
        return 0;
    }
    long n = strtol(argv[1], NULL, DEC);
    int count = 0;
    for(int i = 2; i < n + 2; ++i) {
        pid_t pid = fork();
        if (!pid) {
            int fd;
            fd = open(argv[i], O_RDONLY);
            char buf[PATH_MAX + 1];
            read(fd, buf, sizeof(buf));
            buf[strlen(buf) - 1] = '\0';
            close(fd);
            execlp(buf, buf, NULL);
            _exit(1);
        }
    }
    int status;
    while(wait(&status) != -1) {
        if (WIFEXITED(status)) {
            if (!WEXITSTATUS(status)) {
                count++;
            }
        }
    }
    
    for(int i = n + 2; i < argc; ++i) {
        pid_t pid = fork();
        if (!pid) {
            int fd;
            fd = open(argv[i], O_RDONLY);
            char buf[PATH_MAX + 1];
            read(fd, buf, sizeof(buf));
            buf[strlen(buf) - 1] = '\0';
            close(fd);
            execlp(buf, buf, NULL);
            _exit(1);
        } else {
            waitpid(pid, &status, 0);
            if (WIFEXITED(status)) {
                if (!WEXITSTATUS(status)) {
                    count++;
                }
            }
        }
    }
    printf("%d\n", count);
    fflush(stdout);
}
