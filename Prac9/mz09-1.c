#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

int
main(int argc, char **argv)
{
    pid_t pid = fork();
    if (!pid) {
        int fd1 = open(argv[2], O_RDONLY);
        if (fd1 == -1) {
            _exit(42);
        }
        int fd2 = open(argv[3], O_CREAT | O_APPEND | O_WRONLY, 0660);
        if (fd2 == -1) {
            _exit(42);
        }
        int fd3 = open(argv[4], O_CREAT | O_TRUNC | O_WRONLY, 0660);
        if (fd3 == -1) {
            _exit(42);
        }
        if (dup2(fd1, 0) == -1) {
            _exit(42);
        }
        if (dup2(fd2, 1) == -1) {
            _exit(42);
        }
        if (dup2(fd3, 2) == -1) {
            _exit(42);
        }
        execlp(argv[1], argv[1], NULL);
        _exit(42);

    } else {
        int status;
        wait(&status);
        printf("%d\n", status);
    }
    return 0;
}
