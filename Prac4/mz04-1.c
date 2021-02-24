#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>

enum 
{
    MAS_SIZE = sizeof(unsigned),
    FIRST_BYTE_MASK = 0xf,
    SECOND_BYTE_MASK = 0xff
};

int
main(int argc, char *argv[])
{
    int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0600);
    unsigned tmp;
    while(scanf("%u", &tmp) != EOF) {
        unsigned char mas[MAS_SIZE] = {0};
        for (int i = MAS_SIZE - 1; i >= 0; --i) {
            if (i % 2) {
                mas[i] = tmp & SECOND_BYTE_MASK;
                tmp >>= CHAR_BIT;
            } else {
                mas[i] = tmp & FIRST_BYTE_MASK;
                tmp >>= CHAR_BIT / 2;
            }
        }
        
        if (write(fd, mas, sizeof(mas)) != sizeof(mas)) {
            return 1;
        }
    }
    close(fd);
}
