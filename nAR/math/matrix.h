#ifndef NYANCAT_MATH_MATRIX_H
#define NYANCAT_MATH_MATRIX_H

#ifndef STDLIB_H
#define STDLIB_H
#include <stdlib.h>
#endif

#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif

#ifndef STDBOOL_H
#define STDBOOL_H
#include <stdbool.h>
#endif

#ifndef STDARG_H
#define STDARG_H
#include <stdarg.h>
#endif

#include "complex_num.h"

// Matrix structure
typedef struct _matrix {
    complexnum *vals;
    int rows, columns;
} matrix;

matrix *new_matrix(int rows, int columns);
complexnum get_matrix_val(matrix *m, int row, int column);
int set_matrix_val(matrix *m, int row, int column, complexnum val);
void print_matrix(matrix *m);
bool populate_matrix(matrix *m, ...);
int free_matrix(matrix *m);
matrix *multiply_matrix(matrix *m, matrix *n);

// Allocates a new matrix
matrix *new_matrix(int rows, int columns) {
    matrix *m = (matrix*)malloc(sizeof(matrix));
    m->rows = rows;
    m->columns = columns;
    m->vals = (complexnum*)calloc(sizeof(complexnum), rows * columns);
    return m;
}

// Gets value from matrix
complexnum get_matrix_val(matrix *m, int row, int column) {
    if(!m || row >= m->rows || row < 0 
        || column >= m->columns || column < 0) return (complexnum){0, 0};
    return m->vals[row * m->columns + column];
}

// Sets value on matrix
int set_matrix_val(matrix *m, int row, int column, complexnum val) {
    if(!m || row >= m->rows || row < 0 
        || column >= m->columns || column < 0) return 0;
    m->vals[row * m->columns + column] = val;
    return 1;
}

// Prints matrix
void print_matrix(matrix *m) {
    int i, j;
    for(i = 0; i < m->rows; i++) {
        printf("| ");
        for(j = 0; j < m->columns; j++) {
            printf("%s", show_complex(get_matrix_val(m, i, j)));
        }
        printf("|\n");
    }
}

// Fills a matrix with provided values
bool populate_matrix(matrix *m, ...) {
    if(!m) return false;
    va_list ap;
    va_start(ap, m);

    int i, j; 
    complexnum k;
    for (i = 0; i < m->rows; i++) {
        for (j = 0; j < m->columns; j++) {
            k = va_arg(ap, complexnum);
            set_matrix_val(m, i, j, k);
        }
    }
    va_end(ap);
    return true;
}

// Frees matrix
int free_matrix(matrix *m) {
    if(!m) return 0;
    free(m->vals);
    free(m);
    m = NULL;
    return 1;
}

// Multiplies two matrices and return a third one
matrix *multiply_matrix(matrix *m, matrix *n) {
    if(m->columns != n->rows) return NULL;
    matrix *r = new_matrix(m->rows, n->columns);
    complexnum val;
    int i, j, k;
    for(i = 0; i < m->rows; i++) {
        for(j = 0; j < n->columns; j++) {
            complexnum val = { 0, 0 };
            for(k = 0; k < m->columns; k++) {
                val = add_complex(val,
                    multiply_complex(
                        get_matrix_val(m, i, k),
                        get_matrix_val(n, k, j)
                    )
                );
            }
            set_matrix_val(r, i, j, val);
        }
    }
    return r;
}

#endif