#include <stdio.h>

int
main()
{
    double x, y;
    if (scanf("%lf%lf", &x, &y) != 2) {
        fprintf(stderr, "invalid input\n");
        return 1;
    }
    printf("%d\n", x <= 7 && x >=1 && y <= 5 && y >= 2 && y <= -1 * x + 10);
    return 0;
}
