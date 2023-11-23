#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#define DEFAULT_BUFFER_SIZE 1024

ssize_t myCopy(const char *src, const char *dst, size_t buffer_size) {
    int source_fd, dest_fd;
    ssize_t bytes_written = 0;
    char buffer[buffer_size];

    source_fd = open(src, O_RDONLY);
    if (source_fd == -1) {
        perror("Source file cannot be opened for reading!");
        return -1;
    }

    if ((dest_fd = open(dst, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == -1) {
        perror("Destination file cannot be opened for writing!");
        close(source_fd);
        return -1;
    }

    ssize_t bytes_read;
    while ((bytes_read = read(source_fd, buffer, buffer_size)) > 0) {
        bytes_written = write(dest_fd, buffer, (size_t)bytes_read);
        if (bytes_written != bytes_read) {
            perror("Write error!");
            close(source_fd);
            close(dest_fd);
            return -1;
        }
    }

    close(source_fd);
    close(dest_fd);

    return bytes_written;
}

int main(int argc, char **argv) {
    size_t buffer_size = DEFAULT_BUFFER_SIZE;
    const char *src;
    const char *dst;

    if (argc == 3) {
        src = argv[1];
        dst = argv[2];
    } else if (argc == 5 && strcmp(argv[1], "-b") == 0) {
        buffer_size = atoi(argv[2]);
        src = argv[3];
        dst = argv[4];
    } else {
        printf("Usage: mycopy [-b buffer_size] source_file destination_file\n");
        exit(1);
    }

    ssize_t bytes_copied = myCopy(src, dst, buffer_size);
    if (bytes_copied != -1) {
        printf("File copied successfully. Bytes copied: %zd\n", bytes_copied);
    } else {
        printf("File copy failed.\n");
    }

    return EXIT_SUCCESS;
}

