#include <stdio.h>

enum
{
    MAX_S = 2000,
    MIN_S = 1
};

int 
binpow(int a, int n, int N)
{
    int res = 1;
    while (n) {
        if (n & 1) {
            res *= a;
            res = res % N;
        }
        a *= a;
        a = a % N;
        n >>= 1;
    }
    return res;
}

int 
main(int argc, char *argv[])
{
    int N;
    if (scanf("%d", &N) != 1) {
        fprintf(stderr, "invalid input\n");
        return 1;
    }
    if (N <= MIN_S || N  >= MAX_S){
        fprintf(stderr, "number out of range\n");
        return 2;
    }
    int reverse[MAX_S];
    for (int i = 1; i < N; i++) {
        reverse[i] = binpow(i, N - 2, N);
    }
    for (int i = 0; i < N; i++) {
        for (int j = 1; j < N; j++) {
            printf("%d ", i * reverse[j] % N);
        }
        putchar('\n');
    }
}
