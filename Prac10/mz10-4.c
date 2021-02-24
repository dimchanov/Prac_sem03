#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

void
kill_all_children(pid_t parent_pid)
{
    kill(-parent_pid, SIGKILL);
    while(wait(NULL) != -1){}
}

void 
close_all(int fd1[2], int fd2[2]) {
    close(fd1[0]);
    close(fd1[1]);
    close(fd2[0]);
    close(fd2[1]);
}

int
main(int argc, char **argv)
{
    if (argc < 2) {
        return 0;
    }
    close(2);
    pid_t parent_pid = getpid();
    setpgid(parent_pid, getppid());
    int fd[2];
    if (pipe(fd) == -1) {
        _exit(1);
    }
    int fd_ch[2];
    for (int i = 1; i < argc; ++i) {
        if (pipe(fd_ch) == -1) {
            close_all(fd, fd_ch);
            kill_all_children(parent_pid);
            _exit(1);
        }

        pid_t tmp_pid = fork();

        if (tmp_pid < 0) {
            close_all(fd, fd_ch);
            kill_all_children(parent_pid);
            _exit(1);
        }
        
        if (!tmp_pid) {
            if (i < argc - 1) {
                dup2(fd_ch[1], 1);
            }
            if (i > 1) {
                dup2(fd[0], 0);
            }
            close_all(fd, fd_ch);
            execlp(argv[i], argv[i], NULL);
            _exit(1);
        } else {
            setpgid(tmp_pid, parent_pid);
            if (close(fd[0]) == -1) {
                close_all(fd, fd_ch);
                kill_all_children(parent_pid);
                _exit(1);
            }
            if (close(fd[1]) == -1) {
                close_all(fd, fd_ch);
                kill_all_children(parent_pid);
                _exit(1);
            }
            fd[0] = fd_ch[0];
            fd[1] = fd_ch[1];
        }
    }
    close(fd_ch[0]);
    close(fd_ch[1]);
    while(wait(NULL) != -1) {}
    return 0;
}

