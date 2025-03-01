/*Implementare un programma parallelo per l’ambiente multicore con np unità
 *processanti che impieghi la libreria OpenMP. Il programma deve essere
 *organizzato come segue: il core master deve generare una matrice A
 *di dimensione NxM. Quindi, i core devono collaborare per calcolare
 *il minimo tra gli elementi delle colonne di propria competenza e conservare
 *tale valore in un vettore c di dimensione M.
*/

#include <stdio.h>
#include <omp.h>
#include <limits.h>
#include <time.h>
void fill_matrix(int ** matrix, int row, int col, int max_element);
void print_matrix(int ** matrix, int row, int col);
void printArray(int * array, int size);
int main(void) {

    srand(time(NULL));

    int **matrice, *vettore;
    int N,M,i,j;
    int n_core;
    int minimo = INT_MAX;

    printf("Con quanti core vuoi effettuare il calcolo?= ");
    scanf("%d",&n_core);

    printf("Inserisci il numero di righe della matrice (N)= ");
    scanf("%d",&N);
    printf("\nInserisci il numero di colonne della matrice (M)= ");
    scanf("%d",&M);

    matrice = malloc(N*sizeof(int*));
    for(i=0;i<N;i++) {
        matrice[i]=malloc(M*sizeof(int));
    }

    fill_matrix(matrice,N,M,20);
    printf("Matrice: \n");
    print_matrix(matrice,N,M);

    vettore = calloc(M,sizeof(int));

   double start_time = omp_get_wtime();
#pragma omp parallel for default(none) shared(matrice,vettore,N,M) private(i,j,minimo) num_threads(n_core)
    {
            for(j=0;j<M;j++) {
                minimo = INT_MAX;
                for(i=0;i<N;i++) {
                    if(matrice[i][j]<minimo) {
                        minimo = matrice[i][j];
                    }
                }
                vettore[j] = minimo;
                printf("Thread %d ha elaborato la colonna %d\n", omp_get_thread_num(), j);

            }
    }
   double end_time = omp_get_wtime();

    printf("Vettore dei minimi: \n");
    printArray(vettore,M);

    printf("\n Tempo totale per il calcolo: %lf", end_time-start_time);

    for(i = 0; i < N; i++) {
        free(matrice[i]);
    }
    free(matrice);
    free(vettore);

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





