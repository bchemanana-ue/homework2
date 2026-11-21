#include <stdio.h>

#define MAX_SIZE 100

typedef struct {
    int id;
    int score;
} Result;

/* Функция загружает данные из файла results.txt.
   Возвращает 1, если загрузка прошла успешно.
   Возвращает 0, если произошла ошибка. */
int load(Result data[], int *size) {
    FILE *file;
    int id;
    int score;
    int i;
    int read_count;

    *size = 0;

    file = fopen("results.txt", "r");

    if (file == NULL) {
        printf("Load error\n");
        return 0;
    }

    while (1) {
        read_count = fscanf(file, "%d %d", &id, &score);

        /* Если достигнут конец файла, загрузка закончена. */
        if (read_count == EOF) {
            break;
        }

        /* Если в файле не хватает одного из двух чисел,
           например есть только id без score, это ошибка. */
        if (read_count != 2) {
            printf("Load error\n");
            fclose(file);
            return 0;
        }

        /* Проверяем, что id находится в разрешённом диапазоне. */
        if (id < 1 || id > 9999) {
            printf("Load error\n");
            fclose(file);
            return 0;
        }

        /* Проверяем, что балл находится в разрешённом диапазоне. */
        if (score < 0 || score > 100) {
            printf("Load error\n");
            fclose(file);
            return 0;
        }

        /* Проверяем, что такой id ещё не встречался. */
        for (i = 0; i < *size; i++) {
            if (data[i].id == id) {
                printf("Load error\n");
                fclose(file);
                return 0;
            }
        }

        /* Проверяем, что массив не переполнится. */
        if (*size >= MAX_SIZE) {
            printf("Load error\n");
            fclose(file);
            return 0;
        }

        data[*size].id = id;
        data[*size].score = score;
        (*size)++;
    }

    fclose(file);
    return 1;
}

/* Функция выводит все записи в исходном порядке. */
void show(const Result data[], int size) {
    int i;

    if (size == 0) {
        printf("No data\n");
        return;
    }

    for (i = 0; i < size; i++) {
        printf("%d %d\n", data[i].id, data[i].score);
    }
}

/* Функция выводит статистику по всем записям. */
void statistics(const Result data[], int size) {
    int i;
    int passed;
    int min_index;
    int max_index;
    double sum;
    double average;

    if (size == 0) {
        printf("No data\n");
        return;
    }

    sum = 0.0;
    passed = 0;
    min_index = 0;
    max_index = 0;

    for (i = 0; i < size; i++) {
        sum = sum + data[i].score;

        if (data[i].score >= 60) {
            passed++;
        }

        /* При одинаковом минимальном балле оставляем первую запись. */
        if (data[i].score < data[min_index].score) {
            min_index = i;
        }

        /* При одинаковом максимальном балле оставляем первую запись. */
        if (data[i].score > data[max_index].score) {
            max_index = i;
        }
    }

    average = sum / size;

    printf("Count: %d\n", size);
    printf("Average: %.2f\n", average);
    printf("Passed: %d\n", passed);
    printf("Min: %d %d\n", data[min_index].id, data[min_index].score);
    printf("Max: %d %d\n", data[max_index].id, data[max_index].score);
}

/* Функция ищет запись по коду id.
   Если запись найдена, возвращает её номер в массиве.
   Если записи нет, возвращает -1. */
int find(const Result data[], int size, int id) {
    int i;

    for (i = 0; i < size; i++) {
        if (data[i].id == id) {
            return i;
        }
    }

    return -1;
}
/* Индивидуальный вариант 6.
   Функция выводит записи с нечётным id.
   Исходный массив не изменяется. */
void individual(const Result data[], int size) {
    int i;
    int found;

    if (size == 0) {
        printf("No data\n");
        return;
    }

    found = 0;

    for (i = 0; i < size; i++) {
        /* Остаток от деления на 2 равен 1 только у нечётных чисел. */
        if (data[i].id % 2 != 0) {
            printf("%d %d\n", data[i].id, data[i].score);
            found = 1;
        }
    }

    /* Если нечётных кодов нет, выводим No matches. */
    if (found == 0) {
        printf("No matches\n");
    }
}

/* Функция сохраняет все записи в файл report.txt. */
void save(const Result data[], int size) {
    FILE *file;
    int i;

    file = fopen("report.txt", "w");

    if (file == NULL) {
        printf("Save error\n");
        return;
    }

    for (i = 0; i < size; i++) {
        fprintf(file, "%d %d\n", data[i].id, data[i].score);
    }

    if (fclose(file) != 0) {
        printf("Save error\n");
        return;
    }
}

int main(void) {
    Result data[MAX_SIZE];
    int size;
    int command;
    int id;
    int index;
    int r_count;

    if (load(data, &size) == 0) {
        return 1;
    }

    while (1) {
        printf("Enter command: ");

        r_count = scanf("%d", &command);

        /* Если вместо команды введены буквы,
           программа сообщает об ошибке и завершается. */
        if (r_count != 1) {
            printf("Input error\n");
            return 1;
        }

        if (command == 0) {
            break;
        }
        else if (command == 1) {
            show(data, size);
        }
        else if (command == 2) {
            statistics(data, size);
        }
        else if (command == 3) {
            printf("Enter id: ");

            r_count = scanf("%d", &id);

            if (r_count != 1) {
                printf("Input error\n");
                return 1;
            }

            index = find(data, size, id);

            if (index == -1) {
                printf("Not found\n");
            }
            else {
                printf("%d %d\n", data[index].id, data[index].score);
            }
        }
        else if (command == 4) {
            individual(data, size);
        }
        else if (command == 5) {
            save(data, size);
        }
        else {
            printf("Unknown command\n");
        }
    }

    return 0;
}