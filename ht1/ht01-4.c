#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>

enum
{
    DEC = 10,
    FUNC_BUF_SIZE = 64
};

struct FUNCBUF
{
    char func_opt[FUNC_BUF_SIZE];
};

int
main(int argc, char **argv)
{
    void *handle = dlopen(argv[1], RTLD_LAZY);
    if (handle == NULL) {
        return 1;
    }
    
    void *sum = dlsym(handle, argv[2]);
    if (sum == NULL) {
        return 1;
    }
    
    struct FUNCBUF buf;
    char *mas = buf.func_opt;
    char *str_sig = argv[3];
    int len_sig_func = strlen(str_sig);
    for(int i = 1; i < len_sig_func; ++i) {
        if (str_sig[i] == 's') {
            *(char **) mas = argv[3 + i];
            mas += sizeof(char *);
            continue; 
        }
        if (str_sig[i] == 'i') {
            *(int *) mas = (int)strtol(argv[3 + i], NULL, DEC);
            mas += sizeof(int);
            continue;
        }
        if (str_sig[i] == 'd') {
            *(double *) mas = strtod(argv[3 + i], NULL);
            mas += sizeof(double);
            continue;
        }
    }

    if (str_sig[0] == 'v') {
        ((void (*)(struct FUNCBUF)) sum)(buf);
    } else if (str_sig[0] == 'i') {
        printf("%d\n", ((int (*)(struct FUNCBUF)) sum)(buf));
    } else if (str_sig[0] == 'd') {
        printf("%.10g\n", ((double (*)(struct FUNCBUF)) sum)(buf));
    } else if (str_sig[0] == 's') {
        printf("%s\n", ((char * (*)(struct FUNCBUF)) sum)(buf));
    } else {
        return 1;
    }
    dlclose(handle);
}
