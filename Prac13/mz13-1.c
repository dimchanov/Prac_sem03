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
    int nproc = (int)strtol(argv[1], NULL, DEC);
    int key = (int)strtol(argv[2], NULL, DEC);
    long long maxval;
    maxval = strtoll(argv[3], NULL, DEC);

    long long *tmp;
    tmp =  mmap(NULL, sizeof(*tmp), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    *tmp = 0;

    int *last_proc; 
    last_proc = mmap(NULL, sizeof(*last_proc), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    *last_proc = 0;
    
    int semid = semget(key, nproc, 0600 | IPC_CREAT);
    if (semid == -1) {
        return 1;
    }
    semctl(semid, 0, SETVAL, 1);

    for (int i = 1; i <= nproc; ++i) {
        pid_t pid;
        pid = fork();
        if (!pid) {
            while(1) {
                struct sembuf str_cl[] =
                {
                    { .sem_num = i - 1, .sem_op = -1, .sem_flg = 0}  
                };
                if (semop(semid, str_cl, 1) == -1) {
                    _exit(0);
                }
                printf("%d %lld %d\n", i, *tmp, *last_proc);
                fflush(stdout);
                    
                if (*tmp < maxval) {
                    ++(*tmp);
                    *last_proc = i;
                    int num;
                    num = (((*tmp)%nproc)*((*tmp)%nproc)*((*tmp)%nproc)*((*tmp)%nproc)) % nproc + 1;
                    semctl(semid, num - 1, SETVAL, 1);
                } else {
                    semctl(semid, 0, IPC_RMID);
                    _exit(0);
                }
            }
        }
    }
    
    while (wait(NULL) != -1) {}
    munmap(tmp, sizeof(*tmp));
    munmap(last_proc, sizeof(*last_proc));
    return 0;
}
