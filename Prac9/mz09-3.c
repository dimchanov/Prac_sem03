#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

int 
mysys(char *arg)
{
    pid_t pid = fork();
    if (!pid) {
        execlp(arg, arg, NULL);
        _exit(1);
    } else {
        if (pid == -1) {
            return 1;
        } else {
            int status;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
            }
        }
    }
    return 1;
}

int
main(int argc, char **argv)
{
    return !((!mysys(argv[1]) || !mysys(argv[2])) && !mysys(argv[3]));
}
