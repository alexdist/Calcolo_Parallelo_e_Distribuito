/*Implementare un programma parallelo per l’ambiente multicore con npxnp unità processanti che impieghi la
 *libreria OpenMP. Il programma deve essere organizzato come segue: il core master deve leggere una
 *matrice A di dimensione MxM. Quindi, ogni core deve estrarre un blocco della matrice A di
 *dimensione (M/np) x (M/np), conservando i valori in npxnp matrici. Infine, i core devono
 *collaborare per calcolare la somma di tali matrici.
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>

// Dichiarazione delle funzioni
int **initMatrix(int, int);
void printMatrix(int **, int, int);

int main() {
    int M, p, i, j, k;
    double start, end;
    int ***extractions;
    int **matrix, **result;

    // Inizializzazione del generatore di numeri casuali
    srand(time(NULL));

    // Input da utente per il numero di thread per riga (p)
    printf("Inserire il numero di thread per ogni riga: ");
    scanf("%d", &p);

    // Input da utente per la dimensione N della matrice quadrata (M)
    printf("\nInserire la dimensione N della matrice quadrata: ");
    scanf("%d", &M);

    // Inizializzazione della matrice con valori casuali
    matrix = initMatrix(M, M);

    // Stampare la matrice iniziale
    printf("Matrice:\n");
    printMatrix(matrix, M, M);

    // Allocazione della memoria per le estrazioni
    extractions = (int ***) calloc(p*p, sizeof(int **));

    // Allocazione della memoria per la matrice risultato
    result = (int **) calloc(M/p, sizeof(int *));
    for (int w=0; w<M/p; w++) {
        result[w] = (int *) calloc(M/p, sizeof(int));
    }

    // Misura del tempo di inizio dell'area parallela
    start = omp_get_wtime();

    // Area parallela con direttive OpenMP
#pragma omp parallel private(i, j, k) shared(matrix, extractions, result, M) num_threads(p*p)
    {
        int tid = omp_get_thread_num();
        int blockPerThread = M / p;
        int startRow = tid / p * blockPerThread;
        int startCol = tid % p * blockPerThread;

        // Allocazione della memoria per l'estrazione corrente
        extractions[tid] = (int **) calloc(blockPerThread, sizeof(int *));
        for (k=0; k<blockPerThread; k++) {
            extractions[tid][k] = (int *) calloc(blockPerThread, sizeof(int));
        }

        // Estrazione dei blocchi dalla matrice principale
        for (i = 0; i < blockPerThread; i++) {
            for (j = 0; j < blockPerThread; j++) {
                extractions[tid][i][j] = matrix[i + startRow][j + startCol];
            }
        }

        // Stampa dell'estrazione corrente in modo critico
#pragma omp critical
        {
            printf("\nThread %d:\n", tid);
            printMatrix(extractions[tid], blockPerThread, blockPerThread);
        }

        // Somma parallela dei blocchi estratti nella matrice risultato
#pragma omp for collapse(3)
        for (i=0; i<p*p; i++) {  // Ciclo su tutte le sottomatrici
            for (j=0; j<blockPerThread; j++) {  // Riga all'interno della sottomatrice
                for (k=0; k<blockPerThread; k++) {  // Colonna all'interno della sottomatrice
                    result[j][k] += extractions[i][j][k];
                }
            }
        }
    }

    // Misura del tempo di fine dell'area parallela
    end = omp_get_wtime();

    // Stampare la matrice risultante dalla somma
    printf("\nMatrice risultante dalla somma:\n");
    printMatrix(result, M/p, M/p);

    // Stampare il tempo di esecuzione dell'area parallela
    printf("\nTempo di esecuzione della regione parallela: %lf\n", end-start);

    // Liberare la memoria allocata per le estrazioni
    for (int l=0; l<M/p; l++) {
        for (int w=0; w<M/p; w++) {
            free(extractions[l][w]);
        }
        free(extractions[l]);
    }
    free(extractions);

    // Liberare la memoria allocata per la matrice principale
    for (int w=0; w<M/p; w++) {
        free(matrix[w]);
    }
    free(matrix);

    // Liberare la memoria allocata per la matrice risultato
    for (int w=0; w<M/p; w++) {
        free(result[w]);
    }
    free(result);

    return 0;
}

// Funzione per inizializzare una matrice con input da utente
int **initMatrix(int rows, int cols) {
    int **mat = (int **) calloc(rows, sizeof(int *));
    for (int row=0; row<rows; row++) {
        mat[row] = (int *) calloc(cols, sizeof(int));
    }

    printf("\nInserire gli elementi della matrice.\n");
    for (int i=0; i<rows; i++) {
        for (int j=0; j<cols; j++) {
            printf("Elemento (%d,%d): ", i, j);
            scanf("%d", &mat[i][j]);
        }
    }
    printf("\n");

    return mat;
}

// Funzione per stampare una matrice
void printMatrix(int **mat, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("\t%d", mat[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}
