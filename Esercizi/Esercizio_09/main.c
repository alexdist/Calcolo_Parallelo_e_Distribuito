/*Implementare un programma parallelo per l’ambiente multicore con np unità
 *processanti che impieghi la libreria OpenMP. Il programma deve essere
 *organizzato come segue: il core master deve generare due vettori casuali
 *di dimensione N. Ogni core deve sommare ad ogni elemento dei due vettori
 *il proprio identificativo e collaborare alla costruzione di una matrice,
 *di dimensione NxM, con M=2np, le cui colonne, a due a due, sono i due
 *vettori ottenuti nella fase precedente.
*/
#include <stdio.h>
#include <omp.h>
#include <time.h>
void print_matrix(int ** matrix, int row, int col);
void fill_matrix(int ** matrix, int row, int col, int max_element);
void printArray(int * array, int size);
void fillArray(int * array, int size, double max_element);
int main(void) {
    srand(time(NULL));
    int **matrice, *vettore1, *vettore2;
    int M,N,i,j;
    int np;

    printf("Quanti core vuoi utilizzare per il calcolo?: ");
    scanf("%d",&np);

    printf("Inserisci la dimensione dei vettori (N): ");
    scanf("%d",&N);

    M=2*np;

    matrice = malloc(N*sizeof(int*));
    for(i=0;i<N;i++) {
        matrice[i]=malloc(M*sizeof(int));
    }

    vettore1 = malloc(N*sizeof(int));
    vettore2 = malloc(N*sizeof(int));

    fillArray(vettore1,N,15);
    fillArray(vettore2,N,25);
    printf("\nVettore1: \n");
    printArray(vettore1,N);
    printf("\nVettore2: \n");
    printArray(vettore2,N);

#pragma omp parallel for default(none) shared(matrice,vettore1,vettore2,N,M) private(i,j) num_threads(np)
    {
        for(j=0;j<M;j+=2) {
            printf("\n\nSono il thread %d e sto elaborando la colonna %d e %d\n", omp_get_thread_num(),j,j+1);
            for(i=0;i<N;i++) {
                matrice[i][j]=vettore1[i]+omp_get_thread_num();
                matrice[i][j+1] = vettore2[i]+omp_get_thread_num();

            }
        }
    }
    printf("\n\nMatrice: \n");
   print_matrix(matrice,N,M);


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