#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdint.h>

enum
{
    HEX_BASE = 16,
    FILE_SIZE = 65536,
    PAGE_SIZE = 511,
    MOV_MASK = 9,
    OCT = 8
};

int
main(int argc, char **argv)
{
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        return 1;
    }
    uint16_t adr = (uint16_t)strtol(argv[2], NULL, HEX_BASE);
    void *vptr = mmap(NULL, FILE_SIZE, PROT_READ, MAP_PRIVATE, fd, 0);
    if (vptr == MAP_FAILED) {
        return 1;
    }
    uint8_t  *ptr = (uint8_t *)vptr;
    uint16_t tmp_adr;
    while(scanf("%hx", &tmp_adr) == 1) {
        uint16_t mov_num = tmp_adr & PAGE_SIZE;
        uint16_t page_num = tmp_adr >> MOV_MASK;
        uint8_t *f_adr = (ptr + adr + sizeof(page_num) * page_num);
        page_num = *(f_adr) + (*(f_adr + 1) << OCT);
        uint8_t *r_f_adr = (ptr + page_num + mov_num);
        printf("%hu\n", *(r_f_adr) + (*(r_f_adr + 1) << OCT));        
    }
    munmap(vptr, FILE_SIZE);
    close(fd);
}
