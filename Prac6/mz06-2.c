#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <limits.h>
#include <sys/types.h>
#include <string.h>
#include <ctype.h>

const char sample[] = ".exe";

enum
{
    SAM_LEN = sizeof(sample) - 1
};

int 
cmp_exe(char *str)
{
    int str_len = strlen(str);
    if (str_len > SAM_LEN) {
        for(int i = 0; i < SAM_LEN; ++i) {
            if (str[str_len - 1 - i] != sample[SAM_LEN - 1 - i]) {
                return 0;
            }
        }
        return 1;
    } else {
        return 0;
    }
}

int
main (int argc, char *argv[])
{
    DIR *dir = opendir(argv[1]);
    if (!dir) {
        return 1;
    }
    unsigned long long count = 0;
    struct dirent *cat;
    char path[PATH_MAX];
    struct stat tmp_f;
    while((cat = readdir(dir))){
        if (strcmp(cat->d_name, ".") && strcmp(cat->d_name, "..")) {
            snprintf(path, sizeof(path), "%s/%s", argv[1], cat->d_name);
            if (stat(path, &tmp_f) >= 0) { 
                if (S_ISREG(tmp_f.st_mode) && cmp_exe(path) && access(path, X_OK) == 0) {
                    count++;
                }
            }
        }
    }
    printf("%llu\n", count);
    closedir(dir);
}
