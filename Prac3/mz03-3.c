#include <stdio.h>
#include <stdlib.h>
#include <math.h>

enum 
{
    FOR_ROUND = 10000,
    PERCENT = 100,
    NUM_PERCENT = 2
};

int 
main(int argc, char *argv[])
{
    double first_course;
    char *end;
    first_course = strtod(argv[1], &end);
    for (int i = NUM_PERCENT; i < argc; i++) {
        first_course *= (strtod(argv[i], &end) + PERCENT) / PERCENT;
        first_course = round(first_course * FOR_ROUND) / FOR_ROUND;
    }
    printf("%.4lf\n", first_course);
}
