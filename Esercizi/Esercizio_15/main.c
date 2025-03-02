/*Implementare un programma parallelo per l’ambiente multicore con np
 *unità processanti che impieghi la libreria OpenMP. Il programma
 *deve essere organizzato come segue: il core master deve leggere
 *un vettore a, di dimensione M, e uno scalare alpha.
 *Quindi i core devono collaborare per costruire una matrice A,
 *di dimensione MxM, la cui diagonale principale ha per elementi
 *quelli del vettore a moltiplicati per lo scalare alpha.
*/

#include <stdio.h>
#include <omp.h>
void fillArray(int * array, int size, double max_element);
void printArray(int * array, int size);
void fill_matrix(int ** matrix, int row, int col, int max_element);
void print_matrix(int ** matrix, int row, int col);
int main(void)
{
    int np;
    int *a;
    int **matrice;
    int M,i,j;
    int alpha;

    printf("Con quanti core vuoi effettuare il calcolo?= ");
    scanf("%d",&np);

    printf("Inserisci la dimensione della matrice (MxM)= ");
    scanf("%d",&M);

    printf("Inserisci il valore dello scalare (alpha)= ");
    scanf("%d",&alpha);

    matrice = malloc(M*sizeof(int*));
    if(matrice == NULL) {
        printf("Errore nell'allocazione della memoria.");
        return 1;
    }
    for(i=0;i<M;i++) {
        matrice[i] = malloc(M*sizeof(int));
    }

    a = calloc(M,sizeof(int));

    fill_matrix(matrice,M,M,30);
    printf("Matrice di partenza: \n");
    print_matrix(matrice,M,M);
    fillArray(a,M,10);
    printf("\n\nVettore di partenza: \n");
    printArray(a,M);

double start_time = omp_get_wtime();
#pragma omp parallel for shared(matrice,a,M,alpha) private(i,j) num_threads(np)
    {
        for(i=0;i<M;i++) {
            matrice[i][i] = a[i] * alpha;
        }

    }
    double end_time = omp_get_wtime();

    printf("\n\nMatrice dopo l'elaborazione: \n");
    print_matrix(matrice,M,M);
    printf("\n\nTempo trascorso per il calcolo: %lf", end_time-start_time);

    for(i=0;i<M;i++) {
        free(matrice[i]);
    }
    free(matrice);
    free(a);


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