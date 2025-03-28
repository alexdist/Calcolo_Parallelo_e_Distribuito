#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void read_matrix(int **matrice, int righe, int colonne);
void print_matrix(int **matrice, int righe, int colonne);
void free_matrix(int **matrice, int N);
int main(void)
{
    int righe, colonne;
    int **matrice, *v, *x;
    int nloc, tid, np, num_threads, step, r;

    printf("Con quanti core vuoi eseguire il calcolo? ");
    scanf("%d",&np);

    printf("Inserire il numero di righe: ");
    scanf("%d",&righe);

    printf("Inserire il numero di colonne: ");
    scanf("%d",&colonne);

    matrice = malloc(righe*sizeof(int*));
    for(int i=0;i<righe;i++) {
        matrice[i]=malloc(colonne*sizeof(int));
    }

    v = calloc(righe,sizeof(int));
    x = malloc(colonne*sizeof(int));

    printf("Inserire gli elementi del vettore x: \n");
    for(int i=0;i<colonne;i++) {
        printf("Elemento [%d]-->", i);
        scanf("%d",&x[i]);
        //printf("\n");
    }

    printf("Inserire gli elementi della matrice\n");
    read_matrix(matrice,righe,colonne);

    printf("Matrice: \n");
    print_matrix(matrice,righe,colonne);

#pragma omp parallel private(nloc, step, r, tid) shared(matrice, v, x, righe, colonne, num_threads) num_threads(np)
    {
        tid = omp_get_thread_num();
        num_threads = omp_get_num_threads();
        nloc = colonne/num_threads;
        r = colonne%num_threads;

        if(tid<r) {
            nloc++;
            step=0;
        }
        else {
            step = r;
        }

        for(int j=0;j<nloc;j++) {
            int index = j+tid*nloc+step;
            for(int i=0;i<righe;i++) {
#pragma omp atomic
                v[i] += matrice[i][index] * x[index];
            }
        }
    }

    printf("\nProdotto MatricexVettore v:\n");
    for(int i=0;i<righe;i++) {
        printf("%d", v[i]);
        printf("\n");
    }

    free_matrix(matrice,righe);

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