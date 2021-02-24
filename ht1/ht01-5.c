#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <limits.h>

struct List
{
    int len;
    struct List *next;
    char *str;
};

int
compare(const void* a, const void* b)
{
    return strcasecmp(*(char**)a, *(char**)b);
}

void
dir_tree(char* root, char* r_t)
{
    DIR *dir = opendir(root);
    if (!dir) {
        return;
    }
    if (r_t) {
        printf("cd %s\n", r_t);
    }
    //printf("%s\n", root);
    //fflush(stdout);
    struct dirent *dt;
    char path[PATH_MAX];
    struct List *list_root = calloc(1, sizeof(*list_root));
    if (!list_root) {
        return;
    }
    int dir_name_len = strlen(root);
    struct List *ptr = list_root;
    ptr->next = NULL;
    int count_elem = 0;
    while((dt = readdir(dir))) {
        if (strcmp(dt->d_name, ".") && strcmp(dt->d_name, "..")){
            int tmp_len = strlen(dt->d_name);
            int len = dir_name_len + 2 + tmp_len;
            if (len <= PATH_MAX) {
                snprintf(path, len, "%s/%s", root, dt->d_name);
                struct stat stb;
                if (!lstat(path, &stb) && S_ISDIR(stb.st_mode)) {
                    ptr->str = calloc(tmp_len + 1, sizeof(*(ptr->str)));
                    if (!ptr->str) {
                        return;
                    }
                    snprintf(ptr->str, tmp_len + 1, "%s", dt->d_name);
                    ptr->len = tmp_len + 1;
                    count_elem++;
                    ptr->next = calloc(1, sizeof(*ptr));
                    if (!ptr->next) {
                        return;
                    }
                    ptr = ptr->next;
                    ptr->next = NULL;
                }
            }
        }
    }
    closedir(dir);
    ptr = list_root;
    char **base = calloc(count_elem, sizeof(*base));
    if (!base) {
        return;
    }   
    for(int i = 0; i < count_elem; ++i) {
        //base[i] = calloc(ptr->len, sizeof(*base[i]));
        //snprintf(base[i], ptr->len, "%s", ptr->str);
        base[i] = ptr->str;
        struct List *tmp = ptr;
        ptr = ptr->next;
        free(tmp);
    }
    free(ptr);
    //sort
    qsort(base, count_elem, sizeof(char *), (int(*) (const void *, const void *)) compare);
    //end
    //rec
    for(int i = 0; i < count_elem; ++i) {
        int tmp_len = strlen(base[i]);
        int len = dir_name_len + 2 + tmp_len;
        snprintf(path, len, "%s/%s", root, base[i]);
        //printf("cd %s\n", base[i]);
        dir_tree(path, base[i]);
        free(base[i]);
    }
    free(base);
    if (r_t) {
        printf("cd %s\n", "..");
    }
    return;
}

int
main(int argc, char **argv)
{
    char *dirname = argv[1];
    dir_tree(dirname, NULL);

}
