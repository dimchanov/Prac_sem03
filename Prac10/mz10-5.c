#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

enum
{
    DEC = 10
};

void
repeating_part(FILE* input, FILE* output, unsigned long long  max, int pid)
{
    unsigned long long tmp_num;
    while(fscanf(input, "%llu", &tmp_num) != EOF) {
        if (tmp_num >= max) {
            break;
        }
        printf("%d %llu\n", pid, tmp_num);
        fflush(stdout);
        tmp_num++;
        if (tmp_num >= max) {
            break;
        }
        fprintf(output, "%llu ", tmp_num);
        fflush(output);
    }
    fclose(input);
    fclose(output);
}

int
main(int argc, char **argv)
{
    long long max_num = strtoll(argv[1], NULL, DEC);
    if (max_num <= 1) {
        printf("Done\n");
        fflush(stdout);
        return 0;
    }
    int fd12[2];
    pipe(fd12);
    int fd21[2];
    pipe(fd21);

    pid_t pid1;
    pid1 = fork();
    if (!pid1) {
        close(fd12[0]);
        close(fd21[1]);
        FILE *file_w1 = fdopen(fd12[1], "w");
        FILE *file_r1 = fdopen(fd21[0], "r");
        repeating_part(file_r1, file_w1, (unsigned long long)max_num, 1);
        _exit(0);
    }
    
    pid_t pid2;
    pid2 = fork();
    if (!pid2) {
        close(fd12[1]);
        close(fd21[0]);
        FILE *file_w2 = fdopen(fd21[1], "w");
        FILE *file_r2 = fdopen(fd12[0], "r");
        unsigned long long tmp = 1;
        fprintf(file_w2, "%llu ", tmp);
        fflush(file_w2);
        repeating_part(file_r2, file_w2, (unsigned long long)max_num, 2);
        _exit(0);
    }
    
    close(fd12[0]);
    close(fd12[1]);
    close(fd21[0]);
    close(fd21[1]);
    wait(NULL);
    wait(NULL);

    printf("Done\n");
    fflush(stdout);
    return 0;
}

