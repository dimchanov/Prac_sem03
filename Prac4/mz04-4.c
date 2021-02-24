#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <limits.h>

int 
main(int argc, char *argv[])
{
    int fd = open(argv[1], O_RDWR);
    if (fd == -1){
        return 1;
    }
    off_t idx_first_min = 0;
    off_t idx_elem = 0;
    long long tmp_number;
    long long first_min = LLONG_MAX;
    int flag = 0;
    while (read(fd, &tmp_number, sizeof(tmp_number)) == sizeof(tmp_number)) {
        if (flag == 0 || tmp_number < first_min) {
            first_min = tmp_number;
            flag = 1;
            idx_first_min = idx_elem;
        }
        idx_elem += sizeof(tmp_number);
    }
    if (flag) {
        first_min = -(unsigned long long)first_min;
        if (lseek(fd, idx_first_min, SEEK_SET) == -1) {
            return 1;
        }
        if (write(fd, &first_min, sizeof(first_min)) != sizeof(first_min)) {
            return 1;
        }
    }
    close(fd);
    return 0;
}
