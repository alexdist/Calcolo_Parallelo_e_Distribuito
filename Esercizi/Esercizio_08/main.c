/*
 *Implementare un programma parallelo per l’ambiente multicore con np unità processanti che impieghi
 *la libreria OpenMP. Il programma deve essere organizzato come segue: il core master deve
 *generare una matrice A di dimensione NxM. Quindi, ogni core deve estrarre N/p colonne ed
 *effettuare localmente la somma degli elementi delle sottomatrici estratte,
 *conservando il risultato in un vettore b di dimensione M.
*/

#include <stdio.h>
#include <omp.h>
#include <time.h>
void fill_matrix(int ** matrix, int row, int col, int max_element);
void print_matrix(int ** matrix, int row, int col);
void printArray(int * array, int size);
int main(void)
{
    srand(time(NULL));
    int **matrice, *vettore;
    int N,M,i,j;
    int n_core;

    printf("Con quanti core vuoi eseguire il calcolo?");
    scanf("%d",&n_core);

    printf("\nInserisci il numero di righe della matrice (N): ");
    scanf("%d", &N);

    printf("\nInserisci il numero di colonne della matrice (M): ");
    scanf("%d",&M);

    matrice = malloc(N*sizeof(int*));
    if(matrice == NULL) {
        printf("Errore nell'allocazione della memoria");
        exit(1);
    }
    for(i=0;i<N;i++) {
        matrice[i]=malloc(M*sizeof(int));
    }

    fill_matrix(matrice,N,M,30);
    print_matrix(matrice,N,M);

    vettore = calloc(M,sizeof(int));

   double start_time=omp_get_wtime();
#pragma omp parallel default(none) shared(matrice, vettore, M,N,n_core) private(i,j) num_threads(n_core)
    {
        //codice parallelo per calcolo manuale colonne da assegnare a ciascun thread
/*
        int tid = omp_get_thread_num();
        int chunk_size = M/n_core;
        int r = M%n_core;

        int start_col, end_col;

        if(tid<r) {
            start_col = tid*(chunk_size+1);
            end_col = start_col+chunk_size+1;
        }else {
            start_col = tid*chunk_size+r;
            end_col = start_col+chunk_size;
        }

        printf("Thread %d sta elaborando colonne da %d a %d\n", tid, start_col, end_col - 1);
for(j=start_col; j<end_col;j++) {
    for(i=0;i<N;i++) {
        vettore[j]+=matrice[i][j];
    }
}*/

        //codice alternativo con calcolo automatico colonne da assegnare equivalente al precedente codice
#pragma omp for schedule(static)
        for (j = 0; j < M; j++) {
            printf("Thread %d sta elaborando la colonna %d\n", omp_get_thread_num(), j);

            for (i = 0; i < N; i++) {
                vettore[j] += matrice[i][j];
            }
        }
    }
   double end_time = omp_get_wtime();

    printf("\nVettore delle somme: \n");
    printArray(vettore,M);
    printf("\n Tempo trascorso: %lf", end_time-start_time);

    for(i = 0; i < N; i++) {
        free(matrice[i]);
    }
    free(matrice);
    free(vettore);

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

void printArray(int * array, int size) {
    for (size_t i = 0; i < size; ++i) {
        printf("%d ", array[i]);
    }
}
