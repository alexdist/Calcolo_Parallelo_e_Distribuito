/*Implementare un programma parallelo per l’ambiente multicore con np unità processanti che impieghi la libreria OpenMP.
Il programma deve essere organizzato come segue: il core master deve leggere una matrice A di dimensione NxN
ed uno scalare alpha.
Quindi, i core devono collaborare per calcolare il prodotto tra lo scalare alpha e la matrice A,
sfruttando una strategia di parallelizzazione che usi la distribuzione per colonne della matrice A
come decomposizione del dominio.
*/

#include <stdio.h>
#include <omp.h>
#include <time.h>
void fill_matrix(int ** matrix, int row, int col, int max_element);
void print_matrix(int ** matrix, int row, int col);
int main(void)
{
    srand(time(NULL));
    int n_core;
    int **matrice, scalare;
    int N,i,j;

    printf("Con quanti core vuoi eseguire il calcolo?= ");
    scanf("%d",&n_core);

    printf("Inserisci la dimensione della matrice (NxN): ");
    scanf("%d", &N);

    printf("Inserisci il valore dello scalare (alpha): ");
    scanf("%d",&scalare);

    matrice = malloc(N*sizeof(int*));
    if(matrice == NULL) {
        printf("Errore nell'allocazione della memoria");
        exit(1);
    }
    for(i=0;i<N;i++) {
        matrice[i]=malloc(N*sizeof(int));
    }

    fill_matrix(matrice,N,N,20);
    printf("Matrice iniziale: \n");
    print_matrix(matrice,N,N);

    double start_time = omp_get_wtime();
#pragma omp parallel for schedule(static) default(none) shared(matrice, scalare, N) private(i, j) num_threads(n_core)
    for (j = 0; j < N; j++) {
        int thread_id = omp_get_thread_num();
        printf("Thread %d sta elaborando la colonna %d\n", thread_id, j);

        for (i = 0; i < N; i++) {
            matrice[i][j] *= scalare;
        }
    }

   double end_time = omp_get_wtime();

    printf("\nMatrice risultato:\n");
    print_matrix(matrice,N,N);

        printf("\nTempo per il calcolo: %lf", end_time-start_time);


    for (i = 0; i < N; i++) {
        free(matrice[i]);

    }
    free(matrice);

    return 0;
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