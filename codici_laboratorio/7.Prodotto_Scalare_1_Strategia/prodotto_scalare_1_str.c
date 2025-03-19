#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

void printArray(int * array, int n);

int main() {
    int N,np;
    printf("Con quanti core vuoi eseguire il calcolo?: ");
    scanf("%d",&np);
    printf("\nInserire N: ");
    scanf("%d", &N);

    int * a = calloc(N,sizeof(int));
    int * b = calloc(N,sizeof(int));

    printf("\nInserire gli elementi nel vettore A\n");
    for (int i = 0; i < N; i++) {
        printf("Elemento %d-esimo: ",i);
        scanf("%d",&a[i]);
    }

    printf("\nInserire gli elementi nel vettore B\n");
    for (int i = 0; i < N; i++) {
        printf("Elemento %d-esimo: ",i);
        scanf("%d",&b[i]);
    }

    printf("Vettore A:\n");
    printArray(a, N);
    printf("Vettore B:\n");
    printArray(b, N);

    double start_time, end_time;
    int * temp = calloc(N,sizeof(int));
    int sum = 0;
    start_time = omp_get_wtime();
#pragma omp parallel for shared(sum) schedule(dynamic) num_threads(np)
    for (size_t i = 0; i < N; i++)
    {
        temp[i] = a[i] * b[i];
#pragma omp critical
        sum += temp[i];
    }
    end_time= omp_get_wtime();
    printf("Risultato: %d\n", sum);
    printf("Tempo: %lf\n", end_time - start_time);
    return 0;
}

void printArray(int * array, int n) {
    for (int i = 0; i < n; i++) {
        printf("[%d] = %d\n", i, array[i]);
    }
}