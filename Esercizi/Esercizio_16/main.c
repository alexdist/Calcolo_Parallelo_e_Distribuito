/*Implementare un programma parallelo per l’ambiente multicore con nqxnp unità processanti
 *che impieghi la libreria OpenMP. Il programma deve essere organizzato come segue:
 *il core master deve costruire una matrice A, di dimensione MxN, i cui blocchi
 *sono generati casualmente e in parallelo da ogni core.
*/

#include <stdio.h>
#include <time.h>
#include <omp.h>

void fill_matrix(int ** matrix, int row, int col, int max_element);
void print_matrix(int ** matrix, int row, int col);
void free_matrix(int **matrix, int rows);
int main(void)
{
    int nq,np,n_core;
    int **matrice;
    int N,M;
    int i,j;

    printf("Inserisci N: ");
    scanf("%d",&N);

    printf("Inserisci M: ");
    scanf("%d", &M);

    printf("Inserisci core nq: ");
    scanf("%d", &nq);

    printf("Inserisci core np: ");
    scanf("%d", &np);

    n_core = nq * np;
    if (n_core > omp_get_max_threads()) {
        n_core = omp_get_max_threads(); // Evita di creare più thread di quelli disponibili
    }


    // int sizeBlocco = N/nq;

    matrice = calloc(N,sizeof(int*));
    for(int i = 0;i<N;i++) {
        matrice[i] = calloc(M,sizeof(int));
    }

    srand(time(NULL));

   // fill_matrix(matrice,N,M,30);
    print_matrix(matrice,N,M);

    int sizeBloccoRighe = (N + nq - 1) / nq; // Evita problemi di divisione non esatta
    int sizeBloccoColonne = (M + np - 1) / np;
    // Debug: mostra i parametri di divisione
    printf("\nNumero di thread usati: %d\n", n_core);
    printf("Size Blocco Righe: %d\n", sizeBloccoRighe);
    printf("Size Blocco Colonne: %d\n", sizeBloccoColonne);

/*#pragma omp parallel private(i,j) num_threads(n_core)
{
#pragma omp for collapse(2)
    for(int bloccoRiga = 0; bloccoRiga<N;bloccoRiga += sizeBloccoRighe) {
            for(int bloccoColonna=0; bloccoColonna<M; bloccoColonna += sizeBloccoColonne) {

                int thread_id = omp_get_thread_num();
                unsigned int seed = time(NULL) + thread_id; // Seed diverso per ogni thread
               // printf("Thread %d sta lavorando sul blocco (%d, %d)\n", thread_id, bloccoRiga,bloccoColonna);
                printf("Thread %d sta lavorando sulle righe [%d-%d] e colonne [%d-%d]\n",
       thread_id,
       bloccoRiga, bloccoRiga + sizeBloccoRighe - 1,
       bloccoColonna, bloccoColonna + sizeBloccoColonne - 1);


                for(i=bloccoRiga;i<bloccoRiga+sizeBloccoRighe && i<N;i++) {
                    for(j=bloccoColonna; j<bloccoColonna + sizeBloccoColonne && j<M; j++) {
                        matrice[i][j] = rand_r(&seed) %6+1;
                       // printf("Thread %d ha riempito matrice[%d][%d]\n", thread_id, i, j); // Stampa chi sta scrivendo dove
                    }
                }
            }
        }
    }*/
#pragma omp parallel for collapse(2) private(i, j) num_threads(n_core)
    for (int bloccoRiga = 0; bloccoRiga < N; bloccoRiga += sizeBloccoRighe) {
        for (int bloccoColonna = 0; bloccoColonna < M; bloccoColonna += sizeBloccoColonne) {

            int thread_id = omp_get_thread_num();
            unsigned int seed = time(NULL) + thread_id;

            // Determina i limiti esatti del blocco per evitare uscite dalla matrice
            int fineRiga = bloccoRiga + sizeBloccoRighe;
            if (fineRiga > N) fineRiga = N;

            int fineColonna = bloccoColonna + sizeBloccoColonne;
            if (fineColonna > M) fineColonna = M;

            printf("Thread %d sta lavorando sulle righe [%d-%d] e colonne [%d-%d]\n",
                   thread_id,
                   bloccoRiga, fineRiga - 1, // Correzione dei limiti
                   bloccoColonna, fineColonna - 1);

            // Riempimento del blocco con numeri casuali
            for (i = bloccoRiga; i < fineRiga; i++) {
                for (j = bloccoColonna; j < fineColonna; j++) {
                    matrice[i][j] = rand_r(&seed) % 6 + 1;
                }
            }
        }
    }

    print_matrix(matrice,N,M);

    // Deallocazione memoria
    free_matrix(matrice, N);

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

// Funzione per deallocare la memoria della matrice
void free_matrix(int **matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}