#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void print_matrix(int ** matrice, int righe, int colonne);
void free_matrix(int **matrice, int N);
void read_matrix(int **matrice, int righe, int colonne);
int main(void)
{
    int N,M, np;
    int i,j;
    int **matrice;
    int alpha;

    printf("Con quanti core vuoi eseguire il calcolo? ");
    scanf("%d",&np);

    printf("\nInserisci le righe della matrice(NxM): ");
    scanf("%d", &N);

    printf("\nInserisci le colonne della matrice(NxM): ");
    scanf("%d", &M);

    printf("Inserisci lo scalare alpha: ");
    scanf("%d",&alpha);

    matrice = malloc(N*sizeof(int*));
    for(i=0;i<N;i++) {
        matrice[i] = malloc(M*sizeof(int));
    }

    printf("\nInserisci gli elementi della matrice (%dx%d)\n",N,M);
    read_matrix(matrice, N, M);

    printf("\nMatrice originale:\n");
    print_matrix(matrice,N,M);

#pragma omp parallel for shared(N,M,matrice,alpha) private(i,j) num_threads(np)
    {
        for(i=0;i<N;i++) {
            for(j=0;j<M;j++) {
                matrice[i][j]*=alpha;
            }
        }
    }

    printf("\nMatrice dopo il prodotto con alpha: \n");
    print_matrix(matrice,N,M);

    free_matrix(matrice,N);

    return 0;
}

void read_matrix(int **matrice, int righe, int colonne) {
    for(int i=0;i<righe;i++) {
        for(int j=0;j<colonne;j++) {
            printf("Inserisci l'elemento[%d][%d]-->",i,j);
            scanf("%d",&matrice[i][j]);
            printf("\n");
        }
    }
}

void print_matrix(int ** matrice, int righe, int colonne) {
    for (int i = 0; i < righe; i++) {
        for (int j = 0; j < colonne; j++) {
            printf("%d ", matrice[i][j]);
        }
        putchar('\n');
    }
}
void free_matrix(int **matrice, int N) {
    for(int i=0;i<N;i++) {
        free(matrice[i]);
    }
    free(matrice);
}