#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(void)
{
    int i,N,t,nloc,indice,r,id,step,np;
    float *a, *b, *c;

    printf("Con quanti core vuoi eseguire il calcolo?\n");
    scanf("%d",&np);
    printf("Inserire N\n");
    scanf("%d",&N);

    //allocazione
    a=(float*)calloc(N,sizeof(float));
    b=(float*)calloc(N,sizeof(float));
    c=(float*)calloc(N,sizeof(float));

    //lettura
    printf("Inserire gli elementi del vettore a\n");
    for(i=0;i<N;i++)
    {
        scanf("%f",&a[i]);
    }

    printf("Inserire gli elementi del vettore b\n");
    for(i=0;i<N;i++)
    {
        scanf("%f",&b[i]);
    }

#pragma omp parallel private(nloc,i,indice,id,step) shared(a,b,c,r) num_threads(np)
    {
        t = omp_get_num_threads();
        nloc = N/t;
        r=N%t;
        id=omp_get_thread_num();
        printf("sono %d, di %d: numeri %d, r=%d\n", id,t,nloc,r);

        if(id<r) {
            nloc++;
            step=0;
        }
        else
            step = r;

        printf("sono %d, di %d: numeri %d, r=%d, passo=%d\n", id,t,nloc,r,step);

        for(i=0;i<nloc;i++)
        {
            indice = i+nloc*omp_get_thread_num()+step;
            c[indice] = a[indice] + b[indice];
        }
    }

    //stampa finale
    for(i=0;i<N;i++)
    {
        printf("%f " , c[i]);
    }
    return 0;
}