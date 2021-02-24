#include <stdio.h>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

enum
{
    PRODUCT_NAME = 16,
    PENNY = 100
};

struct Check
{
    char buf[PRODUCT_NAME];
    int price;
};

int
main(int argc, char *argv[])
{
    struct Check check_now;
    int max_first = INT_MIN;
    int max_second = max_first;
    int change_max_first = 0;
    int change_max_second = 0;
    for (int i = 1; i < argc; ++i) {
        int fd = open(argv[i], O_RDONLY);
        if (fd == -1) {
            return 1;
        }
        while ((read(fd, &check_now, sizeof(check_now))) == sizeof(check_now)) {
            if (max_first < check_now.price || change_max_first == 0) {
                change_max_second = change_max_first & 1;
                max_second = max_first;
                max_first = check_now.price;
                change_max_first = 1;
            } else if (max_first > check_now.price && max_second < check_now.price) {
                max_second =  check_now.price;
                change_max_second = 1;
            }
        }
    }
    
    if (change_max_second) {
        printf("%.2f\n", (double)max_second / (double)PENNY);
    }
}
