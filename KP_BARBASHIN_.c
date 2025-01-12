#define _CRT_SECURE_NO_WARNINGS 
#include <locale.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PI 3.14 
#define n 10

double F1(double x);
double F2(double x);
double F3(double x);
void A(double xmin, double dx, double* values);
void B(double* array, int N, double xmin, double xmax);
void result(FILE* outputFile, double x, int func_num, double (*func)(double));
double find_min(double (*func)(double), double* values, int N);
double find_max(double (*func)(double), double* values, int N);
double linear_search(double (*func)(double), double target, double xmin, double xmax, double dx);
double differentiate(double (*func)(double), double x, double epsilon);

void menu(FILE* outputFile);

int main() {
    setlocale(LC_CTYPE, "RUS");
    FILE* outputFile = fopen("output.csv", "w");
    menu(outputFile);
    fclose(outputFile);
    return 0;
}

void menu(FILE* outputFile) {
    int choice;
    double values[n], xmin, xmax, dx, x, target, epsilon = 1e-5;
    int func_choice, N;
    double* array;

    while (1) {
        printf("\nМеню:\n");
        printf("1. Выбор функции\n");
        printf("2. Табуляция функции\n");
        printf("3. Генерация уникальных случайных значений\n");
        printf("4. Определение минимального и максимального значений\n");
        printf("5. Линейный поиск значения аргумента при заданном значении функции\n");
        printf("6. Дифференцирование функции в заданной точке\n");
        printf("7. Выход\n");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("Выберите функцию:\n1 - F1\n2 - F2\n3 - F3\n");
            scanf("%d", &func_choice);
            printf("Введите значение x:\n");
            scanf("%lf", &x);
            fprintf(outputFile, "|x\t\t|F%d(x)\t\t|\n", func_choice);
            fprintf(outputFile, "_________________________\n");
            switch (func_choice) {
            case 1: result(outputFile, x, 1, F1); break;
            case 2: result(outputFile, x, 2, F2); break;
            case 3: result(outputFile, x, 3, F3); break;
            default: printf("Некорректный выбор функции.\n");
            }
            break;
        case 2:
            printf("Выберите функцию:\n1 - F1\n2 - F2\n3 - F3\n");
            scanf("%d", &func_choice);
            printf("Введите начальное значение (xmin):\n");
            scanf("%lf", &xmin);
            printf("Введите шаг (dx):\n");
            scanf("%lf", &dx);
            A(xmin, dx, values);
            fprintf(outputFile, "|x\t\t|F%d(x)\t\t|\n", func_choice);
            fprintf(outputFile, "_________________________\n");
            for (int i = 0; i < n; i++) {
                switch (func_choice) {
                case 1: fprintf(outputFile, "|%.5lf\t|%lf\t|\n", values[i], F1(values[i])); break;
                case 2: fprintf(outputFile, "|%.5lf\t|%lf\t|\n", values[i], F2(values[i])); break;
                case 3: fprintf(outputFile, "|%.5lf\t|%lf\t|\n", values[i], F3(values[i])); break;
                default: printf("Некорректный выбор функции.\n"); break;
                }
            }
            break;
        case 3:
            printf("Введите начальное значение (xmin):\n");
            scanf("%lf", &xmin);
            printf("Введите конечное значение (xmax):\n");
            scanf("%lf", &xmax);
            printf("Введите количество значений N:\n");
            scanf("%d", &N);
            array = (double*)malloc(N * sizeof(double));
            B(array, N, xmin, xmax);
            fprintf(outputFile, "Сгенерированные уникальные значения:\n");
            for (int i = 0; i < N; i++) {
                fprintf(outputFile, "%lf\n", array[i]);
            }
            free(array);
            break;
        case 4:
            printf("Выберите функцию:\n1 - F1\n2 - F2\n3 - F3\n");
            scanf("%d", &func_choice);
            printf("Введите начальное значение (xmin):\n");
            scanf("%lf", &xmin);
            printf("Введите конечное значение (xmax):\n");
            scanf("%lf", &xmax);
            printf("Введите количество значений N:\n");
            scanf("%d", &N);

            array = (double*)malloc(N * sizeof(double));
            if (!array) {
                printf("Ошибка выделения памяти для массива.\n");
                return;
            }
            B(array, N, xmin, xmax);

            double (*selected_func)(double) = NULL;
            switch (func_choice) {
            case 1: selected_func = F1; break;
            case 2: selected_func = F2; break;
            case 3: selected_func = F3; break;
            default:
                printf("Некорректный выбор функции.\n");
                free(array);
                return;
            }

            double* func_values = (double*)malloc(N * sizeof(double));
            if (!func_values) {
                printf("Ошибка выделения памяти для массива значений функции.\n");
                free(array);
                return;
            }

            for (int i = 0; i < N; i++) {
                func_values[i] = selected_func(array[i]);
            }

            double min_value = find_min(selected_func, func_values, N);
            double max_value = find_max(selected_func, func_values, N);

            printf("Минимальное значение: %.5lf\n", min_value);
            printf("Максимальное значение: %.5lf\n", max_value);

            free(func_values);
            free(array);
            break;

        case 5:
            printf("Введите целевое значение функции:\n");
            scanf("%lf", &target);
            printf("Введите начальное значение (xmin):\n");
            scanf("%lf", &xmin);
            printf("Введите конечное значение (xmax):\n");
            scanf("%lf", &xmax);
            printf("Введите шаг (dx):\n");
            scanf("%lf", &dx);
            x = linear_search(F1, target, xmin, xmax, dx);
            printf("Найденное значение аргумента: %.5lf\n", x);
            break;
        case 6:
            printf("Введите точку для дифференцирования:\n");
            scanf("%lf", &x);
            printf("Производная функции F1 в точке %.5lf: %.5lf\n", x, differentiate(F1, x, epsilon));
            break;
        case 7:
            printf("Выход из программы.\n");
            return;
        default:
            printf("Некорректный выбор. Попробуйте снова.\n");
            break;
        }
    }
}

