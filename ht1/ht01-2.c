#include <stdlib.h>
#include <stdio.h>

typedef struct RandomGenerator RandomGenerator;

enum
{
    MUL_A = 1103515245,
    INC_C = 12345,
    MOD_M = 1 << 31
};
unsigned next_rand_gen(RandomGenerator *);
void destroy_rand_gen(RandomGenerator *);

const struct RandomOperations
{
    unsigned (*next)(RandomGenerator *);
    void (*destroy)(RandomGenerator *);
} rand_op = {next_rand_gen, destroy_rand_gen};

typedef struct RandomGenerator
{
    const struct RandomOperations *ops;
    unsigned key;
} RandomGenerator;

unsigned next_rand_gen(RandomGenerator *tmp)
{
    tmp->key = ((unsigned long long) MUL_A * tmp->key + INC_C) % ((unsigned)MOD_M);
    return tmp->key;
}

void destroy_rand_gen(RandomGenerator *tmp)
{
    free(tmp);
}

RandomGenerator *random_create(int seed)
{
    RandomGenerator *ptr = calloc(1, sizeof(*ptr));
    ptr->ops = &rand_op;
    ptr->key = seed;
    return ptr;
}
int
main(void)
{
    RandomGenerator *rr = random_create(1234);
    for (int j = 0; j < 1000; ++j) {
        printf("%d\n", rr->ops->next(rr));
    }
    rr->ops->destroy(rr);
}
