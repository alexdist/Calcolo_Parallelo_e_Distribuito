/*Implementare un programma parallelo per l’ambiente multicore con np unità processanti che
impieghi la libreria OpenMP. Il programma deve essere organizzato come segue: il core master
deve leggere una matrice di dimensione NxM, quindi i core devono collaborare per sommare
in parallelo i vettori corrispondenti alle righe dispari della matrice.*/

#include <stdio.h>
#include <omp.h>
#include <time.h>
void fill_matrix(int ** matrix, int row, int col, int max_element);
void print_matrix(int ** matrix, int row, int col);
void printArray(int * array, int size);
int main(void)
{
    srand(time(NULL));
    int np;
    int **matrice;
    int *vettore;
    int N,M,i,j;
    int somma;

    printf("Con quanti core vuoi effettuare il calcolo?= ");
    scanf("%d",&np);

    if (np < 1 || np > omp_get_max_threads()) {
        printf("Numero di core non valido. Impostato al massimo disponibile: %d\n", omp_get_max_threads());
        np = omp_get_max_threads();
    }


    printf("Inserisci il numero di righe (N): ");
    scanf("%d",&N);

    printf("Inserisci il numero di colonne (M): ");
    scanf("%d", &M);

    matrice = malloc(N*sizeof(int*));
    if(matrice == NULL) {
        printf("Errore nell'allocazione della memoria");
        return 1;
    }
    for(i=0;i<N;i++) {
        matrice[i] = malloc(M*sizeof(int));
    }

    vettore = calloc((N+1)/2,sizeof(int)); //(N+1)/2 per gestire il caso in cui N è dispari

    fill_matrix(matrice,N,M,30);
    print_matrix(matrice,N,M);



   double start_time = omp_get_wtime();
#pragma omp parallel for default(none) shared(matrice,vettore, N,M) private(i,j,somma) num_threads(np)
    {
        for(i=1;i<N;i+=2) {
            somma = 0;
            for(j=0;j<M;j++) {
                somma += matrice[i][j];
            }

            vettore[i/2] = somma;
            printf("Thread %d ha elaborato la riga %d\n", omp_get_thread_num(), i);
        }
    }
   double end_time = omp_get_wtime();


    printf("Vettore delle somme: \n");
    printArray(vettore,N/2);

    printf("\n\nTempo impiegato per il calcolo: %lf", end_time-start_time);

    // Free memory
    for (i = 0; i < N; i++) {
        free(matrice[i]);
    }
    free(matrice);
    free(vettore);

    return 0;
}

void printArray(int * array, int size) {
    for (size_t i = 0; i < size; ++i) {
        printf("%d ", array[i]);
    }
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