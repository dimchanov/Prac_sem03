#include <stdio.h>
#include <stdlib.h>

enum { BASE = 10 };

int 
main(int argc, char *argv[])
{
    long long sum_neg_num = 0;
    long long sum_pos_num = 0;
    for (int i = 1; i < argc; i++) {
        char *end;
        long tmp_num = strtol(argv[i], &end, BASE);
        if (tmp_num > 0) {
            sum_pos_num += tmp_num;
        } else {
            sum_neg_num += tmp_num;
        }
    }
    printf("%lld\n%lld\n", sum_pos_num, sum_neg_num);
}

