#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_RESULT_STRING_LENGTH 35
#define PARTITION_ARRAY_SIZE 6
#define error(...) (fprintf(stderr, __VA_ARGS__))

void free_results_of_strings(char **results, int count) {
    for (int i = 0; i < count; i++) {
        free(results[i]);
    }
    free(results);
}

double calculate_simpson(int partition, double left_border, double right_border) {
    double step, calculation_steps, integral;
    step = (right_border - left_border) / (2 * partition);
    calculation_steps = left_border + step;
    integral = sin(left_border) + sin(right_border);
    for (int i = 1; i < 2 * partition; i++) {
        if (i % 2 != 0) {
            integral = integral + 4 * sin(calculation_steps);
        } else {
            integral = integral + 2 * sin(calculation_steps);
        }
        calculation_steps += step;
    }
    integral = (step / 3) * integral;
    return integral;
}

double calculate_rectangle(int partition, double left_border, double right_border) {
    double step, calculation_steps, integral = 0;
    step = (right_border - left_border) / partition;
    calculation_steps = left_border;
    for (int i = 0; i < partition; i++) {
        integral = integral + sin(calculation_steps + step / 2);
        calculation_steps += step;
    }
    integral = step * integral;
    return integral;
}

char **calculate_integrals(int array[], double left_border, double right_border, 
							int partition_array_size, int max_result_string_length) {
    char **results = malloc(partition_array_size * sizeof(char *));
    if (!results) {
        error("Cannot allocate memory for results string in all experiment\n");
        return NULL;
    }
    for (int i = 0; i < partition_array_size; i++) {
        double s, r;
        results[i] = malloc(sizeof(char) * max_result_string_length);
        if (!results[i]) {
            free_results_of_strings(results, i);
            error("Cannot allocate memory for result string in %d experiment\n", i);
            return NULL;
        }
        r = calculate_rectangle(array[i], left_border, right_border);
        s = calculate_simpson(array[i], left_border, right_border);
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
    int array[PARTITION_ARRAY_SIZE] = {6, 10, 20, 100, 500, 1000};
    if (read_interval(&left_border, &right_border)) {
        return 1;
    }
    char **results = calculate_integrals(array, left_border, right_border, PARTITION_ARRAY_SIZE, MAX_RESULT_STRING_LENGTH);
    if (!results) {
        return 1;
    }
    for (int i = 0; i < PARTITION_ARRAY_SIZE; i++) {
        if (printf("%s \n", results[i])<0){
            error("Cannot write %d result to stdout", i);
            break;
        }
    }
    free_results_of_strings(results, PARTITION_ARRAY_SIZE);
    return 0;
}
