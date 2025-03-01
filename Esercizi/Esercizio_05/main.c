
/*
*Implementare un programma parallelo per l’ambiente multicore con np unità processanti che impieghi la libreria OpenMP.
Il programma deve essere organizzato come segue: il core master deve generare una matrice di dimensione NxN,
ogni core deve estrarre N/np righe e calcolare il prodotto puntuale tra i vettori corrispondenti alle righe estratte.
*/

#include <stdio.h>
#include <omp.h>
#include <time.h>

void allocateMatrix(int ***matrix, int rows, int cols);
void freeMatrix(int **matrix, int rows);
void fillMatrix(int **matrix, int rows, int cols);
void print_matrix(int **matrix, int row, int col);

int main(void) {
    srand(time(NULL));

    int **matrix;
    int np;
    int rows, cols, i, j;

    printf("Inserisci il numero di core: ");
    scanf("%d", &np);

    printf("Inserisci le righe (N): ");
    scanf("%d", &rows);
    cols = rows;  // Matrice quadrata N x N

    allocateMatrix(&matrix, rows, cols);
    fillMatrix(matrix, rows, cols);
    print_matrix(matrix, rows, cols);

    // Allocazione della matrice risultato (numThreads x cols)
    int **result;
    allocateMatrix(&result, np, cols);

    double startTime = omp_get_wtime();

    #pragma omp parallel default(none) shared(matrix, result, rows, cols,np) private(i,j) num_threads(np)
    {
        int tid = omp_get_thread_num();
        int chunk_size = rows / np;
        int remainder = rows % np;

        // Calcolo dell'intervallo per ogni thread con gestione del resto
        int start = tid * chunk_size + (tid < remainder ? tid : remainder);
        int end = start + chunk_size + (tid < remainder ? 1 : 0);

        // Assicurarsi che "end" non superi "rows"
        if (end > rows) end = rows;

        // Stampa corretta dell'intervallo
        printf("Thread %d: processa righe [%d - %d]\n", tid, start, end );


        // Inizializza la riga del risultato a 1 per la moltiplicazione
        for (j = 0; j < cols; j++) {
            result[tid][j] = 1;
        }

        // Calcola il prodotto puntuale per le righe assegnate al thread corrente
        for (i = start; i < end; i++) {
            for (j = 0; j < cols; j++) {
                result[tid][j] *= matrix[i][j];
            }
        }
    }

    double endTime = omp_get_wtime();
    // Stampa più chiara del risultato
    printf("\n=== Risultati per thread ===\n");
    for (int t = 0; t < np; t++) {
        printf("Thread %d -> ", t);
        for (int j = 0; j < cols; j++) {
            printf("%d ", result[t][j]);
        }
        printf("\n");
    }
    // Stampa il risultato finale
   // printf("Element-wise product result by threads:\n");
  //  print_matrix(result, np, cols);

    printf("Execution time: %f seconds\n", endTime - startTime);

    freeMatrix(result, np);
    freeMatrix(matrix, rows);
    return 0;
}

// Funzione per allocare la matrice
void allocateMatrix(int ***matrix, int rows, int cols) {
    *matrix = (int **)malloc(rows * sizeof(int *));
    if (*matrix == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(1);
    }
    for (int i = 0; i < rows; i++) {
        (*matrix)[i] = (int *)malloc(cols * sizeof(int));
        if ((*matrix)[i] == NULL) {
            fprintf(stderr, "Memory allocation failed!\n");
            exit(1);
        }
    }
}

// Funzione per liberare la matrice
void freeMatrix(int **matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

// Funzione per riempire la matrice con numeri casuali
void fillMatrix(int **matrix, int rows, int cols) {
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = 1 + rand() % 10;
        }
    }
}

// Funzione per stampare la matrice
void print_matrix(int **matrix, int row, int col) {
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            printf("%d ", matrix[i][j]);
        }
        putchar('\n');
    }
}
