#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_RESULT_STRING_LENGTH 35
#define ARRAY_SIZE 6
#define error(...) (fprintf(stderr, __VA_ARGS__))

void free_results_of_strings(char **results, int count) {
    for (int i = 0; i < count; i++) {
        free(results[i]);
    }
    free(results);
}

double simpson(int n, double left_border, double right_border) {
    double h, g, integral;
    h = (right_border - left_border) / (2 * n);
    g = left_border + h;
    integral = sin(left_border) + sin(right_border);
    for (int i = 1; i < 2 * n; i++) {
        if (i % 2 != 0) {
            integral = integral + 4 * sin(g);
        } else {
            integral = integral + 2 * sin(g);
        }
        g += h;
    }
    integral = (h / 3) * integral;
    return integral;
}

double rectangle(int n, double left_border, double right_border) {
    double h, g, integral = 0;
    h = (right_border - left_border) / n;
    g = left_border;
    for (int i = 0; i < n; i++) {
        integral = integral + sin(g + h / 2);
        g += h;
    }
    integral = h * integral;
    return integral;
}

char **calculate_integrals(int array[], double left_border, double right_border, int array_size, int max_result_string_length) {
    char **results = malloc(array_size * sizeof(char *));
    if (!results) {
        error("Cannot allocate memory for results string in all experiment\n");
        return NULL;
    }
    for (int i = 0; i < array_size; i++) {
        double s, r;
        results[i] = malloc(sizeof(char) * max_result_string_length);
        if (!results[i]) {
            free_results_of_strings(results, i);
            error("Cannot allocate memory for result string in %d experiment\n", i);
            return NULL;
        }
        r = rectangle(array[i], left_border, right_border);
        s = simpson(array[i], left_border, right_border);
        if (!sprintf(results[i], "%d %.5f %.5f ", array[i], r, s)){
            free_results_of_strings(results, i+1);
            error("Cannot write results to string in %d experiment\n", i);
            return NULL;
        }
    }
    return results;
}

int read_interval(double *left_border, double *right_border) {
            if (printf("Enter interval's left border: ") < 0) {
                error("Cannot write to stdout to stdout\n");
                return 1;
            }
            if (scanf("%lf", left_border) != 1) {
                error("Cannot read interval's left border\n");
                return 1;
            }
            if (*left_border < 0) {
                error("Left border of the interval must be greater than or equal to 0\n");
                return 1;
            }
            if (*left_border > M_PI) {
                error("Left border of the interval must be less than pi\n");
                return 1;
            }
            if (printf("Enter interval's right border: ") < 0) {
                error("Cannot write to stdout\n");
                return 1;
            }
            if (scanf("%lf", right_border) != 1) {
                error("Cannot read interval's right border\n");
                return 1;
            }
            if (*right_border < 0) {
                error("Right border of the interval must be greater than 0\n");
                return 1;
            }
            if (*right_border > M_PI) {
                error("Right border of the interval must be less than or equal to pi\n");
                return 1;
            }
            if (*left_border > *right_border) {
                error("Right border of the interval must be greater than left\n");
                return 1;
            }
    return 0;
}

int main() {
    double left_border, right_border;
    int array[ARRAY_SIZE] = {5, 10, 20, 100, 500, 1000};
    if (read_interval(&left_border, &right_border)) {
        return 1;
    }
    char **results = calculate_integrals(array, left_border, right_border, ARRAY_SIZE, MAX_RESULT_STRING_LENGTH);
    if (!results) {
        return 1;
    }
    for (int i = 0; i < ARRAY_SIZE; i++) {
        if (printf("%s \n", results[i])<0){
            error("Cannot write %d result to stdout", i);
            break;
        }
    }
    free_results_of_strings(results, ARRAY_SIZE);
    return 0;
}
