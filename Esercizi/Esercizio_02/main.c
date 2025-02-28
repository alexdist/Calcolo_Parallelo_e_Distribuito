/*
 *Implementazione di un algoritmo parallelo (np core) per il calcolo
 *dell’operazione c = alpha * a+b, con a,b vettori di dimensione N e
 *alpha uno scalare in ambiente OpenMP
 **/

#include <stdio.h>
#include <omp.h>
#include <time.h>
void fillArray(int * array, int size, double max_element);
void printArray(int * array, int size);
int main(void) {

    int n_core;
    int N,i;

    int scalare;

    int *vettore_a;
    int *vettore_b;
    int *vettore_c;

    srand(time(NULL));

    printf("Quanti core vuoi utilizzare per il calcolo?= ");
    scanf("%d",&n_core);

    printf("Inserire il valore dello scalare (alpha)= ");
    scanf("%d",&scalare);

    printf("Inserire la dimensione dei vettori (N)= ");
    scanf("%d", &N);

    vettore_a = calloc(N,sizeof(int));
    vettore_b = calloc(N,sizeof(int));
    vettore_c = calloc(N,sizeof(int));

    if(vettore_a == NULL || vettore_b == NULL || vettore_c == NULL) {
        printf("Errore nell'allocazione della memoria");
        return 1;
    }

    fillArray(vettore_a,N,10);
    fillArray(vettore_b,N,40);

    printf("\nVettore a: ");
    printArray(vettore_a,N);
    printf("\nVettore b: ");
    printArray(vettore_b,N);

  double start_time = omp_get_wtime();
#pragma omp parallel for shared(vettore_a, vettore_b, vettore_c, N) private(i) num_threads(n_core)
{

        for(i=0;i<N;i++) {
            vettore_c[i] = scalare * vettore_a[i] + vettore_b[i];
            printf("\nThread %d sta elaborando l'elemento %d\n", omp_get_thread_num(), i);

        }

    }
    double end_time = omp_get_wtime();

    printf("\nVettore c: ");
    printArray(vettore_c,N);

    printf("\n\nTempo di esecuzione regione parallela: %lf", end_time-start_time);


    free(vettore_a);
    free(vettore_b);
    free(vettore_c);


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
