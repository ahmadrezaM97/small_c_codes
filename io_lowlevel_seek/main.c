#include <errno.h>
#include <stdio.h>
#include <unistd.h>

// file control options
#include <fcntl.h>

#include <string.h>

int file_with_holes(const char *path) {

    int fd = open(path, O_RDWR | O_CREAT, 0666);
    if (fd == -1) {
        perror("open failed");
        return -1;
    }

    char *content = "something";
    for (int i = 0; i < 5; i++) {
        int n = write(fd, content, strlen(content));
        if (n == -1) {
            perror("write failed");
            return -1;
        }
    }

    int ret = lseek(fd, 1024*1024*10, SEEK_CUR);
    if (ret == -1) {
        perror("lseek failed");
        return -1;
    }

    content = "after seek";
    for (int i = 0; i < 5; i++) {
        int n = write(fd, content, strlen(content));
        if (n == -1) {
            perror("write failed");
            return -1;
        }
    }

    return 0;
}

int tail_file(const char *path, int n_lines) {
    int fd = open(path, O_RDONLY);
    if (fd == -1) {
        perror("open failed");
        return -1;
    }

    int ret = lseek(fd, -100, SEEK_END);
    if (ret == -1) {
        perror("lseek failed");
        return -1;
    }

    char buffer[1024];
    int ssize = read(fd, buffer, sizeof(buffer) - 1);
    if (ssize == -1) {
        perror("read failed");
        return -1;
    }

    buffer[ssize] = '\0';

    printf("'%s'", buffer);

    return 0;
}

int main(void) {

    int n = 10;
    char *path = "./test1.txt";

    int ern = file_with_holes(path );
    if (ern == -1) {
        perror("tail_file failed");
        return -1;
    }

    return 0;
}
