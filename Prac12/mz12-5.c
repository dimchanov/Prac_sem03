#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/signalfd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

enum
{
    BIT_COUNT = 8
};

int
main(int argc, char **argv)
{
    sigset_t s1, s2;
    sigemptyset(&s1);
    sigaddset(&s1, SIGUSR1);
    sigaddset(&s1, SIGUSR2);
    sigaddset(&s1, SIGALRM);
    sigaddset(&s1, SIGIO);
    sigprocmask(SIG_BLOCK, &s1, &s2);
    int fd = signalfd(-1, &s1, 0);

    pid_t pid1;
    pid1= fork();
    if (!pid1) {
        while(1) {
            char tmp = 0;
            int flag = 1;
            for (int i = 0; i < BIT_COUNT; ++i) {

                struct signalfd_siginfo str_si;
                if (read(fd, &str_si, sizeof(str_si)) != sizeof(str_si)) {
                    _exit(1);
                }

                if (str_si.ssi_signo == SIGUSR1) {
                    kill(str_si.ssi_pid, SIGALRM);  
                }

                if (str_si.ssi_signo == SIGUSR2) {
                    tmp = tmp | (1 << i);
                    kill(str_si.ssi_pid, SIGALRM);
                }
            
                if (str_si.ssi_signo == SIGIO) {
                    flag = 0;
                    break;
                }
            }
            if (flag) {
                printf("%c", tmp);
                fflush(stdout);
            } else {
                break;
            }
        }
        close(fd);
        _exit(0);
    } else {
        pid_t pid2;
        pid2 = fork();
        if (!pid2) {
            int file_fd = open(argv[1], O_RDONLY);
            char tmp;
            
            while(1) {
                
                if (read(file_fd, &tmp, sizeof(tmp)) == sizeof(tmp)) {
                    for (int i = 0; i < BIT_COUNT; ++i) {
                        if (tmp & 1) {
                            kill(pid1, SIGUSR2);
                        } else {
                            kill(pid1, SIGUSR1);
                        }
                        tmp >>= 1;
                        struct signalfd_siginfo str_si;
                        if (read(fd, &str_si, sizeof(str_si)) != sizeof(str_si)) {
                            _exit(1);
                        }
                    }
                } else {
                    kill(pid1, SIGIO);
                    break;
                }
            }
            close(fd);
            close(file_fd);
            _exit(0);
        } else {
            close(fd);
            while(wait(NULL) != -1) {}
            return 0;
        }
    }
}