double F1(double x) {
     double y;
     y = exp(x * cos((PI * x) / 4));
     return y;
}
double F2(double x) {
    double sum = 0;
    double y;
    if (x > -2) {
        for (int i = 1; i <= 5; i++) sum += pow(x, -i) / (i + 2);
        return sum;
    }
    else  y = pow(3 + x, 2 - x); return y;
}
double F3(double x) {
    double y;
    y = (exp(2 * x) - 1) / (exp(2 * x) + 1);
    return y;
} 
void A(double xmin, double dx, double* values) {
    for (int i = 0; i < n; i++)
        values[i] = xmin + i * dx;
}
void B(double* array, int N, double xmin, double xmax) {
    int count = 0;
    while (count < N) {
        double num = xmin + (double)rand() / RAND_MAX * (xmax - xmin);
        int unique = 1;
        for (int i = 0; i < count; i++) {
            if (fabs(array[i] - num) < 1e-6) {
                unique = 0;
                break;
            }
        }
        if (unique) array[count++] = num;
    }
}
void result(FILE* outputFile, double x, int func_num, double (*func)(double)) {
    fprintf(outputFile, "|%.3lf\t|%lf\t|\n", x, func(x));
}
double find_min(double (*func)(double), double* values, int N) {
    double min = values[0];
    for (int i = 1; i < N; i++)
        if (values[i] < min)
            min = values[i];
    return min;
}
double find_max(double (*func)(double), double* values, int N) {
    double max = values[0];
    for (int i = 1; i < N; i++)
        if (values[i] > max)
            max = values[i];
    return max;
}
double linear_search(double (*func)(double), double target, double xmin, double xmax, double dx) {
    for (double x = xmin; x <= xmax; x += dx) {
        if (fabs(func(x) - target) < 1e-2)
            return x;
    }
    return NAN;
}
double differentiate(double (*func)(double), double x, double epsilon) {
    return (func(x + epsilon) - func(x)) / epsilon;  // приближенное дифференцирование
}
