#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

struct List
{
    struct List *next;
    struct List *last;
    char name[NAME_MAX + 1];
};

void 
del_useless(struct List *root)
{
    struct List *ptr = root;
    while(ptr) {
        struct List *tmp1;
        if (strcmp(ptr->name, "..") == 0) {
            tmp1 = ptr;
            ptr = tmp1->last;
            ptr->next = tmp1->next;
            if (tmp1->next) {
                tmp1->next->last = ptr;
            }
            free(tmp1);
            if (strcmp(ptr->name, "/") != 0) {
                tmp1 = ptr;
                ptr = tmp1->last;
                ptr->next = tmp1->next;
                if (tmp1->next) {
                    tmp1->next->last = ptr;
                }
                free(tmp1);
            }
        } else {
            if (strcmp(ptr->name, ".") == 0) {
                tmp1 = ptr;
                ptr = tmp1->last;
                ptr->next = tmp1->next;
                if (tmp1->next) {
                    tmp1->next->last = ptr;
                }
                free(tmp1);
            }
        }
        ptr = ptr->next;
    }
    return;
}
int
d_path_to_list(char *path, struct List *tmp)
{
    int i = 0;
    while(*path != '/' && *path != '\0') {
        tmp->name[i] = *path;
        i++;
        path++;
    }
    return i;
}
struct List * path_to_list(char *path)
{
    struct List *root;
    if (!(root = malloc(sizeof(*root)))) {
        fprintf(stderr, "Allocation error");
        exit(1);
    }
    memset(root->name, '\0', sizeof(root->name));
    root->next = NULL;
    root->last = NULL;
    root->name[0] = *path;
    path++;
    struct List *temp = root;
    while(*path != '\0') {
        if (*path == '/') {
            path++;
        }
        if (*path == '\0') {
            break;
        }
        struct List *ptr;
        if (!(ptr = malloc(sizeof(*ptr)))) {
            fprintf(stderr, "Allocation error");
            exit(1);
        }
        memset(ptr->name, '\0', sizeof(ptr->name));
        ptr->next = NULL;
        ptr->last = temp;
        temp->next = ptr;
        path += d_path_to_list(path, ptr);
        temp = ptr;
    }
    return root;
}



char *relativize_path(const char *path1, const char *path2)
{
    char *str1 = (char *)path1;
    char *str2 = (char *)path2; 
    struct List *root1 = path_to_list(str1);
    del_useless(root1);
    struct List *root2 = path_to_list(str2);
    del_useless(root2);
    struct List *ptr1 = root1;
    struct List *ptr2 = root2;


    while (ptr1 && ptr2 && strcmp(ptr1->name, ptr2->name) == 0) {
        struct List *tmp = ptr1;
        ptr1 = ptr1->next;
        free(tmp);
        tmp = ptr2;
        ptr2 = ptr2->next;
        free(tmp);
    }
    char *path = calloc(PATH_MAX + 1, sizeof(char));
    while(ptr1 && ptr1->next) {
        int len = strlen(path);
        path[len] = '/';
        path[len + 1] = '.';
        path[len + 2] = '.';
        struct List *tmp = ptr1;
        ptr1 = ptr1->next;
        free(tmp);
    }
    if (ptr1) {
        free(ptr1);
    }
    while(ptr2) {
        int len = strlen(path);
        path[len] = '/';
        for(int i = 0; i < strlen(ptr2->name); ++i) {
            path[len + i + 1] = ptr2->name[i];

        }
        struct List *tmp = ptr2;
        ptr2 = ptr2->next;
        free(tmp);
    }
    int new_len = strlen(path);
    char *new_path = calloc(new_len, sizeof(char));
    for(int i = 0; i < new_len; ++i) {
        new_path[i] = path[i + 1];
    }
    free(path);
    return new_path;
}
