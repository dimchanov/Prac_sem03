#include <stdio.h>

enum
{
    NUM = 1,
    LOWER = NUM + '9' - '0' + 1,
    UPPER = LOWER + 'z' - 'a' + 1
};

void
c_e_second(int *cmp)
{
    int c = *cmp;
    if (c == 0) {
        *cmp = '@';
    }
    if (c > 0 && c < LOWER) {
        *cmp = c + '0' - NUM;
    }
    if (c >= LOWER && c < UPPER) {
        *cmp = c + 'a' - LOWER;
    }
    if (c >= UPPER && c < 64) {
        *cmp = c + 'A' - UPPER;
    }
}

int
change_encoding(int *cmp)
{
    int c = *cmp;
    int tmp = 0;
    if (c >= '0' && c <= '9') {
        *cmp = c - '0' + NUM;
        *cmp ^= 1 << 3;
        *cmp &= ~(1 << 2);
        tmp = 1;
    }
    if (c >= 'a' && c <= 'z') {
        *cmp = c - 'a' + LOWER;
        *cmp ^= 1 << 3;
        *cmp &= ~(1 << 2);       
        tmp = 1;
    }
    if (c >= 'A' && c <= 'Z') {
        *cmp = c - 'A' + UPPER;
        *cmp ^= 1 << 3;
        *cmp &= ~(1 << 2);  
        tmp = 1;
    }
    return tmp;
}

int
check_char(char c)
{
    return (('0' <= c && '9' >= c) || ('a' <= c && 'z' >= c) ||('A' <= c && 'Z' >= c));
}

int
main(int argc, char *argv[])
{
    int c;
    while ((c = getchar()) != EOF) {
        if (change_encoding(&c)) {
            c_e_second(&c);
            putchar(c);
        } else {
            continue;
        }
    }
}
