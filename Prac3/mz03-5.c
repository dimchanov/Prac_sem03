#include <stdio.h>
#include <limits.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

enum
{
    BUF_SIZE = 1024,
    MAS_SIZE = '9' - '0' + 1
};

int
main(void)
{
    unsigned long long ans[MAS_SIZE] = {0};
    char file_name[PATH_MAX + 1];
    if (!fgets(file_name, sizeof(file_name), stdin)) {
        for (int i = 0; i < MAS_SIZE; i++) {
            printf("%d %llu\n", i, ans[i]);
        }
        return 0;
    }
    
    for (int i = 0; i < sizeof(file_name); i++) {
        if (file_name[i] == '\n') {
            if (i > 0 && file_name[i - 1] == '\r') {
                file_name[i - 1] = '\0';
            }
            file_name[i] = '\0';
            break;
        }
    }
    int fd = open(file_name, O_RDONLY);
    char buf[BUF_SIZE];
    ssize_t n;
    while (fd != -1 && (n = read(fd, buf, sizeof(buf))) > 0) {
        for (int i = 0; i < n; i++) {
            int tmp = buf[i];
            if ('0' <= tmp && tmp <= '9') {
                ans[tmp - '0']++;
            }
        }
    }

    for (int i = 0; i < MAS_SIZE; i++) {
        printf("%d %llu\n", i, ans[i]);
    }
    close(fd);
}
