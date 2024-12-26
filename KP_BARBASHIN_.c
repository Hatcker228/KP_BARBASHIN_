#define _CRT_SECURE_NO_WARNINGS 
#include <locale.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PI 3.14 
#define n 10

double F1(double x);  // функция 1
double F2(double x);  // функция 2
double F3(double x);  // функция 3
void A(double xmin, double dx, double* values);  // для задания значений через шаг
void B(double* array, int N, double xmin, double xmax);  // для случайных значений
void result(FILE* outputFile, double x, int func_num, double (*func)(double));  // вывод результата в файл
void menu(FILE* outputFile);  // главное меню
void additional_menu(FILE* outputFile, double* values, int N, double xmin, double xmax);  // дополнительные операции

double find_min(double (*func)(double), double* values, int N);  // нахождение минимального значения
double find_max(double (*func)(double), double* values, int N);  // нахождение максимального значения
double linear_search(double (*func)(double), double target, double xmin, double xmax, double dx);  // линейный поиск
double differentiate(double (*func)(double), double x, double epsilon);  // дифференцирование

int main() {
    setlocale(LC_CTYPE, "RUS");  // установка локали
    FILE* outputFile = fopen("output.cvs", "w");  // открытие файла для записи
    menu(outputFile);  // запуск главного меню
    fclose(outputFile);  // закрытие файла
    return 0;
}

void menu(FILE* outputFile) {
    int choice;

    while (1) {
        // вывод меню выбора режима
        printf("(Задание А) Если вы хотите получить значения функций F1, F2, F3, нажмите 1\n");
        printf("(Задание Б) Если вы хотите получить значения функций F1, F2, F3 от 10 сгенерированных чисел, нажмите 2\n");
        printf("(Задание В) Для генерации уникальных случайных значений, нажмите 3\n");
        printf("Для выхода, нажмите 0\n");
        scanf("%d", &choice);  // выбор пользователя

        switch (choice) {
        case 1:  // режим А
        {
            int func_choice;
            double x;
            printf("Выберите функцию:\n1 - F1\n2 - F2\n3 - F3\n");
            scanf("%d", &func_choice);
            printf("Введите значение х для вычислений:\n");
            scanf("%lf", &x);

            fprintf(outputFile, "Значения для выбранной функции:\n");
            fprintf(outputFile, "|x\t\t|\tF%d(x)\t\t|\n", func_choice);
            fprintf(outputFile, "_________________________\n");

            switch (func_choice) {  // выбор функции
            case 1:
                result(outputFile, x, 1, F1);  // вызов функции F1
                break;
            case 2:
                result(outputFile, x, 2, F2);  // вызов функции F2
                break;
            case 3:
                result(outputFile, x, 3, F3);  // вызов функции F3
                break;
            default:
                printf("Некорректный ввод.\n");
            }
        }
        break;
        case 2:  // режим Б
        {
            int func_choice;
            double xmin, dx;
            double values[n];

            printf("Выберите функцию для выполнения вычислений на интервале:\n1 - F1\n2 - F2\n3 - F3\n");
            scanf("%d", &func_choice);

            printf("Введите начальное значение (xmin): ");
            scanf("%lf", &xmin);

            printf("Введите шаг (dx): ");
            scanf("%lf", &dx);

            A(xmin, dx, values);  // генерация значений с шагом

            fprintf(outputFile, "Значения для выбранной функции на интервале:\n");
            fprintf(outputFile, "|x\t\t\t|\tF%d(x)\t\t|\n", func_choice);
            fprintf(outputFile, "_____________________________\n");

            for (int i = 0; i < n; i++) {
                switch (func_choice) {
                case 1:
                    fprintf(outputFile, "|%.5lf\t|\t%lf\t|\n", values[i], F1(values[i]));  // вывод значений для F1
                    break;
                case 2:
                    fprintf(outputFile, "|%.5lf\t|\t%lf\t|\n", values[i], F2(values[i]));  // вывод значений для F2
                    break;
                case 3:
                    fprintf(outputFile, "|%.5lf\t|\t%lf\t|\n", values[i], F3(values[i]));  // вывод значений для F3
                    break;
                default:
                    fprintf(outputFile, "Некорректный выбор функции.\n");
                    break;
                }
            }
            additional_menu(outputFile, values, n, xmin, xmin + n * dx);  // переход в дополнительное меню
        }
        break;
        case 3:  // режим В
        {
            int func_choice, N;
            double xmin, xmax;
            double* array;

            printf("Введите начальное значение (xmin): ");
            scanf("%lf", &xmin);

            printf("Введите конечное значение (xmax): ");
            scanf("%lf", &xmax);

            printf("Введите количество значений N: ");
            scanf("%d", &N);

            array = (double*)malloc(N * sizeof(double));  // выделение памяти для массива значений

            B(array, N, xmin, xmax);  // генерация уникальных случайных значений

            fprintf(outputFile, "Сгенерированные уникальные значения:\n");
            for (int i = 0; i < N; i++) {
                fprintf(outputFile, "%lf ", array[i]);  // вывод сгенерированных значений
            }
            fprintf(outputFile, "\n");

            printf("Выберите функцию для выполнения вычислений:\n1 - F1\n2 - F2\n3 - F3\n");
            scanf("%d", &func_choice);

            fprintf(outputFile, "Значения для выбранной функции:\n");
            fprintf(outputFile, "|x\t\t|\tF%d(x)\t\t|\n", func_choice);
            fprintf(outputFile, "_________________________\n");

            for (int i = 0; i < N; i++) {
                switch (func_choice) {
                case 1:
                    fprintf(outputFile, "|%.2lf\t|\t%lf\t|\n", array[i], F1(array[i]));  // вывод значений для F1
                    break;
                case 2:
                    fprintf(outputFile, "|%.2lf\t|\t%lf\t|\n", array[i], F2(array[i]));  // вывод значений для F2
                    break;
                case 3:
                    fprintf(outputFile, "|%.2lf\t|\t%lf\t|\n", array[i], F3(array[i]));  // вывод значений для F3
                    break;
                default:
                    fprintf(outputFile, "Некорректный выбор функции.\n");
                    break;
                }
            }
            free(array);  // освобождение памяти

            additional_menu(outputFile, array, N, xmin, xmax);  // переход в дополнительное меню
        }
        break;
        case 0:
            printf("Программа завершена.\n");
            return;  // завершение работы программы
        default:
            printf("Некорректный ввод. Попробуйте снова.\n");
            break;
        }
    }
}

