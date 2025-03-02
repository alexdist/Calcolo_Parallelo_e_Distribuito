/*Implementare un programma parallelo per l’ambiente multicore con npxnp unità processanti
 *che impieghi la libreria OpenMP. Il programma deve essere organizzato come segue:
 *il core master deve generare una matrice A di dimensione NxN. Quindi, ogni core deve
 *sommare tra loro gli elementi di un blocco quadrato della matrice A di dimensione (N/np)x(N/np),
 *conservando i valori in un vettore b.*/


#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

/*// Funzione per stampare la matrice suddivisa in blocchi
void printMatrixWithBlocks(int **matrix, int size, int np) {
    int blockSize = size / np + (size % np != 0 ? 1 : 0);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%2d ", matrix[i][j]);
            if ((j + 1) % blockSize == 0 && j != size - 1) {
                printf("| ");  // Separatore tra blocchi
            }
        }
        printf("\n");
        if ((i + 1) % blockSize == 0 && i != size - 1) {
            for (int k = 0; k < size; k++) {
                printf("---");  // Linea di separazione tra blocchi
            }
            printf("\n");
        }
    }
}*/

// Funzione per stampare la matrice suddivisa in blocchi con indici di riga e colonna
void printMatrixWithBlocks(int **matrix, int size, int np) {
    int blockSize = size / np + (size % np != 0 ? 1 : 0);
    printf("   ");
    for (int j = 0; j < size; j++) {
        printf("%2d ", j);
        if ((j + 1) % blockSize == 0 && j != size - 1) {
            printf("| ");  // Separatore tra blocchi
        }
    }
    printf("\n");
    for (int i = 0; i < size; i++) {
        printf("%2d ", i);
        for (int j = 0; j < size; j++) {
            printf("%2d ", matrix[i][j]);
            if ((j + 1) % blockSize == 0 && j != size - 1) {
                printf("| ");  // Separatore tra blocchi
            }
        }
        printf("\n");
        if ((i + 1) % blockSize == 0 && i != size - 1) {
            printf("   ");
            for (int k = 0; k < size; k++) {
                printf("---");  // Linea di separazione tra blocchi
            }
            printf("\n");
        }
    }
}

// Funzione per l'allocazione di una matrice di dimensione N x N
int **allocateMatrix(int size) {
    int **matrix = calloc(size, sizeof(int *));
    for (int i = 0; i < size; i++) {
        matrix[i] = calloc(size, sizeof(int));
    }
    return matrix;
}

// Funzione per riempire la matrice con valori casuali tra 1 e 30
void fillMatrix(int **matrix, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix[i][j] = 1 + rand() % 30;
        }
    }
}

// Funzione per stampare la matrice
void printMatrix(int **matrix, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%d\t", matrix[i][j]);
        }
        printf("\n");
    }
}

// Funzione per liberare la memoria della matrice
void freeMatrix(int **matrix, int size) {
    for (int i = 0; i < size; i++) {
        free(matrix[i]);
    }
    free(matrix);
}



int main(void) {
    srand(time(NULL));
    int N, np;

    // Input dimensione matrice e numero di thread
    printf("Inserisci la dimensione della matrice (N x N): ");
    scanf("%d", &N);
    printf("Inserisci il numero di thread (np): ");
    scanf("%d", &np);

    // Allocazione della matrice
    int **matrix = allocateMatrix(N);
    fillMatrix(matrix, N);
    printf("\nMatrice generata:\n");
    //printMatrix(matrix, N);
    printMatrixWithBlocks(matrix,N,np);

    // Allocazione del vettore dei risultati
    int *resultArray = calloc(np * np, sizeof(int));

    double startTime = omp_get_wtime();

    // Parallelizzazione con OpenMP
    #pragma omp parallel num_threads(np * np)
    {
        int threadId = omp_get_thread_num();
        int blockSize = N / np + (N % np != 0 ? 1 : 0); //(N + np - 1) / np;
        int startRow = (threadId / np) * blockSize;
        int startCol = (threadId % np) * blockSize;

        printf("Thread %d -> Blocchi righe [%d-%d] colonne [%d-%d]\n",
       threadId, startRow, (startRow + blockSize - 1 < N ? startRow + blockSize - 1 : N - 1),
       startCol, (startCol + blockSize - 1 < N ? startCol + blockSize - 1 : N - 1));

        for (int i = startRow; i < startRow + blockSize && i < N; i++) {
            for (int j = startCol; j < startCol + blockSize && j < N; j++) {
                #pragma omp atomic
                resultArray[threadId] += matrix[i][j];
            }
        }
    }

    double endTime = omp_get_wtime();

    // Stampa del risultato
    printf("\nVettore dei risultati:\n");
    for (int i = 0; i < np * np; i++) {
        printf("%d\t", resultArray[i]);
    }
    printf("\n\nTempo di esecuzione: %f secondi\n", endTime - startTime);

    // Liberazione della memoria
    freeMatrix(matrix, N);
    free(resultArray);

    return 0;
}