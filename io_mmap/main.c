#include <ctype.h>
#include <errno.h> // for errno
#include <fcntl.h> // for open
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h> // for perror
#include <sys/mman.h>
#include <sys/stat.h> // for stat
#include <sys/types.h>
#include <unistd.h> // for stat

int count_words(const char *file_path) {
    int fd = open(file_path, O_RDONLY);
    if (fd == -1) {
        perror("open failed");
        return -1;
    }

    struct stat st;
    if (fstat(fd, &st) == -1) {
        perror("stat failed");
        close(fd);
        return 1;
    }

    if (st.st_size == 0) {
        fprintf(stderr, "size is null\n");
        close(fd);
        return 1;
    }

    size_t len = (size_t)st.st_size;
    const char *p = mmap(NULL, len, PROT_READ, MAP_PRIVATE, fd, 0);
    if (p == MAP_FAILED) {
        perror("mmap failed");
        close(fd);
        return -1;
    }

    size_t word_number = 0;
    bool in_word = false;
    for (size_t i = 0; i < len; i++) {
        if (in_word && isspace(p[i])) {
            word_number++;
            in_word = false;
        } else if (!in_word && isalpha(p[i])) {
            in_word = true;
        }
    }

    printf("would count %zu\n", word_number);

    return 0;
}
int main(void) {
    int err = count_words("./test.txt");
    if (err == -1) {
        fprintf(stderr, "Failed to count words\n");
        return 1;
    }

    return 0;
}
