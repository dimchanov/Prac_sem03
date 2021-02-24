#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>

enum
{
    LAST_YEAR = 25,
    LAST_MON = 9,
    LAST_DAY = 7,
    SECS_IN_DAY = 86400,
    YEAR = 1900,
    NEXT_DAY = 30,
    MON = 12,
    ARG = 3
};

int
main(void)
{
    struct tm next_day;
    struct tm last_day;
    memset(&next_day, 0, sizeof(next_day));
    memset(&last_day, 0, sizeof(last_day));
    last_day.tm_year = LAST_YEAR;
    last_day.tm_mon = LAST_MON;
    last_day.tm_mday = LAST_DAY;
    last_day.tm_isdst = 1;
    int64_t last_time = mktime(&last_day);
    int year, mon, day;
    while(scanf("%d%d%d", &year, &mon, &day) == ARG) {
        mon--;
        next_day.tm_year = year - YEAR;
        next_day.tm_mon = mon;
        next_day.tm_mday = day;
        next_day.tm_isdst = -1;
        int64_t ch = (int64_t) mktime(&next_day) - last_time;
        ch /= SECS_IN_DAY;
        printf("%"PRIdLEAST64" ", ch / (MON * NEXT_DAY) + 1);
        printf("%"PRIdLEAST64" ", (ch / NEXT_DAY) % MON + 1);
        printf("%"PRIdLEAST64"\n", ch % NEXT_DAY + 1);
    }
}
