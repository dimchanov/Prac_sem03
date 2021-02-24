#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <wait.h>

int
main(void)
{
    int p_to_gs[2];
    pipe(p_to_gs);
    pid_t pid;
    pid = fork();
    if (pid != 0) {
        if (pid < 0) {
        _exit(0);
        }
        int tmp_num;
        close(p_to_gs[0]);
        while(scanf("%d", &tmp_num) == 1) {
            write(p_to_gs[1], &tmp_num, sizeof(tmp_num));
        }
        close(p_to_gs[1]);
        waitpid(pid, NULL, 0);
        _exit(0);
    } else { 
        pid_t pid_s;
        pid_s = fork();
        if (pid_s != 0) {
            if (pid_s < 0) {
            _exit(1);
            }
            close(p_to_gs[0]);
            close(p_to_gs[1]);
            waitpid(pid_s, NULL, 0);
            _exit(0);
        } else {
            close(p_to_gs[1]);
            int num; 
            long long sum = 0;
            while(read(p_to_gs[0], &num, sizeof(num)) == sizeof(num)) {
                sum += (long long)num;
            }
            printf("%lld\n", sum);
            fflush(stdout);
            close(p_to_gs[0]);
            _exit(0);
        }
    }
    return 0;
}
