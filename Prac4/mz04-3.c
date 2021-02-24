#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

struct Node
{
    int32_t key;
    int32_t left_idx;
    int32_t right_idx;
};

void
print_tr(int fd, struct Node root_tmp)
{
    struct Node next_r;
    if (root_tmp.right_idx) {
        if (lseek(fd, root_tmp.right_idx * sizeof(next_r), SEEK_SET) == -1) {
            return;
        }
        if (read(fd, &next_r, sizeof(next_r)) != sizeof(next_r)) {
            return;
        }
        print_tr(fd, next_r);
    } 
    printf("%d\n", root_tmp.key);
    if (root_tmp.left_idx) {
        if (lseek(fd, root_tmp.left_idx * sizeof(next_r), SEEK_SET) == -1) {
            return;
        }
        if (read(fd, &next_r, sizeof(next_r)) != sizeof(next_r)) {
            return;
        }
        print_tr(fd, next_r);
    } 
    return;
}

int 
main(int argc, char *argv[])
{
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        return 1;
    }
    struct Node root_tr;
    if (read(fd, &root_tr, sizeof(root_tr)) != sizeof(root_tr)) {
        return 1;
    }
    print_tr(fd, root_tr);
}
