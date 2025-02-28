/*
 *
 * Implementazione di un algoritmo parallelo (np core) per il calcolo
 * dell’operazione c = a+b con a,b vettori di dimensione N, in ambiente OpenMP
 */
#include <stdio.h>
#include <omp.h>
#include <time.h>
void fillArray(int * array, int size, double max_element);
void printArray(int * array, int size);
int main(void)
{
    int N, i;
    int np;

    int *vettore_a;
    int *vettore_b;
    int *vettore_c;
    srand(time(NULL));

    printf("Con quanti core vuoi eseguire il calcolo? ");
    scanf("%d",&np);

    printf("Inserire la dimensione dei vettori (N)= ");
    scanf("%d",&N);

    //alloco memoria per i vettori
    vettore_a = calloc(N,sizeof(int));
    vettore_b = calloc(N,sizeof(int));
    vettore_c = calloc(N,sizeof(int));
    if (vettore_a == NULL || vettore_b == NULL || vettore_c == NULL) {
        printf("Errore nell'allocazione della memoria.\n");
        exit(1);
    }

    //riempio i vettori con elementi casuali
    fillArray(vettore_a,N,20);
    fillArray(vettore_b,N,30);

    //stampo i vettori
    printf("Vettore a: ");
    printArray(vettore_a,N);
    printf("\nVettore b: ");
    printArray(vettore_b,N);

   double start_time = omp_get_wtime();
#pragma omp parallel for default(none) shared(vettore_a,vettore_b,vettore_c,N) private(i) num_threads(np)

    {
        for(i=0;i<N;i++) {
            vettore_c[i] = vettore_a[i]+vettore_b[i];
            printf("\nThread %d sta elaborando l'elemento %d\n", omp_get_thread_num(), i);
        }
    }

    double end_time = omp_get_wtime();

    printf("\n\nVettore c: \n");
    for(i=0;i<N;i++) {
                printf("Elemento [%d]=%d\n",i, vettore_c[i]);
            }


    printf("Tempo di esecuzione: %f secondi\n", end_time - start_time);

    // Libera la memoria nella sezione seriale dopo che tutti i thread hanno terminato
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