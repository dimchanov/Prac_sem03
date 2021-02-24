#include <stdio.h>
#include <ctype.h>

int
mystrspccmp(const char *str1, const char *str2)
{
    unsigned char *s1 = (unsigned char*) str1;
    unsigned char *s2 = (unsigned char*) str2;
    unsigned char tmp1 = (unsigned char) *s1;
    unsigned char tmp2 = (unsigned char) *s2;
    s1++;
    s2++;
    while (1) {
        while (isspace(tmp1)) {
            tmp1 = (unsigned char) *s1;
            s1++;
        }
        while (isspace(tmp2)) {
            tmp2 = (unsigned char) *s2;
            s2++;
        }
        if (tmp1 == '\0' || tmp1 != tmp2) {
           return tmp1 - tmp2;
        }
        tmp1 = (unsigned char) *s1;
        s1++;
        tmp2 = (unsigned char) *s2;
        s2++;
    }
}

int
main(){
   printf("%d\n", mystrspccmp("aaaaa", "aaaab"));
}
