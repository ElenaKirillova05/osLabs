#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define SHARED_MEMORY_FILE "/shared_memory"
#define MEMORY_SIZE 1024

// Флаги для отслеживания завершения работы дочерних процессов
volatile sig_atomic_t child1_finished = 0;
volatile sig_atomic_t child2_finished = 0;

// Обработчики сигналов
void signal_handler_child1(int sig) {
    child1_finished = 1;
}

void signal_handler_child2(int sig) {
    child2_finished = 1;
}

int main() {
    // Устанавливаем обработчики сигналов
    signal(SIGUSR1, signal_handler_child1);
    signal(SIGUSR2, signal_handler_child2);

    // Создаем и открываем файл общей памяти
    int shared_fd = shm_open(SHARED_MEMORY_FILE, O_CREAT | O_RDWR, 0666);
    if (shared_fd == -1) {
        perror("shm_open failed");
        exit(EXIT_FAILURE);
    }
    ftruncate(shared_fd, MEMORY_SIZE);

    // Отображаем файл общей памяти в адресное пространство процесса
    char *shared_memory = mmap(NULL, MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shared_fd, 0);
    if (shared_memory == MAP_FAILED) {
        perror("mmap failed");
        exit(EXIT_FAILURE);
    }

    pid_t child1_pid = fork();
    if (child1_pid == -1) {
        perror("fork for child1 failed");
        exit(EXIT_FAILURE);
    }

    if (child1_pid == 0) {
        execl("./child1", "./child1", NULL);
        perror("execl for child1 failed");
        exit(EXIT_FAILURE);
    }

    pid_t child2_pid = fork();
    if (child2_pid == -1) {
        perror("fork for child2 failed");
        exit(EXIT_FAILURE);
    }

    if (child2_pid == 0) {
        execl("./child2", "./child2", NULL);
        perror("execl for child2 failed");
        exit(EXIT_FAILURE);
    }

    while (1) {
        printf("Enter the line (or 0 to end the program): ");
        if (fgets(shared_memory, MEMORY_SIZE, stdin) == NULL) break;

        if (shared_memory[0] == '0' && (shared_memory[1] == '\n' || shared_memory[1] == '\0')) break;

        // Сбрасываем флаги завершения
        child1_finished = 0;
        child2_finished = 0;

        kill(child1_pid, SIGUSR1);
        // Ждем завершения обработки первого дочернего процесса
        while (!child1_finished) pause();

        kill(child2_pid, SIGUSR2);
        // Ждем завершения обработки второго дочернего процесса
        while (!child2_finished) pause();

        printf("Result: %s\n", shared_memory);
    }

    // Отправляем сигнал SIGINT дочерним процессам для корректного завершения
    kill(child1_pid, SIGINT);
    kill(child2_pid, SIGINT);

    waitpid(child1_pid, NULL, 0);
    waitpid(child2_pid, NULL, 0);

    // Освобождаем ресурсы
    munmap(shared_memory, MEMORY_SIZE);
    shm_unlink(SHARED_MEMORY_FILE);
    return 0;
}
