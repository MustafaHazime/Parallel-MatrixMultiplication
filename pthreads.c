#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define M 4 // number of threads
#define MAX 1000 // maximum value for matrix elements

int N, K;

// matrices A, B, and C
double **A, **B, **C;

// range struct for passing to each thread
struct range {
    int start;
    int end;
};

// function for each thread to compute its assigned rows
void *multiply(void *arg) {
    struct range *r = (struct range *) arg;
    int i, j, k;
    for (i = r->start; i <= r->end; i++) {
        for (j = 0; j < N; j++) {
            for (k = 0; k < K; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    pthread_exit(NULL);
}

int main() {
    int i, j;
    
    // input the size of matrices
    printf("Enter the size of matrix A (M x K): ");
    scanf("%d %d", &N, &K);

    printf("Enter the size of matrix B (K x N): ");
    scanf("%*d %d", &N);

    // allocate memory for matrices A, B, and C
    A = (double **) malloc(N * sizeof(double *));
    B = (double **) malloc(K * sizeof(double *));
    C = (double **) malloc(N * sizeof(double *));
    for (i = 0; i < N; i++) {
        A[i] = (double *) malloc(K * sizeof(double));
        C[i] = (double *) calloc(N, sizeof(double));
    }
    for (i = 0; i < K; i++) {
        B[i] = (double *) malloc(N * sizeof(double));
    }

    // input matrix A
    printf("Enter matrix A:\n");
    for (i = 0; i < N; i++) {
        for (j = 0; j < K; j++) {
            scanf("%lf", &A[i][j]);
        }
    }

    // input matrix B
    printf("Enter matrix B:\n");
    for (i = 0; i < K; i++) {
        for (j = 0; j < N; j++) {
            scanf("%lf", &B[i][j]);
        }
    }

    // create M threads and assign them to compute their assigned rows
    pthread_t threads[M];
    struct range ranges[M];
    int rows_per_thread = N / M;
    for (i = 0; i < M; i++) {
        ranges[i].start = i * rows_per_thread;
        ranges[i].end = (i == M-1) ? N-1 : (i+1) * rows_per_thread - 1;
        pthread_create(&threads[i], NULL, multiply, &ranges[i]);
    }

    // wait for all threads to finish
    for (i = 0; i < M; i++) {
        pthread_join(threads[i], NULL);
    }

    // print matrices A, B, and C
    printf("\nMatrix A:\n");
    for (i = 0; i < N; i++) {
        for (j = 0; j < K; j++) {
            printf("%g ", A[i][j]);
        }
        printf("\n");
    }

       printf("Matrix B:\n");
    for (i = 0; i < K; i++) {
        for (j = 0; j < N; j++)
            printf("%d ", B[i][j]);
        printf("\n");
    }

    printf("Result matrix C = A x B:\n");
    for (i = 0; i < M; i++) {
        for (j = 0; j < N; j++)
            printf("%d ", C[i][j]);
        printf("\n");
    }

    return 0;
}