void additional_menu(FILE* outputFile, double* values, int N, double xmin, double xmax) {
    int choice;

    while (1) {
        printf("\nДополнительные операции:\n");
        printf("1 - Определить минимальное и максимальное значения функции F1, F2 или F3\n");
        printf("2 - Линейный поиск значения аргумента при заданном значении функции\n");
        printf("3 - Дифференцирование функции в заданной точке\n");
        printf("0 - Вернуться в главное меню\n");
        scanf("%d", &choice);

        switch (choice) {
        case 1:  // нахождение min/max
        {
            int func_choice;
            printf("Выберите функцию для поиска минимального и максимального значения:\n");
            printf("1 - F1\n2 - F2\n3 - F3\n");
            scanf("%d", &func_choice);

            double* func_values = (double*)malloc(N * sizeof(double));
            for (int i = 0; i < N; i++) {
                switch (func_choice) {
                case 1:
                    func_values[i] = F1(values[i]);
                    break;
                case 2:
                    func_values[i] = F2(values[i]);
                    break;
                case 3:
                    func_values[i] = F3(values[i]);
                    break;
                default:
                    printf("Некорректный выбор функции.\n");
                    free(func_values);
                    return;
                }
            }

            printf("Минимальное значение функции: %.5lf\n", find_min(NULL, func_values, N));  // вывод min
            printf("Максимальное значение функции: %.5lf\n", find_max(NULL, func_values, N));  // вывод max
            free(func_values);
        }
        break;
        case 2:  // линейный поиск
        {
            double target;
            printf("Введите целевое значение функции: ");
            scanf("%lf", &target);
            double x = linear_search(F1, target, xmin, xmax, 0.01);  // поиск для F1
            printf("Найденное значение аргумента: %.5lf\n", x);
        }
        break;
        case 3:  // дифференцирование
        {
            double x;
            printf("Введите точку для дифференцирования: ");
            scanf("%lf", &x);
            double epsilon = 1e-5;
            double derivative = differentiate(F1, x, epsilon);  // дифференцирование для F1
            printf("Дифференциал функции в точке %.5lf: %.5lf\n", x, derivative);
        }
        break;
        case 0:
            return;  // возврат в главное меню
        default:
            printf("Некорректный выбор. Попробуйте снова.\n");
            break;
        }
    }
}

double find_min(double (*func)(double), double* values, int N) {
    double min = values[0];
    for (int i = 1; i < N; i++) {
        if (values[i] < min) {
            min = values[i];
        }
    }
    return min;  // нахождение минимального значения
}

double find_max(double (*func)(double), double* values, int N) {
    double max = values[0];
    for (int i = 1; i < N; i++) {
        if (values[i] > max) {
            max = values[i];
        }
    }
    return max;  // нахождение максимального значения
}

double linear_search(double (*func)(double), double target, double xmin, double xmax, double dx) {
    for (double x = xmin; x <= xmax; x += dx) {
        if (fabs(func(x) - target) < 1e-2) {  // допустимая погрешность
            return x;
        }
    }
    return NAN;  // если не найдено
}

double differentiate(double (*func)(double), double x, double epsilon) {
    return (func(x + epsilon) - func(x)) / epsilon;  // приближенное дифференцирование
}

double F1(double x) {
    return exp(x * cos((PI * x) / 4));  // первая функция
}

double F2(double x) {
    double sum = 0;
    if (x > -2) {
        for (int i = 1; i <= 5; i++) {
            sum += pow(x, -i) / (i + 2);  // вычисление для функции 2
        }
        return sum;
    }
    else {
        return pow(3 + x, 2 - x);  // другое выражение для функции 2
    }
}

double F3(double x) {
    double th;
    th = (exp(2 * x) - 1) / (exp(2 * x) + 1);  // третья функция

    return th;
}

void A(double xmin, double dx, double* values) {
    for (int i = 0; i < n; i++) {
        values[i] = xmin + i * dx;  // генерация значений
    }
}

void B(double* array, int N, double xmin, double xmax) {
    int count = 0;
    while (count < N) {
        double num = xmin + (double)rand() / RAND_MAX * (xmax - xmin);  // генерация случайных чисел
        int unique = 1;
        for (int i = 0; i < count; i++) {
            if (fabs(array[i] - num) < 1e-6) {  // проверка на уникальность
                unique = 0;
                break;
            }
        }
        if (unique) {
            array[count++] = num;  // добавление уникального значения
        }
    }
}

void result(FILE* outputFile, double x, int func_num, double (*func)(double)) {
    fprintf(outputFile, "|%.3lf\t|\t%lf\t|\n", x, func(x));  // вывод результата в файл
}
