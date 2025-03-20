#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void print_matrix(int **matrice, int righe, int colonne);
void free_matrix(int **matrice, int N);
void read_matrix(int **matrice, int righe, int colonne);

int main(void)
{
    int N, M, nq, np, n_thread;
    int i, j;
    int **matrice;
    int alpha;

    // Input dell'utente per divisione della matrice
    printf("Inserisci il numero di divisioni per righe (nq): ");
    scanf("%d", &nq);

    printf("Inserisci il numero di divisioni per colonne (np): ");
    scanf("%d", &np);

    // Input per dimensioni della matrice
    printf("\nInserisci le righe della matrice (N): ");
    scanf("%d", &N);

    printf("Inserisci le colonne della matrice (M): ");
    scanf("%d", &M);

    printf("Inserisci lo scalare alpha: ");
    scanf("%d", &alpha);

    // Allocazione della matrice
    matrice = malloc(N * sizeof(int *));
    for (i = 0; i < N; i++) {
        matrice[i] = malloc(M * sizeof(int));
    }

    printf("\nInserisci gli elementi della matrice (%dx%d)\n", N, M);
    read_matrix(matrice, N, M);

    printf("\nMatrice originale:\n");
    print_matrix(matrice, N, M);

    // Calcola le dimensioni dei blocchi
    int sizeBloccoRighe = (N + nq - 1) / nq;
    int sizeBloccoColonne = (M + np - 1) / np;

    // L'utente sceglie il numero di thread
    printf("\nQuanti thread vuoi usare? ");
    scanf("%d", &n_thread);

    // Verifica che il numero di thread non sia superiore a quelli disponibili
    if (n_thread > omp_get_max_threads()) {
        n_thread = omp_get_max_threads();
    }

    // Numero totale di blocchi
    int num_blocchi = nq * np;

    printf("\nNumero di thread usati: %d\n", n_thread);
    printf("Numero di blocchi (nq * np): %d\n", num_blocchi);
    printf("Size Blocco Righe: %d, Size Blocco Colonne: %d\n", sizeBloccoRighe, sizeBloccoColonne);

    // Imposta il numero di thread
    omp_set_num_threads(n_thread);

    // Parallelizzazione con OpenMP
#pragma omp parallel for collapse(2) schedule(dynamic,1)
    for (int bloccoRiga = 0; bloccoRiga < nq; bloccoRiga++) {
        for (int bloccoColonna = 0; bloccoColonna < np; bloccoColonna++) {
            int thread_id = omp_get_thread_num();

            // Calcola i limiti del blocco
            int inizioRiga = bloccoRiga * sizeBloccoRighe;
            int fineRiga = (bloccoRiga + 1) * sizeBloccoRighe;
            if (fineRiga > N) fineRiga = N;

            int inizioColonna = bloccoColonna * sizeBloccoColonne;
            int fineColonna = (bloccoColonna + 1) * sizeBloccoColonne;
            if (fineColonna > M) fineColonna = M;

            printf("Thread %d lavora su righe [%d-%d] e colonne [%d-%d]\n",
                   thread_id,
                   inizioRiga, fineRiga - 1,
                   inizioColonna, fineColonna - 1);

            // Calcola il prodotto scalare per ogni elemento del blocco assegnato
            for (int i = inizioRiga; i < fineRiga; i++) {
                for (int j = inizioColonna; j < fineColonna; j++) {
                    matrice[i][j] *= alpha;
                }
            }
        }
    }

    printf("\nMatrice dopo il prodotto con alpha: \n");
    print_matrix(matrice, N, M);

    free_matrix(matrice, N);

    return 0;
}

void read_matrix(int **matrice, int righe, int colonne) {
    for (int i = 0; i < righe; i++) {
        for (int j = 0; j < colonne; j++) {
            printf("Inserisci l'elemento[%d][%d]-->", i, j);
            scanf("%d", &matrice[i][j]);
            printf("\n");
        }
    }
}

void print_matrix(int **matrice, int righe, int colonne) {
    for (int i = 0; i < righe; i++) {
        for (int j = 0; j < colonne; j++) {
            printf("%d ", matrice[i][j]);
        }
        putchar('\n');
    }
}

void free_matrix(int **matrice, int N) {
    for (int i = 0; i < N; i++) {
        free(matrice[i]);
    }
    free(matrice);
}

