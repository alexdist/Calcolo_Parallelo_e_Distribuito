/*
 *Implementare un programma parallelo per l’ambiente multicore con np unità processanti che impieghi
 *la libreria OpenMP. Il programma deve essere organizzato come segue: il core master deve generare
 *una matrice di dimensione NxM. Quindi, ogni core deve estrarre la riga della matrice con
 *indice uguale al proprio identificativo e conservarla in un vettore locale; successivamente
 *deve sommare gli elementi di tale vettore e conservare il risultato in un vettore c di dimensione np.
 *Infine, i core devono collaborare per individuare il massimo tra gli elementi del vettore c.
*/

#include <stdio.h>
#include <omp.h>
#include <limits.h>
void fill_matrix(int ** matrix, int row, int col, int max_element);
void print_matrix(int ** matrix, int row, int col);
void printArray(int * array, int size);
int main(void) {

    int np;
    int **matrice, *c;
    int M,i;
    int max_element;// = INT_MIN;
    int local_sum = 0;
    int tid;

    printf("Inserisci il numero di righe della matrice (N)=numero di threads = ");
    scanf("%d",&np);

    printf("Inserisci il numero di colonne della matrice (M)= ");
    scanf("%d",&M);

    matrice = malloc(np*sizeof(int*));
    if(matrice == NULL) {
        printf("Errore nell'allocazione della memoria");
        return 1;
    }
    for(i=0;i<np;i++) {
        matrice[i]= malloc(M*sizeof(int));
    }

    c = calloc(np,sizeof(int));

    fill_matrix(matrice,np,M,30);
    print_matrix(matrice,np,M);

   double start_time = omp_get_wtime();
#pragma omp parallel shared(matrice, M, c) private(i,tid,local_sum) num_threads(np)
    {

        tid=omp_get_thread_num();
        local_sum = 0;

        // Stampa il thread attuale che sta eseguendo la somma
        printf("Thread %d sta sommando la riga %d\n", tid, tid);
    for(i=0;i<M;i++) {
        local_sum += matrice[tid][i];
    }
        c[tid]=local_sum;

//#pragma omp barrier

        // Calcolo del massimo nel vettore c con OpenMP
        #pragma omp parallel for reduction(max:max_element)
            for (i = 0; i < np; i++) {
                if (c[i] > max_element) {
                    max_element = c[i];
                }
            }
    }
   double end_time = omp_get_wtime();
    printf("Tempo di esecuzione: %f secondi\n", end_time - start_time);

    printArray(c,np);
    printf("\n\nElemento max= %d", max_element);

    for(i = 0; i < np; i++) {
        free(matrice[i]);
    }
    free(matrice);
    free(c);

    return 0;
}
void printArray(int * array, int size) {
    for (size_t i = 0; i < size; ++i) {
        printf("%d ", array[i]);
    }
}

void fill_matrix(int ** matrix, int row, int col, int max_element) {
#pragma omp parallel for collapse(2)
    for (size_t i = 0; i < row; ++i) {
        for (size_t j = 0; j < col; ++j) {
            matrix[i][j] = rand() % max_element;
        }
    }
}

void print_matrix(int ** matrix, int row, int col) {
    for (size_t i = 0; i < row; ++i) {
        for (size_t j = 0; j < col; ++j) {
            printf("%d ", matrix[i][j]);
        }
        putchar('\n');
    }
}