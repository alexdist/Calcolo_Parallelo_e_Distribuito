#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(void)
{
    int *a, N, i, nloc, r, step, id, t, np;
    int sum = 0;

    printf("Con quanti core vuoi eseguire il calcolo?\n ");
    scanf("%d",&np);

    printf("Inserire N:\n");
    scanf("%d",&N);

    a= calloc(N,sizeof(int));
    if(a==NULL) {
        printf("Errore nell'allocazione della memoria");
        exit(1);
    }

    printf("Inserire gli elementi nel vettore:\n ");
    for(i=0;i<N;i++) {
        printf("Inserire l'elemento %d-esimo: ", i);
        scanf("%d", &a[i]);
    }

#pragma omp parallel default(none) private(i,nloc,r,step,id) shared(N,a,t) reduction(+:sum) num_threads(np)
    {
        id = omp_get_thread_num();
        t = omp_get_num_threads();
        nloc = N/t;
        r = N%t;

        if(id<r) {
            nloc++;
            step = 0;
        }
        else
            step=r;

        for(i=0;i<nloc;i++) {
            int index = i+nloc*id+step;
            sum += a[index];
        }
    }
    printf("La somma degli elementi del vettore è %d", sum);

    return 0;
}
