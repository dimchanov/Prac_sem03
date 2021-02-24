#include <sys/stat.h>
enum
{
    USER = 448,
    GROUP = 56,
    OTHER = 7
};

struct Task
{
    unsigned uid;
    int gid_count;
    unsigned *gids;
};

int access_cmp(int access, int bits)
{
    return ((access & bits) == access);
}

int is_user(int mode)
{
    return (mode & USER) >> 6;
}

int is_group(int mode)
{
    return (mode & GROUP) >> 3;
}

int is_other(int mode)
{
    return (mode & OTHER);
}

int myaccess(const struct stat *stb, const struct Task *task, int access)
{
    if (!task->uid) {
        return 1;
    }
    int bits_cmp_access = is_user(stb->st_mode);
    if (task->uid == stb->st_uid) {
        return access_cmp(access, bits_cmp_access);
    }
    bits_cmp_access = is_group(stb->st_mode);
    for (int i = 0; i < task->gid_count; ++i) {
        if (task->gids[i] == stb->st_gid) {
            return access_cmp(access, bits_cmp_access);
        }
    }
    bits_cmp_access = is_other(stb->st_mode);
    return access_cmp(access, bits_cmp_access);
}
