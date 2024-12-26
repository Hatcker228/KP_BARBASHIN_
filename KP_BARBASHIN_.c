#define _CRT_SECURE_NO_WARNINGS 
#include <locale.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PI 3.14 
#define n 10

double F1(double x);  // ������� 1
double F2(double x);  // ������� 2
double F3(double x);  // ������� 3
void A(double xmin, double dx, double* values);  // ��� ������� �������� ����� ���
void B(double* array, int N, double xmin, double xmax);  // ��� ��������� ��������
void result(FILE* outputFile, double x, int func_num, double (*func)(double));  // ����� ���������� � ����
void menu(FILE* outputFile);  // ������� ����
void additional_menu(FILE* outputFile, double* values, int N, double xmin, double xmax);  // �������������� ��������

double find_min(double (*func)(double), double* values, int N);  // ���������� ������������ ��������
double find_max(double (*func)(double), double* values, int N);  // ���������� ������������� ��������
double linear_search(double (*func)(double), double target, double xmin, double xmax, double dx);  // �������� �����
double differentiate(double (*func)(double), double x, double epsilon);  // �����������������

int main() {
    setlocale(LC_CTYPE, "RUS");  // ��������� ������
    FILE* outputFile = fopen("output.cvs", "w");  // �������� ����� ��� ������
    menu(outputFile);  // ������ �������� ����
    fclose(outputFile);  // �������� �����
    return 0;
}

void menu(FILE* outputFile) {
    int choice;

    while (1) {
        // ����� ���� ������ ������
        printf("(������� �) ���� �� ������ �������� �������� ������� F1, F2, F3, ������� 1\n");
        printf("(������� �) ���� �� ������ �������� �������� ������� F1, F2, F3 �� 10 ��������������� �����, ������� 2\n");
        printf("(������� �) ��� ��������� ���������� ��������� ��������, ������� 3\n");
        printf("��� ������, ������� 0\n");
        scanf("%d", &choice);  // ����� ������������

        switch (choice) {
        case 1:  // ����� �
        {
            int func_choice;
            double x;
            printf("�������� �������:\n1 - F1\n2 - F2\n3 - F3\n");
            scanf("%d", &func_choice);
            printf("������� �������� � ��� ����������:\n");
            scanf("%lf", &x);

            fprintf(outputFile, "�������� ��� ��������� �������:\n");
            fprintf(outputFile, "|x\t\t|\tF%d(x)\t\t|\n", func_choice);
            fprintf(outputFile, "_________________________\n");

            switch (func_choice) {  // ����� �������
            case 1:
                result(outputFile, x, 1, F1);  // ����� ������� F1
                break;
            case 2:
                result(outputFile, x, 2, F2);  // ����� ������� F2
                break;
            case 3:
                result(outputFile, x, 3, F3);  // ����� ������� F3
                break;
            default:
                printf("������������ ����.\n");
            }
        }
        break;
        case 2:  // ����� �
        {
            int func_choice;
            double xmin, dx;
            double values[n];

            printf("�������� ������� ��� ���������� ���������� �� ���������:\n1 - F1\n2 - F2\n3 - F3\n");
            scanf("%d", &func_choice);

            printf("������� ��������� �������� (xmin): ");
            scanf("%lf", &xmin);

            printf("������� ��� (dx): ");
            scanf("%lf", &dx);

            A(xmin, dx, values);  // ��������� �������� � �����

            fprintf(outputFile, "�������� ��� ��������� ������� �� ���������:\n");
            fprintf(outputFile, "|x\t\t\t|\tF%d(x)\t\t|\n", func_choice);
            fprintf(outputFile, "_____________________________\n");

            for (int i = 0; i < n; i++) {
                switch (func_choice) {
                case 1:
                    fprintf(outputFile, "|%.5lf\t|\t%lf\t|\n", values[i], F1(values[i]));  // ����� �������� ��� F1
                    break;
                case 2:
                    fprintf(outputFile, "|%.5lf\t|\t%lf\t|\n", values[i], F2(values[i]));  // ����� �������� ��� F2
                    break;
                case 3:
                    fprintf(outputFile, "|%.5lf\t|\t%lf\t|\n", values[i], F3(values[i]));  // ����� �������� ��� F3
                    break;
                default:
                    fprintf(outputFile, "������������ ����� �������.\n");
                    break;
                }
            }
            additional_menu(outputFile, values, n, xmin, xmin + n * dx);  // ������� � �������������� ����
        }
        break;
        case 3:  // ����� �
        {
            int func_choice, N;
            double xmin, xmax;
            double* array;

            printf("������� ��������� �������� (xmin): ");
            scanf("%lf", &xmin);

            printf("������� �������� �������� (xmax): ");
            scanf("%lf", &xmax);

            printf("������� ���������� �������� N: ");
            scanf("%d", &N);

            array = (double*)malloc(N * sizeof(double));  // ��������� ������ ��� ������� ��������

            B(array, N, xmin, xmax);  // ��������� ���������� ��������� ��������

            fprintf(outputFile, "��������������� ���������� ��������:\n");
            for (int i = 0; i < N; i++) {
                fprintf(outputFile, "%lf ", array[i]);  // ����� ��������������� ��������
            }
            fprintf(outputFile, "\n");

            printf("�������� ������� ��� ���������� ����������:\n1 - F1\n2 - F2\n3 - F3\n");
            scanf("%d", &func_choice);

            fprintf(outputFile, "�������� ��� ��������� �������:\n");
            fprintf(outputFile, "|x\t\t|\tF%d(x)\t\t|\n", func_choice);
            fprintf(outputFile, "_________________________\n");

            for (int i = 0; i < N; i++) {
                switch (func_choice) {
                case 1:
                    fprintf(outputFile, "|%.2lf\t|\t%lf\t|\n", array[i], F1(array[i]));  // ����� �������� ��� F1
                    break;
                case 2:
                    fprintf(outputFile, "|%.2lf\t|\t%lf\t|\n", array[i], F2(array[i]));  // ����� �������� ��� F2
                    break;
                case 3:
                    fprintf(outputFile, "|%.2lf\t|\t%lf\t|\n", array[i], F3(array[i]));  // ����� �������� ��� F3
                    break;
                default:
                    fprintf(outputFile, "������������ ����� �������.\n");
                    break;
                }
            }
            free(array);  // ������������ ������

            additional_menu(outputFile, array, N, xmin, xmax);  // ������� � �������������� ����
        }
        break;
        case 0:
            printf("��������� ���������.\n");
            return;  // ���������� ������ ���������
        default:
            printf("������������ ����. ���������� �����.\n");
            break;
        }
    }
}

