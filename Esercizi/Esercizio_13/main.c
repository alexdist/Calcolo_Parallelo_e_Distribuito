/*
 *Implementare un programma parallelo per l’ambiente multicore con
 *np unità processanti che impieghi la libreria OpenMP. Il programma
 *deve essere organizzato come segue: il core master deve generare
 *una matrice A di dimensione MxM. Quindi, i core devono collaborare
 *per valutare il minimo valore tra gli elementi di A e successivamente
 *costruire la matrice B i cui elementi sono dati dalla somma di tale
 *valore minimo per gli elementi di posto corrispondente della matrice di partenza.
*/

#include <stdio.h>
#include <omp.h>
#include <limits.h>
void fill_matrix(int ** matrix, int row, int col, int max_element);
void print_matrix(int ** matrix, int row, int col);
int main(void) {

    int np;
    int **matrice;
    int M,i,j;
    int min_val = INT_MAX;

    printf("Con quanti core vuoi eseguire il calcolo?= ");
    scanf("%d",&np);

    printf("Inserisci la dimensione della matrice (MxM)= ");
    scanf("%d",&M);

    matrice = malloc(M*sizeof(int*));
    for(i=0;i<M;i++) {
        matrice[i]=malloc(M*sizeof(int));
    }

    fill_matrix(matrice,M,M,30);
    printf("\nMatrice A:\n");
    print_matrix(matrice,M,M);


    double start_time = omp_get_wtime();
#pragma omp parallel shared(matrice,M,min_val) private(i,j) num_threads(np)
    {

#pragma omp for reduction(min:min_val)
        for(i=0;i<M;i++) {
            for(j=0;j<M;j++) {
                if(matrice[i][j]<min_val)
                    min_val = matrice[i][j];
            }
        }

#pragma omp for
        for(i=0;i<M;i++) {
            for(j=0;j<M;j++) {
                matrice[i][j]+=min_val;
            }
        }

    }
    double end_time = omp_get_wtime();

    printf("\nValore minimo=%d",min_val);
    printf("\n\nMatrice B:\n");
    print_matrix(matrice,M,M);
    printf("\n\nTempo di esecuzione: %f secondi\n", end_time - start_time);

    for (i = 0; i < M; i++) {
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