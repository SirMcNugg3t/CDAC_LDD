#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    char buf[100];
    int fd = open("/dev/mychardev", O_RDWR);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    write(fd, "CDAC Embedded Linux", 20);
    lseek(fd, 0, SEEK_SET);  // Reset position to start
    read(fd, buf, 20);

    buf[20] = '\0';
    printf("Read from driver: %s\n", buf);

    close(fd);
    return 0;
}
