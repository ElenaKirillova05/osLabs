#include <stdio.h>
#include <stdlib.h>
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
    // Устанавливаем обработчик сигнала SIGUSR2
    signal(SIGUSR2, signal_handler);
    // Устанавливаем обработчик сигнала SIGINT для завершения программы
    signal(SIGINT, exit_handler);

    // Открываем файл общей памяти
    int shared_fd = shm_open(SHARED_MEMORY_FILE, O_RDWR, 0666);
    if (shared_fd == -1) {
        perror("shm_open failed");
        exit(EXIT_FAILURE);
    }

    // Отображаем файл общей памяти в адресное пространство процесса
    char *shared_memory = mmap(NULL, MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shared_fd, 0);
    if (shared_memory == MAP_FAILED) {
        perror("mmap failed");
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Ждем сигнала готовности
        while (!ready && !exit_flag) pause();

        if (exit_flag) break;

        // Указатели для копирования строки
        char *src = shared_memory;
        char *dest = shared_memory;
        int last_was_space = 0; 

        while (*src) {
            if (*src == ' ') {
                if (!last_was_space) {
                    *dest++ = ' ';
                    last_was_space = 1;
                }
            } else {
                *dest++ = *src;
                last_was_space = 0;
            }
            src++;
        }
        *dest = '\0'; 

        // Отправляем сигнал родительскому процессу
        kill(getppid(), SIGUSR2);

        // Сбрасываем флаг готовности
        ready = 0;
    }

    // Освобождаем память
    munmap(shared_memory, MEMORY_SIZE);
    close(shared_fd);
    return 0;
}
