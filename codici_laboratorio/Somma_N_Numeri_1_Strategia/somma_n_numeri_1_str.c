#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(void)
{
    int i,N,t,nloc,np;
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

#pragma omp parallel private (sum,nloc,i) shared(sumtot) num_threads(np)
    {
        t=omp_get_num_threads();
        nloc=N/t;
        printf("sono %d, di %d: numeri %d\n", omp_get_thread_num(),t,nloc);

        sum=0;
        for(i=0;i<nloc;i++)
        {
            sum = sum+a[i+nloc*omp_get_thread_num()];
        }
        sumtot+=sum;
    }
    printf("somma totale: %f\n", sumtot);

    return 0;
}
