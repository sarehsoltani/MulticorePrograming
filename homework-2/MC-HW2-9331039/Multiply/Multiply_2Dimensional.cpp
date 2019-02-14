#include <iostream>

// Let it be.
#define _CRT_SECURE_NO_WARNINGS
#define NUMBER_OF_REPEAT 5

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <time.h>
#include <omp.h>


using namespace std;

typedef struct {
    int *A, *B, *C, *C_prime;
    int n, m, p;
} DataSet;

void fillDataSet(DataSet *dataSet);
void printDataSet(DataSet dataSet);
void closeDataSet(DataSet dataSet);
void multiply(DataSet dataSet);
void multiply_par_2Dimensional(DataSet dataSet);
void check_omp() {
    #ifdef _DEBUG
        printf("[!] Configuration: Debug.\n");
        printf("[#] Change to Release.\n");
    #pragma message ("Change configuration to Release for a fast execution.")
    #else
        printf("[-] Configuration: Release.\n");
    #endif // _DEBUG
    #ifdef _M_X64
        printf("[-] Platform: x64\n");
    #elif _M_IX86
        printf("[-] Platform: x86\n");
    #pragma message ("Change platform to x64 for more memory.")
    #endif // _M_IX86
    #ifdef _OPENMP
        printf("[!] OpenMP is on.\n");
        printf("[-] OpenMP version: %d\n", _OPENMP);
    #else
        printf("[!] OpenMP is off.\n");
        printf("[#] Enable OpenMP.\n");
    #endif // _OPENMP
        printf("[-] Maximum threads: %d\n", omp_get_max_threads());
        printf("[-] Nested Parallelism: %s\n", omp_get_nested() ? "On" : "Off");
    #pragma message("Enable nested parallelism if you wish to have parallel region within parallel region.")
        printf("[#] Enable nested parallelism.\n");
}

int main(int argc, char *argv[]) {
    check_omp();
    double startTime,endTime,sumTime=0;
    DataSet dataSet;
    if (argc < 4) {
        printf("[-] Invalid No. of arguments.\n");
        printf("[-] Try -> <n> <m> <p>\n");
        printf("»> ");
        scanf("%d %d %d", &dataSet.n, &dataSet.m, &dataSet.p);
    }
    else {
        dataSet.n = atoi(argv[1]);
        dataSet.m = atoi(argv[2]);
        dataSet.p = atoi(argv[3]);
    }
    for (int i = 0; i<NUMBER_OF_REPEAT; i++) {
        fillDataSet(&dataSet);
        startTime = omp_get_wtime();
//        multiply(dataSet);
        multiply_par_2Dimensional(dataSet);
        endTime = omp_get_wtime() - startTime;
        printf("%3d loop --> Time Elapsed %f Secs \n", i + 1, endTime);
            sumTime += endTime;
        //printDataSet(dataSet);
    //int cmp = memcmp(dataSet.C, dataSet.C_prime, dataSet.n * dataSet.p);
    //  if cmp == 0 then everything is fine.
     //printf("[-] Comparing results... %d\n", cmp);
            closeDataSet(dataSet);
    }
     printf("average Time Passed %f \n", sumTime / NUMBER_OF_REPEAT);
//    system("PAUSE");
    return EXIT_SUCCESS;
}

void fillDataSet(DataSet *dataSet) {
    int i, j;

    dataSet->A = (int *)malloc(sizeof(int) * dataSet->n * dataSet->m);
    dataSet->B = (int *)malloc(sizeof(int) * dataSet->m * dataSet->p);
    dataSet->C = (int *)malloc(sizeof(int) * dataSet->n * dataSet->p);
    dataSet->C_prime = (int *)malloc(sizeof(int) * dataSet->n * dataSet->p);

    srand(time(NULL));

    for (i = 0; i < dataSet->n; i++) {
        for (j = 0; j < dataSet->m; j++) {
            dataSet->A[i*dataSet->m + j] = rand() % 100;
        }
    }

    for (i = 0; i < dataSet->m; i++) {
        for (j = 0; j < dataSet->p; j++) {
            dataSet->B[i*dataSet->p + j] = rand() % 100;
        }
    }
}

void printDataSet(DataSet dataSet) {
    int i, j;

    printf("[-] Matrix A\n");
    for (i = 0; i < dataSet.n; i++) {
        for (j = 0; j < dataSet.m; j++) {
            printf("%-4d", dataSet.A[i*dataSet.m + j]);
        }
        putchar('\n');
    }

    printf("[-] Matrix B\n");
    for (i = 0; i < dataSet.m; i++) {
        for (j = 0; j < dataSet.p; j++) {
            printf("%-4d", dataSet.B[i*dataSet.p + j]);
        }
        putchar('\n');
    }

    printf("[-] Matrix C\n");
    for (i = 0; i < dataSet.n; i++) {
        for (j = 0; j < dataSet.p; j++) {
            printf("%-8d", dataSet.C[i*dataSet.p + j]);
        }
        putchar('\n');
    }
}

void closeDataSet(DataSet dataSet) {
    free(dataSet.A);
    free(dataSet.B);
    free(dataSet.C);
    free(dataSet.C_prime);
}

// Serial
void multiply(DataSet dataSet) {
    int i, j, k, sum;
    for (i = 0; i < dataSet.n; i++) {
        for (j = 0; j < dataSet.p; j++) {
            sum = 0;
            for (k = 0; k < dataSet.m; k++) {
                sum += dataSet.A[i * dataSet.m + k] * dataSet.B[k * dataSet.p + j];
            }
            dataSet.C_prime[i * dataSet.p + j] = sum;
        }
    }
}

// Parallel
void multiply_par_2Dimensional(DataSet dataSet) {
    omp_set_num_threads(8);
#pragma omp parallel
    {
        int  sum;
#pragma omp for
        for (int i = 0; i < dataSet.n; i++) {
#pragma omp parallel for
            for (int j = 0; j < dataSet.p; j++) {
                sum = 0;
                for (int k = 0; k < dataSet.m; k++) {
                    sum += dataSet.A[i * dataSet.m + k] * dataSet.B[k * dataSet.p + j];
                }
                dataSet.C[i * dataSet.p + j] = sum;
            }
        }
    };
}