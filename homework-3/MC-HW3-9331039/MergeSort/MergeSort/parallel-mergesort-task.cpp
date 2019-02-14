#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>

#define MAX(A, B) ((A)>(B))?(A):(B)
#define MIN(A, B) ((A)<(B))?(A):(B)

void printArray(int *array, int size);
void fillArray(int *array, int size);
void merge(int *a, int n, int m);
void mergeSort(int *a, int n);
void parallel_mergesort_task(int *a, int n);
void parallel_mergesort_section(int *a, int n);
void check_omp();
int counter =0;
int main(int argc, char *argv[]) {
	double startTime, endTime;
	int *array = NULL;
	int size = 0;
	if (argc < 2) {
		printf("[-] Invalid No. of arguments.\n");
		printf("[-] Try -> <size_of_array>\n");
		printf(">>> ");
		scanf("%d", &size);
	}
	else {
		size = atoi(argv[1]);
	}
	array = (int *)malloc(sizeof(int) * size);

	fillArray(array, size);
	printf("Merge Sort :\n");
	startTime = omp_get_wtime();
	parallel_mergesort_task(array, size);
	endTime = omp_get_wtime() - startTime;
	//printArray(array, size);
	printf("Time Elapsed %f Secs \n", endTime);
	free(array);
	system("PAUSE");
	return EXIT_SUCCESS;
}

void fillArray(int *array, int size) {
	srand(time(NULL));
	while (size-->0) {
		*array++ = rand() % 100;
	}
}

void printArray(int *array, int size) {
	while (size-->0) {
		printf("%d, ", *array++);
	}
	printf("\n");
}

void merge(int *a, int n, int m) {
	int i, j, k;
	int *temp = (int *)malloc(n * sizeof(int));
	for (i = 0, j = m, k = 0; k < n; k++) {
		temp[k] = j == n ? a[i++]
			: i == m ? a[j++]
			: a[j] < a[i] ? a[j++]
			: a[i++];
	}
	for (i = 0; i < n; i++) {
		a[i] = temp[i];
	}
	free(temp);
}


void parallel_mergesort_task(int *a, int n) {
	counter++;
	check_omp();
	printf("Merge Sort:\n");
	int m;
	if (n < 2)
		return;
	m = n / 2;
	#pragma omp parallel num_threads(4)
	{
		#pragma omp single nowait
		{
			#pragma omp task if(counter<1000)
			parallel_mergesort_task(a, m);
			#pragma omp task  if(counter<1000)
			parallel_mergesort_task(a + m, n - m);
		}
		#pragma omp taskwait
		merge(a, n, m);
	}
}

void check_omp() {
	// written by Ahmad Siavashi (siavashi@aut.ac.ir) April 26th, 2018
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
