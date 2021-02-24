#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

enum
{
    BASE = 10
};

void
init_matr(unsigned * A, unsigned n, unsigned m)
{
    unsigned tmp = 0;
    unsigned left = 0;
    unsigned right = m - 1;
    unsigned top = 0;
    unsigned bottom = n - 1;
    unsigned i;
    
    while(tmp < n * m) {
        for(i = left; i <= right; ++i) {
            tmp++;
            A[top * m + i] = tmp;
        }
        if (tmp >= n * m) break;
        top++;
        for(i = top; i <= bottom; ++i) {
            tmp++;
            A[right + i * m] = tmp;
        }
        if (tmp >= n * m) break;
        right--;
        for(i = left; i <= right; ++i) {
            tmp++;
            A[bottom * m + right - i + left] = tmp;
        }
        if (tmp >= n * m) break;
        bottom--;
        for(i = top; i <= bottom; ++i) {
            tmp++;
            A[(bottom - i + top) * m + left] = tmp;
        }
        if (tmp >= n * m) break;
        left++;
    }
}

int
main(int argc, char **argv)
{
    char *end;
    unsigned  rows, cols;
    rows = (unsigned)strtoll(argv[2], &end, BASE);
    cols = (unsigned)strtoll(argv[3], &end, BASE);
    int fd = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, 0777);
    if (fd == -1) {
        return 1;
    }
    unsigned mass_size = rows*cols*sizeof(unsigned);
    ftruncate(fd, mass_size);
    void *ptr =  mmap(NULL, mass_size, PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        return 1;
    }
    unsigned *adr = ptr;
    init_matr(adr, rows, cols);
    munmap(adr, mass_size);
    close(fd);
}
