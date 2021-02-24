#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/msg.h>

enum
{
    DEC = 10
};

struct Let
{
    long ntype;
    long long x[2];
};

int
main(int argc, char **argv)
{
    long long key, n, value1, value2, maxval;
    key = strtoll(argv[1], NULL, DEC); 
    n = strtoll(argv[2], NULL, DEC);
    value1 = strtoll(argv[3], NULL, DEC);
    value2 = strtoll(argv[4], NULL, DEC);
    maxval = strtoll(argv[5], NULL, DEC); 
    int msgid = msgget(key, IPC_CREAT | 0600);
    if (msgid == -1) {
        return 1;
    }

    struct Let f_snd;
    f_snd.ntype = 1;
    f_snd.x[0] = value1;
    f_snd.x[1] = value2;
    msgsnd(msgid, &f_snd, sizeof(f_snd.x), 0);
    
    pid_t *pids = calloc(n, sizeof(pid_t));

    for(int i = 0; i < n; ++i) {
        pids[i] = fork();
        if (!pids[i]) {
            while(1) {
                struct Let msgp;
                if (msgrcv(msgid, &msgp, sizeof(msgp.x), (long)(1 + i), 0) == -1) {
                    break;
                }
                long long num;
                num = msgp.x[0] + msgp.x[1];
                printf("%d %lld\n", i, num);
                fflush(stdout);
                if (num > maxval) {
                    break;
                }
                struct Let msg_snd;
                msg_snd.ntype = num % n + 1;
                msg_snd.x[0] = msgp.x[1];
                msg_snd.x[1] = num;
                msgsnd(msgid, &msg_snd, sizeof(msg_snd.x), 0);
            } 
            msgctl(msgid, IPC_RMID, 0);
            free(pids);
            _exit(0);
        } else if (pids[i] == -1){
            for (int j = 0; j < i; ++j) {
                kill(pids[j], SIGTERM);
            }
            while(wait(NULL) != -1) {}
            free(pids);
            msgctl(msgid, IPC_RMID, 0);
            return 1;
        }
    }
    free(pids);
    while(wait(NULL) != -1) {}
    return 0;
}
