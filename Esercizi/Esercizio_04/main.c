/*
*Implementare un programma parallelo per l’ambiente multicore con np unità processanti che impieghi la libreria OpenMP.
Il programma deve essere organizzato come segue: il core master deve leggere una matrice di dimensione NxN,
quindi i core devono collaborare per ricopiare in parallelo gli elementi della diagonale principale
in un vettore di lunghezza N. Infine, i core devono effettuare la somma degli elementi di tale vettore in parallelo.
 **/

#include <stdio.h>
#include <omp.h>
#include <time.h>

void allocate_matrix(int ***matrix, int N);
void free_matrix(int **matrix, int N);
void fill_matrix(int **matrix, int N, int max_element);
void print_matrix(int ** matrix, int row, int col);
void printArray(int * array, int size);

int main(void) {
    int n_core, N, i;
    int **matrice, *vettore;
    int sum = 0;

    srand(time(NULL));
    printf("Con quanti core eseguire il calcolo? ");
    scanf("%d", &n_core);

    printf("Inserisci la dimensione (N): ");
    scanf("%d", &N);

    // Allocazione e riempimento della matrice eseguite dal thread master
    allocate_matrix(&matrice, N);
    fill_matrix(matrice, N, 30);
    print_matrix(matrice,N,N);

    vettore = malloc(N * sizeof(int));
    if (vettore == NULL) {
        fprintf(stderr, "Errore nell'allocazione della memoria del vettore\n");
        exit(1);
    }

    double start_time = omp_get_wtime();
    // Avvio della regione parallela con variabili condivise tra i thread
#pragma omp parallel default(none) shared(matrice, vettore, N,sum) private(i) num_threads(n_core)
    {

        #pragma omp for
        for (i = 0; i < N; i++) {
            vettore[i] = matrice[i][i];
            printf("\nThread %d ha copiato l'elemento [%d][%d] = %d nella diagonale\n", omp_get_thread_num(), i, i, vettore[i]);
        }

        #pragma omp for reduction(+:sum)
        for (i = 0; i < N; i++) {
            sum += vettore[i];
            printf("\nThread %d sta sommando l'elemento %d = %d alla somma totale\n", omp_get_thread_num(), i, vettore[i]);
        }

    }
    double end_time = omp_get_wtime();

    // Stampa dei risultati
    printf("\n=== Risultati ===\n");
    printArray(vettore,N);
    printf("\nSomma totale: %d\n", sum);
    printf("Tempo calcolo regione parallela: %lf secondi\n", end_time - start_time);

    // Deallocazione della memoria
    free(vettore);
    free_matrix(matrice, N);

    return 0;
}

void printArray(int * array, int size) {
    for (size_t i = 0; i < size; ++i) {
        printf("%d ", array[i]);
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

// Funzione per allocare la matrice
void allocate_matrix(int ***matrix, int N) {
    *matrix = malloc(N * sizeof(int *));
    if (*matrix == NULL) {
        fprintf(stderr, "Errore nell'allocazione della matrice\n");
        exit(1);
    }
    for (int i = 0; i < N; i++) {
        (*matrix)[i] = malloc(N * sizeof(int));
        if ((*matrix)[i] == NULL) {
            fprintf(stderr, "Errore nell'allocazione della riga %d\n", i);
            exit(1);
        }
    }
}

// Funzione per liberare la matrice
void free_matrix(int **matrix, int N) {
    for (int i = 0; i < N; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

// Funzione per riempire la matrice con valori casuali
void fill_matrix(int **matrix, int N, int max_element) {
#pragma omp parallel for collapse(2)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = rand() % max_element;
        }
    }
}
