#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdint.h>

#define SYM_ADDR 0x10000000
#define SYM_INPUT_SIZE 16

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s <input>\n", argv[0]);
        return 1;
    }

    // Map memory at symbolic trigger address
    void *mapped = mmap((void *)SYM_ADDR, 0x1000,
                        PROT_READ | PROT_WRITE,
                        MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED,
                        -1, 0);
    if (mapped == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    // Read input file directly into symbolic address
    FILE *fp = fopen(argv[1], "rb");
    if (!fp || fread(mapped, 1, SYM_INPUT_SIZE, fp) != SYM_INPUT_SIZE) {
        fprintf(stderr, "error reading input\n");
        return 1;
    }
    fclose(fp);

    // Trigger symbolic mode by touching the mapped memory
    volatile uint8_t *trigger = (uint8_t *)mapped;
    uint8_t dummy = *trigger;

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return 1;
    }

    int level = *((uint8_t *)SYM_ADDR);          // symbolic byte 0
    int optname = *((uint8_t *)SYM_ADDR + 1);    // symbolic byte 1
    int optval = *(int *)(SYM_ADDR + 2);         // symbolic 4 bytes starting at byte 2
    socklen_t optlen = sizeof(optval);

    setsockopt(sockfd, level, optname, &optval, optlen);

    close(sockfd);
    return 0;
}

