#include<stdio.h>
#define NLPN 6
#define NLUS 8

struct mochila{
    int me;
    int mv;
};

int pd[20][40];
int main()  {
    int i,j, m = 20, t = 5,rep, max=20000;

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
        for(j=p[i];j<=m;++j){
         if ((k[j-p[i]].me>=0)&&((k[j].mv<(k[j-p[i]].mv+v[i])&&(k[j-p[i]].mv+v[i]<max))||((k[j].mv==0)&&(k[j].me==-1)))){
            k[j].me=i; k[j].mv=k[j-p[i]].mv+v[i];  pd[i][j+p[i]]=k[j].mv+v[i];
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
            printf("\n %d  = R$ %d",i, k[i].mv);
        }
    }

    return 0;
}

