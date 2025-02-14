#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>

void *lib_handle = NULL;
char *(*translation)(long) = NULL;
int (*GCD)(int, int) = NULL;
char current_system[20] = "";

void load_library(const char *lib_path)
{
    if (lib_handle != NULL)
    {
        dlclose(lib_handle);
    }

    lib_handle = dlopen(lib_path, RTLD_LAZY);
    if (!lib_handle)
    {
        fprintf(stderr, "Ошибка загрузки библиотеки: %s\n", dlerror());
        exit(1);
    }

    GCD = dlsym(lib_handle, "GCD");
    translation = dlsym(lib_handle, "translation");

    char *error = dlerror();
    if (error != NULL)
    {
        fprintf(stderr, "Ошибка загрузки функций: %s\n", error);
        dlclose(lib_handle);
        exit(1);
    }

    if (strstr(lib_path, "binary") != NULL)
    {
        strcpy(current_system, "двоичной");
    }
    else if (strstr(lib_path, "ternary") != NULL)
    {
        strcpy(current_system, "троичной");
    }
}

int main()
{
    load_library("./libgcd_euclid_and_binary.so");

    char command[100];
    int running = 1;

    while (running)
    {
        printf("\nВведите команду (1 для НОД, 2 для перевода, 0 для смены библиотеки, q для выхода): ");
        fgets(command, sizeof(command), stdin);

        if (command[0] == '1')
        {
            int a, b;
            printf("Введите два числа для НОД: ");
            scanf("%d %d", &a, &b);
            getchar();
            printf("НОД: %d\n", GCD(a, b));
        }
        else if (command[0] == '2')
        {
            long x;
            printf("Введите число для перевода: ");
            scanf("%ld", &x);
            getchar();
            printf("Число %ld в %s системе: %s\n", x, current_system, translation(x));
        }
        else if (command[0] == '0')
        {
            printf("Введите путь к новой библиотеке: ");
            char lib_path[256];
            fgets(lib_path, sizeof(lib_path), stdin);
            lib_path[strcspn(lib_path, "\n")] = '\0';

            load_library(lib_path);
            printf("Библиотека переключена.\n", current_system);
        }
        else if (command[0] == 'q' || command[0] == 'Q')
        {
            printf("Завершение программы.\n");
            running = 0;
        }
        else
        {
            printf("Неверная команда. Попробуйте снова.\n");
        }
    }

    dlclose(lib_handle);
    return 0;
}
