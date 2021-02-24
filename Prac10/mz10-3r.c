#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

int
main(int argc, char **argv)
{
    int fd[2];
    pipe(fd);
    pid_t pid_f;
    pid_f = fork(); 
    if (!pid_f) {
        if (dup2(fd[1], 1) == -1) {
            _exit(1);
        }
        close(fd[0]);
        close(fd[1]);

        pid_t pid_1;
        pid_1 = fork();
        if (!pid_1) {
            int f_in = open(argv[4], O_RDONLY);
            if (f_in == -1) {
                _exit(1);
            }
            if (dup2(f_in, 0) == -1) {
                _exit(1);
            }
            close(f_in);
            execlp(argv[1], argv[1], NULL);
            _exit(1);
        }
        
        int status;
        waitpid(pid_1, &status, 0);
        if (WIFEXITED(status) && !WEXITSTATUS(status)) {        
            pid_t pid_2;
            pid_2 = fork();
            if (!pid_2) {
                execlp(argv[2], argv[2], NULL);
                _exit(1);
            }
        }
        wait(NULL);
        _exit(0);
    }
    
    pid_t pid_s;
    pid_s = fork();
    if (!pid_s) {
        if (dup2(fd[0], 0) == -1) {
            _exit(1);
        }
        close(fd[0]);
        close(fd[1]);

        pid_t pid_3;
        pid_3 = fork();
        if (!pid_3) {
            int f_out = open(argv[5], O_WRONLY | O_CREAT | O_APPEND, 0666);
            if (f_out == -1) {
                _exit(1);
            }
            if (dup2(f_out, 1) == -1) {
                _exit(1);
            }
            close(f_out);
            execlp(argv[3], argv[3], NULL);
            _exit(1);
        }
        wait(NULL);
        _exit(0);
    }
    close(fd[0]);
    close(fd[1]);
    wait(NULL);
    wait(NULL);
    return 0;
}

