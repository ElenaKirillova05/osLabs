#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

#define SHARED_MEMORY_FILE "/shared_memory"
#define MEMORY_SIZE 1024

volatile sig_atomic_t ready = 0;
volatile sig_atomic_t exit_flag = 0;

void signal_handler(int sig) {
    ready = 1;
}

void exit_handler(int sig) {
    exit_flag = 1;
}

int main() {
    signal(SIGUSR1, signal_handler);
    signal(SIGINT, exit_handler);

    int shared_fd = shm_open(SHARED_MEMORY_FILE, O_RDWR, 0666);
    if (shared_fd == -1) {
        perror("shm_opeфn failed");
        exit(EXIT_FAILURE);
    }

    char *shared_memory = mmap(NULL, MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shared_fd, 0);
    if (shared_memory == MAP_FAILED) {
        perror("mmap failed");
        exit(EXIT_FAILURE);
    }

    while (1) {
        while (!ready && !exit_flag) pause();

        if (exit_flag) break;

        for (int i = 0; shared_memory[i]; i++) {
            shared_memory[i] = toupper(shared_memory[i]);
        }

        kill(getppid(), SIGUSR1);

        ready = 0;
    }

    munmap(shared_memory, MEMORY_SIZE);
    close(shared_fd);
    return 0;
}
