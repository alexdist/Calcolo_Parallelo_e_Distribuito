#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(void)
{
    int i,N,t,nloc,np,r,id,step;
    float sumtot,sum,*a;
    sumtot=0;

    printf("Con quanti core vuoi eseguire il calcolo?\n");
    scanf("%d",&np);
    printf("Inserire N\n");
    scanf("%d",&N);

    a=calloc(N,sizeof(float));

    printf("Inserire i numeri da sommare\n");
    for(i=0;i<N;i++)
    {
        scanf("%f", &a[i]);
    }

#pragma omp parallel private (sum,nloc,i,id,step) shared(sumtot,r) num_threads(np)
    {
        t=omp_get_num_threads();
        nloc=N/t;
        r=N%t;
        id=omp_get_thread_num();
        printf("sono %d, di %d: numeri %d, r=%d\n", id,t,nloc,r);

        if(id<r)
        {
            nloc++;
            step=0;
        }
        else
            step=r;

        printf("sono %d, di %d: numeri %d, r=%d, passo=%d\n", id,t,nloc,r,step);

        sum=0;
        for(i=0;i<nloc;i++)
        {
            sum = sum+a[i+nloc*omp_get_thread_num()+step];
        }

        printf("sono %d, di %d: numeri %d, r=%d, la mia sum=%f\n", id,t,nloc,r,sum);
        sumtot+=sum;
    }
    printf("somma totale: %f\n", sumtot);

    return 0;
}