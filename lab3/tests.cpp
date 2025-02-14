#include <gtest/gtest.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <cstring>

#define SHARED_MEMORY_FILE "/shared_memory"
#define MEMORY_SIZE 1024

class SharedMemoryTest : public ::testing::Test {
protected:
    char *shared_memory;
    int shared_fd;

    void SetUp() override {
        shared_fd = shm_open(SHARED_MEMORY_FILE, O_CREAT | O_RDWR, 0666);
        if (shared_fd == -1) {
            perror("shm_open failed");
            exit(EXIT_FAILURE);
        }
        ftruncate(shared_fd, MEMORY_SIZE);

        shared_memory = (char *)mmap(NULL, MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shared_fd, 0);
        if (shared_memory == MAP_FAILED) {
            perror("mmap failed");
            exit(EXIT_FAILURE);
        }
    }

    void TearDown() override {
        munmap(shared_memory, MEMORY_SIZE);
        close(shared_fd);
        shm_unlink(SHARED_MEMORY_FILE);
    }
};

TEST_F(SharedMemoryTest, MemoryOperationsTest) {
    const char* test_input = "hello world";
    strncpy(shared_memory, test_input, MEMORY_SIZE);

    EXPECT_STREQ(shared_memory, test_input);
}

TEST_F(SharedMemoryTest, SignalHandlingTest) {
    const char* input_data = " hello  world ";
    strncpy(shared_memory, input_data, MEMORY_SIZE);

    for (int i = 0; shared_memory[i]; i++) {
        shared_memory[i] = toupper(shared_memory[i]);
    }

    EXPECT_STREQ(shared_memory, " HELLO  WORLD ");

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

    EXPECT_STREQ(shared_memory, " HELLO WORLD ");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
