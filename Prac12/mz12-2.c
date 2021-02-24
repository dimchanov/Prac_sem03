#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <string.h>

enum
{
    DEC = 10,
    BUF_SIZE = 1024
};

int
main(int argc, char **argv)
{
    int count =  (int) strtol(argv[1], NULL, DEC);
    int modulo = (int) strtol(argv[2], NULL, DEC);

    int fd12[2], fd24[2];
    pipe(fd12);
    pipe(fd24);

    pid_t pid1;
    pid1 = fork();
    if (!pid1) {
        pid_t pid2;
        pid2= fork();
        if (!pid2) {
            close(fd12[1]);
            close(fd24[0]);
            dup2(fd12[0], 0);
            close(fd12[0]);
            dup2(fd24[1], 1);
            close(fd24[1]);
            execlp(argv[3], argv[3], NULL);
            _exit(0);
        }
        close(fd24[0]);
        close(fd24[1]);
        close(fd12[0]);
        FILE *f = fdopen(fd12[1], "w");
        if (!f) {
            _exit(1);
        }
        for (int i = 1; i <= count; ++i) {
            unsigned long long  val;
            val = i % modulo;
            val = (val * val) % modulo;
            fprintf(f, "%d ", (int)val);
            fflush(f);
        }
        fclose(f);
        waitpid(pid2, NULL, 0);
        _exit(0);
    }
    close(fd12[0]);
    close(fd12[1]);
    close(fd24[1]);
    pid_t pid3;
    pid3 = fork();
    if (!pid3) {
        pid_t pid4;
        pid4 = fork();
        if (!pid4) {
            char print_buf[BUF_SIZE];
            int byte_count = 0;
            while((byte_count = read(fd24[0], print_buf, sizeof(print_buf))) > 0) {
                for(int i = 0; i < byte_count; ++i) {
                    if (print_buf[i] == ' ') {
                        print_buf[i] = '\n';
                    }
                }
                write(1, print_buf, byte_count);
                fflush(stdout);
            }
            close(fd24[0]);
            _exit(0);
        }
        close(fd24[0]);
        waitpid(pid4, NULL, 0);
        _exit(0);
    }
    close(fd24[0]);
    while(wait(NULL) != -1) {}
    printf("0\n");
    fflush(stdout);
    return 0;
}
