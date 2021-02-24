#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>

int
main(void)
{
int n = 10;

pid_t pid = 0;
int i = 1;
printf("aaaa   %d %d\n", getpid(), getppid());
fflush(stdout);
while (i < n) {
    pid = fork();
    printf("aaaa   %d %d %d\n",pid, getppid(), getpid());
    fflush(stdout);
    if(pid){
    break;
    } else {
        printf("%d %d %d %d\n", i,pid, getppid(), getpid());
        fflush(stdout);
    }
    i++;
    }

return 0;
}
