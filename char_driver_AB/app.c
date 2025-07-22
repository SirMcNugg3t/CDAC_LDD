#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd = open("/dev/char_case", O_RDWR);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    char input = 'A';
    char output;

    write(fd, &input, 1);
    read(fd, &output, 1);

    printf("Sent: %c, Received: %c\n", input, output);

    close(fd);
    return 0;
}

