#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <unistd.h>
#include <limits.h>
#include <fcntl.h>

char DEFTMPPATH[] = "/tmp";

int
main (int argc, char *argv[])
{
    //get path of tmp dir
    char *pathdir = getenv("XDG_RUNTIME_DIR");

    if (pathdir == NULL) {
        pathdir = getenv("TMPDIR");
        if (pathdir == NULL) {
            pathdir = DEFTMPPATH;
        }
    }

    //create unique name
    char *pathfile = calloc(PATH_MAX, sizeof(*pathfile));

    sprintf(pathfile, "%s/myscr_%d.py", pathdir, (int)getpid());

    //create file
    int fd = creat(pathfile, 0777);
    if (fd == -1) {
        return 1;
    }
    close(fd);
 
    //write script   
    FILE *file = fopen(pathfile, "w");
    fprintf(file, "#!/usr/bin/env python3\nimport os\nprint(%s", argv[1]);
    for (int i = 2; i < argc; i++) {
        fprintf(file, " * %s", argv[i]);
    }
    fprintf(file, ")\nos.remove(\"%s\")", pathfile);
    fclose(file);

    //start script
    execlp("/bin/sh", "sh", "-c", pathfile, (char*) NULL);
    _exit(2);
}
