/**
 * See http://marcansoft.com/blog/2009/12/making-firefox-play-nicely-with-laptop-mode/
 */

#define _GNU_SOURCE
#include <unistd.h>
#include <dlfcn.h>
#include <sys/stat.h>
#include <fcntl.h>

static int in_laptop_mode(void) {
    char buf[2];
    int fd;
    int l;
    fd = open("/proc/sys/vm/laptop_mode", O_RDONLY);
    if (fd < 0)
        return 0;
    l = read(fd, buf, 2);
    close(fd);
    if (l == 2 && buf[0] == '0' && buf[1] == '\n')
        return 0;
    else
        return 1;
}

int fsync(int fd) {
    static int (*_fsync)(int);
    if (!_fsync)
        _fsync = dlsym(RTLD_NEXT, "fsync");
    if (!in_laptop_mode())
        return _fsync(fd);
    else
        return 0;
}

int fdatasync(int fd) {
    static int (*_fdatasync)(int);

    if (!_fdatasync)
        _fdatasync = dlsym(RTLD_NEXT, "fdatasync");
    if (!in_laptop_mode())
        return _fdatasync(fd);
    else
        return 0;
}
