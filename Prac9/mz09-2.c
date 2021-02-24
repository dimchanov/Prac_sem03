#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

int 
mysys(const char *str)
{
    pid_t pid = fork();
    if (!pid) {
        execl("/bin/sh", "sh", "-c", str, NULL);
        _exit(127);
    } else {
        if (pid == -1) {
            _exit(-1);
        } else {
            int status;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status)) {
                if (0 <= WEXITSTATUS(status) && WEXITSTATUS(status) <= 127)
                    return WEXITSTATUS(status);   
            }
            if (WIFSIGNALED(status)) {
                return (128 + WTERMSIG(status));
            }
        }
    }
    return 0;
}
