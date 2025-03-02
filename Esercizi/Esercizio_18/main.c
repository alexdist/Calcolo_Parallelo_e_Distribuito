/*Implementare un programma parallelo per l’ambiente multicore con np unità processanti
 *che impieghi la libreria OpenMP. Il programma deve essere organizzato come segue:
 *il core master deve leggere un vettore a, di dimensione N ed uno scalare beta.
 *Quindi i core devono collaborare per verificare se nel vettore esista almeno un elemento uguale al valore beta.
*/

#include <stdio.h>
#include <omp.h>
void fillArray(int * array, int size, double max_element);
void printArray(int * array, int size);
int main(void)
{
    int np;
    int *a;
    int beta;
    int N,i;
    int contatore=0;


    printf("Con quanti core vuoi effettuare il calcolo?");
    scanf("%d",&np);

    printf("Inserisci la dimensione del vettore a (N)= ");
    scanf("%d", &N);

    printf("Inserisci il valore dello scalare beta= ");
    scanf("%d",&beta);

    a = malloc(N*sizeof(int));
    if(a==NULL) {
        printf("Errore nell'allocazione della memoria.");
        return 1;
    }

#pragma omp parallel shared(a,N,beta) private(i) num_threads(np)
    {
#pragma omp master
         {
             fillArray(a,N,30);
             printArray(a,N);
         }

#pragma omp barrier

#pragma omp for reduction(+:contatore)
        for(i=0;i<N;i++) {
            if(a[i] == beta) {
                contatore++;
            }
        }
    }

    printf("\nNel vettore sono presenti %d valori uguali allo scalare beta= %d",contatore,beta);

    free(a);
    return 0;
}

void printArray(int * array, int size) {
    for (size_t i = 0; i < size; ++i) {
        printf("%d ", array[i]);
    }
}

void fillArray(int * array, int size, double max_element) {
#pragma omp parallel for
    for (size_t i = 0; i < size; ++i) {
        array[i] = (double) rand() / (double) RAND_MAX * max_element;
    }
}