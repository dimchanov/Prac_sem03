#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

enum
{
    BUF_SIZE = 8,
    DEC = 10
};

int
main(void)
{
    pid_t pid_1 = fork();
    if (pid_1 != 0) {
        pid_t pid_2 = fork();
        if (pid_2 != 0) {
            pid_t pid_3 = fork();
            if (pid_3 != 0) {
                int status;
                while((wait(&status)) > 0) {

                }
            } else {
                int number;
                char buf[BUF_SIZE];
                read(STDIN_FILENO, buf, sizeof(buf) - 1);
                sscanf(buf, "%d", &number);
                printf("%d %d\n", 3, number * number);
            }   
        } else {
            int number;
            char buf[BUF_SIZE];
            read(STDIN_FILENO, buf, sizeof(buf) - 1);
            sscanf(buf, "%d", &number);
            printf("%d %d\n", 2, number * number);
        }   
    } else {
        int number;
        char buf[BUF_SIZE];
        read(STDIN_FILENO, buf, sizeof(buf) - 1);
        sscanf(buf, "%d", &number);
        printf("%d %d\n", 1, number * number);
    }   
}
