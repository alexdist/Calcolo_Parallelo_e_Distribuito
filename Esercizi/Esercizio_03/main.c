/*
 *Implementazione di un algoritmo parallelo (np core) per il calcolo
 *degli elementi di un vettore c, i cui valori sono ottenuti moltiplicando
 *l’identificativo del core per gli elementi di un vettore a di dimensione N, in ambiente OpenMP
 *
 */

#include <stdio.h>
#include <omp.h>
#include <time.h>
void fillArray(int * array, int size, double max_element);
void printArray(int * array, int size);
int main(void) {

    int n_core;
    int N, i;

    int *vettore_a;
    int *vettore_c;

    srand(time(NULL));

    printf("Quanti core vuoi utilizzare per il calcolo?= ");
    scanf("%d", &n_core);

    printf("Inserisci la dimensione dei vettori (N): ");
    scanf("%d", &N);

    vettore_a = calloc(N,sizeof(int));
    vettore_c = calloc(N, sizeof(int));

    if(vettore_a == NULL || vettore_c == NULL) {
        printf("Errore nell'allocazione della memoria");
        return 1;
    }

    fillArray(vettore_a, N, 20);

    printf("\nVettore a: ");
    printArray(vettore_a,N);

 double start_time = omp_get_wtime();
#pragma omp parallel for default(none) shared(vettore_a, vettore_c, N) private(i) num_threads(n_core)
    {
        for(i=0;i<N;i++) {

            vettore_c[i] = omp_get_thread_num() * vettore_a[i];
            printf("\nL'elemento [%d] = %d e' stato calcolato dal thread %d\n", i, vettore_c[i], omp_get_thread_num());

        }
    }
    double end_time = omp_get_wtime();


    printf("\nVettore c: ");
    printArray(vettore_c,N);

    printf("\nTempo: %lf", end_time-start_time);

    return 0;
}
void fillArray(int * array, int size, double max_element) {
#pragma omp parallel for
    for (size_t i = 0; i < size; ++i) {
        array[i] = (double) rand() / (double) RAND_MAX * max_element;
    }
}

void printArray(int * array, int size) {
    for (size_t i = 0; i < size; ++i) {
        printf("%d ", array[i]);
    }
}

