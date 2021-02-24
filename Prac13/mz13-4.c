#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/mman.h>

enum
{
    DEC = 10
};

int
main(int argc, char **argv)
{
    int count = (int)strtol(argv[1], NULL, DEC);
    
    key_t key = ftok(argv[0], 0);

    int semid = semget(key, count, 0600 | IPC_CREAT);
    if (semid == -1) {
        return 1;
    }
    semctl(semid, 0, SETVAL, 1);
    setbuf(stdin, NULL);
    for (int i = 0; i < count; ++i) {
        pid_t pid;
        pid = fork();
        if (!pid) {
            while(1) {
                struct sembuf str_cl[] =
                {
                    { .sem_num = i, .sem_op = -1, .sem_flg = 0}  
                };
                if (semop(semid, str_cl, 1) == -1) {
                    _exit(0);
                }
                int num;
                if (scanf("%d", &num) == 1) {
                    printf("%d %d\n", i, num);
                    fflush(stdout);
                    semctl(semid, (count + (num % count)) % count, SETVAL, 1);
                } else {
                    semctl(semid, 0, IPC_RMID);
                    _exit(0);
                }
            }
        }
    }
    
    while (wait(NULL) != -1) {}

    return 0;
}