void additional_menu(FILE* outputFile, double* values, int N, double xmin, double xmax) {
    int choice;

    while (1) {
        printf("\n�������������� ��������:\n");
        printf("1 - ���������� ����������� � ������������ �������� ������� F1, F2 ��� F3\n");
        printf("2 - �������� ����� �������� ��������� ��� �������� �������� �������\n");
        printf("3 - ����������������� ������� � �������� �����\n");
        printf("0 - ��������� � ������� ����\n");
        scanf("%d", &choice);

        switch (choice) {
        case 1:  // ���������� min/max
        {
            int func_choice;
            printf("�������� ������� ��� ������ ������������ � ������������� ��������:\n");
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
                    printf("������������ ����� �������.\n");
                    free(func_values);
                    return;
                }
            }

            printf("����������� �������� �������: %.5lf\n", find_min(NULL, func_values, N));  // ����� min
            printf("������������ �������� �������: %.5lf\n", find_max(NULL, func_values, N));  // ����� max
            free(func_values);
        }
        break;
        case 2:  // �������� �����
        {
            double target;
            printf("������� ������� �������� �������: ");
            scanf("%lf", &target);
            double x = linear_search(F1, target, xmin, xmax, 0.01);  // ����� ��� F1
            printf("��������� �������� ���������: %.5lf\n", x);
        }
        break;
        case 3:  // �����������������
        {
            double x;
            printf("������� ����� ��� �����������������: ");
            scanf("%lf", &x);
            double epsilon = 1e-5;
            double derivative = differentiate(F1, x, epsilon);  // ����������������� ��� F1
            printf("������������ ������� � ����� %.5lf: %.5lf\n", x, derivative);
        }
        break;
        case 0:
            return;  // ������� � ������� ����
        default:
            printf("������������ �����. ���������� �����.\n");
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
    return min;  // ���������� ������������ ��������
}

double find_max(double (*func)(double), double* values, int N) {
    double max = values[0];
    for (int i = 1; i < N; i++) {
        if (values[i] > max) {
            max = values[i];
        }
    }
    return max;  // ���������� ������������� ��������
}

double linear_search(double (*func)(double), double target, double xmin, double xmax, double dx) {
    for (double x = xmin; x <= xmax; x += dx) {
        if (fabs(func(x) - target) < 1e-2) {  // ���������� �����������
            return x;
        }
    }
    return NAN;  // ���� �� �������
}

double differentiate(double (*func)(double), double x, double epsilon) {
    return (func(x + epsilon) - func(x)) / epsilon;  // ������������ �����������������
}

double F1(double x) {
    return exp(x * cos((PI * x) / 4));  // ������ �������
}

double F2(double x) {
    double sum = 0;
    if (x > -2) {
        for (int i = 1; i <= 5; i++) {
            sum += pow(x, -i) / (i + 2);  // ���������� ��� ������� 2
        }
        return sum;
    }
    else {
        return pow(3 + x, 2 - x);  // ������ ��������� ��� ������� 2
    }
}

double F3(double x) {
    double th;
    th = (exp(2 * x) - 1) / (exp(2 * x) + 1);  // ������ �������

    return th;
}

void A(double xmin, double dx, double* values) {
    for (int i = 0; i < n; i++) {
        values[i] = xmin + i * dx;  // ��������� ��������
    }
}

void B(double* array, int N, double xmin, double xmax) {
    int count = 0;
    while (count < N) {
        double num = xmin + (double)rand() / RAND_MAX * (xmax - xmin);  // ��������� ��������� �����
        int unique = 1;
        for (int i = 0; i < count; i++) {
            if (fabs(array[i] - num) < 1e-6) {  // �������� �� ������������
                unique = 0;
                break;
            }
        }
        if (unique) {
            array[count++] = num;  // ���������� ����������� ��������
        }
    }
}

void result(FILE* outputFile, double x, int func_num, double (*func)(double)) {
    fprintf(outputFile, "|%.3lf\t|\t%lf\t|\n", x, func(x));  // ����� ���������� � ����
}
