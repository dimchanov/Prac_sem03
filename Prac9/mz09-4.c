#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <limits.h>
#include<stdlib.h>

int
main(int argc, char **argv)
{
    if (argc < 2) {
        return 1;
    }
    const char *fpath;
    if (!(fpath = getenv("XDG_RUNTIME_DIR"))) {
        if (!(fpath = getenv("TMPDIR"))) {
            fpath = "/tmp";
        }
    }

    char file_path[PATH_MAX];
    pid_t pid;
    pid = getpid();
    snprintf(file_path, sizeof(file_path), "%s/scr%d.py", fpath, pid);

    int fd = open(file_path, O_RDWR | O_TRUNC | O_CREAT, 0700);
    if (fd == -1) {
        return 1;
    }
    int size_s1 = strlen("#! /usr/bin/python3\nimport os\nprint(");
    if (write(fd, "#! /usr/bin/python3\nimport os\nprint(", size_s1) != size_s1) {
        return 1;
    }
    for(int i = 1; i < argc - 1; ++i) {
        int size = strlen(argv[i]);
        if (write(fd, argv[i], size) != size) {
            return 1;
        }
        if (write(fd, "*", 1) != 1) {
            return 1;
        }
    }
    if (write(fd, argv[argc - 1], strlen(argv[argc - 1])) != strlen(argv[argc - 1])) {
        return 1;
    }
    if (write(fd, ")", 1) != 1) {
        return 1;
    }
    int size_s2 = strlen("\nos.remove(\"");
    if (write(fd, "\nos.remove(\"", size_s2) != size_s2) {
        return 1;
    }
    if (write(fd, file_path, strlen(file_path)) != strlen(file_path)) {
        return 1;
    }
    if (write(fd, "\")", 2) != 2) {
        return 1;
    }
    close(fd);

    execl("/bin/sh", "sh", "-c", file_path, NULL);
    return 1;

}
