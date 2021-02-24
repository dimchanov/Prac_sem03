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

void
operation(int *data, int ind1, int ind2, int value);


int
main(int argc, char **argv)
{
    unsigned long long count = strtoll(argv[1], NULL, DEC);
    long long key = strtoll(argv[2], NULL, DEC);
    unsigned long long  nproc = strtoll(argv[3], NULL, DEC);
    unsigned long long iter_count = strtoll(argv[4], NULL, DEC);

    int semid = semget(key, count, 0600 | IPC_CREAT);
    if (semid == -1) {
        return 1;
    }
    
    int *mas;
    mas = (int *)mmap(NULL, count*sizeof(*mas), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    for(int i = 0; i < count; ++i) {
        if (scanf("%d", &mas[i]) != 1) {
            return 1;
        }
        semctl(semid, i, SETVAL, 1);
    }   
    
    for(int i = 0; i < nproc; ++i) {
        pid_t pid;
        pid = fork();
        if (!pid) {
            unsigned long long base = strtoll(argv[5 + i], NULL, DEC);
            srand(base);
            for(int j = 0; j < iter_count; ++j) {
                int idx1 =  rand() % count;
                int idx2 =  rand() % count;
                int num = rand() % DEC;
                if (idx1 > idx2) {
                    int a = idx1;
                    idx1 = idx2;
                    idx2 = a;
                }
                if (idx1 == idx2) {
                    continue;
                }
                semop(semid, (struct sembuf[]) {{.sem_num = idx1, .sem_op = -1, .sem_flg = 0},
                {.sem_num = idx2, .sem_op = -1, .sem_flg = 0}}, 2);

                operation(mas, idx1, idx2, num);
 
                semop(semid, (struct sembuf[]) {{.sem_num = idx1, .sem_op = +1, .sem_flg = 0},
                {.sem_num = idx2, .sem_op = +1, .sem_flg = 0}}, 2);
            }
            _exit(0);
        }
    }   

    while(wait(NULL) != -1) {}
    
    semctl(semid, 0, IPC_RMID);
    for(int i = 0; i < count; ++i) {
        printf("%d ", mas[i]);
        fflush(stdout);
    }
    printf("\n");
    fflush(stdout);
    munmap(mas, count * sizeof(*mas));
    return 0;
}
