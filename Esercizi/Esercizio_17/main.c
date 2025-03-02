/*Implementare un programma parallelo per l’ambiente multicore con np unità processanti che impieghi
 *la libreria OpenMP. Il programma deve essere organizzato come segue:
 *il core master deve leggere quattro valori scalari a0, a1, a2, a3,
 *quindi i core devono collaborare per costruire un vettore di
 *dimensione 4np i cui elementi sono dati dalla somma di tali valori
 *con il proprio identificativo.
*/

#include <stdio.h>
#include <omp.h>

void printArray(int * array, int size);
void fillArray(int * array, int size, double max_element);
int main(void) {
    int np;
    int a0,a1,a2,a3;
    int *a;
    int i;
    int sum;

    printf("Con quanti core vuoi effettuare il calcolo?= ");
    scanf("%d",&np);

    int N = 4*np;

    printf("Inserisci il valore di a0= ");
    scanf("%d",&a0);

    printf("Inserisci il valore di a1= ");
    scanf("%d",&a1);

    printf("Inserisci il valore di a2= ");
    scanf("%d",&a2);

    printf("Inserisci il valore di a3= ");
    scanf("%d",&a3);



    a=calloc(N,sizeof(int));



#pragma omp parallel shared(a,N,sum) private(i) num_threads(np)
    {

#pragma omp master
    sum = a0+a1+a2+a3;
        printf("\nSomma: %d\n",sum);

#pragma omp barrier

#pragma omp for
    for(i=0;i<N;i++) {
        int thread_id = omp_get_thread_num();
        a[i] = sum+thread_id;
        // Stampa informazioni su thread, posizione e valore calcolato
        printf("Thread ID: %d ha calcolato a[%d] = %d\n", thread_id, i, a[i]);
    }
}


    // Stampa del vettore risultante
    printf("Vettore risultante: \n");
    printArray(a, N);

    // Rilascio della memoria
    free(a);

    return 0;
}

void printArray(int * array, int size) {
    for (size_t i = 0; i < size; ++i) {
        printf("%d ", array[i]);
    }
}

void fillArray(int * array, int size, double max_element) {
#pragma omp parallel for
    for (size_t i = 0; i < size; ++i) {
        array[i] = (double) rand() / (double) RAND_MAX * max_element;
    }
}