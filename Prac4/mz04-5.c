#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

unsigned long long hash_function(unsigned long long);

enum
{
    HEX = 16,
    DEC = 10
};

int
main(int argc, char *argv[])
{
    int fd = open(argv[1], O_WRONLY | O_CREAT, 0660);
    if (fd == -1) {
        fprintf(stderr, "file open error\n");
        return 1;
    }
    char *c;
    unsigned long long h_0 = strtoll(argv[2], &c, HEX);
    unsigned long long count = strtoll(argv[3], &c, DEC);
    
    off_t last_elem_idx = lseek(fd, count * sizeof(h_0), SEEK_END);
    if (last_elem_idx == -1) {
        return 1;
    }
    unsigned long long h_tmp = h_0;
    for (long long i = count - 1; i >= 0; --i) {
        last_elem_idx -= sizeof(h_0);
        if (lseek(fd, last_elem_idx, SEEK_SET) == -1) {
            return 1;
        }
        if (write(fd, &h_tmp, sizeof(h_0)) != sizeof(h_0)) {
            return 1;
        }
        h_tmp = hash_function(h_tmp);
    }
    close(fd);
}
