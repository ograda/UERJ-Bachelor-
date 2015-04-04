#include<stdio.h>
#define NLPN 6
#define NLUS 8

struct mochila{
    int me;
    int mv;
};

int pd[20][40];
int main()  {
    int i,j, m = 20, t = 5,rep, max=20;

    int p[5] = {0,2,NLPN-2,NLUS-2,3}; // pesos
    int v[5] = {0,2,6,6,8}; // valores
    struct mochila k[m+10];

    for (j=1;j<=m;++j) {
        k[j].me=-1;
        k[j].mv=0;
    }
    k[0].me=0;
    k[0].mv=0;

    for (i=1;i<=t;++i) {
        for(j=m-p[i];j>=0;--j) {
         if ((k[j].me>=0)&&((k[j+p[i]].mv<(k[j].mv+v[i])&&(k[j].mv+v[i]<max))||(((k[j].mv==0)&&(k[j+p[i]].me==-1))))){
             k[j+p[i]].me=i; k[j+p[i]].mv=k[j].mv+v[i]; pd[i][j+p[i]]=k[j].mv+v[i];
             }
        }
    }
    printf("       " );
	for (i=1;i<=m;++i)
        printf("%d ",i%10);
	printf("\n");

	int aux = 0;
	for (i=1;i<t;++i){
		for (j=1;j<=m;++j) {
    		while( (!pd[aux][j]) && (aux<=t)) {
                    pd[aux][i]=pd[j][i];
                    aux++;
            }
        }
    }

    for (i=1;i<=t;++i){
    	printf("PD%d    ",i);
    	for (j=1;j<=m;++j){
    		printf("%d ",pd[i][j]);
    	}
    	printf("\n");
    	}

     printf("\nSOLUCOES:\nPeso|Itens");
    for (i=1;i<=m;++i){
        if ((k[i].me!=-1)){
            printf("\n %d  =  %d",i, k[i].mv);
        }
    }

    return 0;
}

