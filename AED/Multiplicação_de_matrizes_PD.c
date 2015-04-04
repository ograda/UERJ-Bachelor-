// M1(NLPN x 400),  M2(400 x NLUS),  M3(NLUS x 4), M4(4 x 900).
// AXB * BXA = BXB

#include <stdio.h>
#include <stdlib.h>
#define NLPN 6
#define NLUS 8
#define MATRIZES 4
int DIMENSOES_DE_MULTIPLICACAO[(MATRIZES+1)] = {NLPN,400,NLUS,4,900};


int contador=-1;
void inic_matriz(int **m,int n){
    int i,j;
    for (i=0;i<n;++i)
        for (j=0;j<n;++j)
            m[i][j]=0;
}

void imprime(int **m,int n) {
 int i,j;
    for (i=0;i<n;++i) {
        printf("%d ", i);
        for (j=0;j<n;++j) printf("%10d",m[i][j]);
        printf("\n");
    }
}

void inic_vetor(int *v,int n) {
    int i;
    for (i=0;i<n;i++) v[i]=0;
}

void MP(int **m,int **mat,int * v,int n, int bestcase) {
    int d,i,j,k, flag=n-1, aux,inf;
    inf = ((bestcase != 0) ? 2147483647:0);
    for (d=1;d<flag;++d)
        for(i=1;i<n-d;++i) {
            j=i+d;
            m[i][j]=inf;
            for(k=i;k<=(j-1);++k) {
                aux=m[i][k]+m[k+1][j]+v[i-1]*v[k]*v[j];
                    if (bestcase != 0) {
                        if( aux < m[i][j]) {
                        m[i][j]=aux;
                        mat[i][j]=k;
                        }
                    }
                    else {
                        if( aux > m[i][j]) {
                            m[i][j]=aux;
                            mat[i][j]=k;
                        }
                    }
            }
        }
}

void exp(int i, int j, int **mk ,char * resposta){
    if (i==j) resposta[++contador]=(i+'0');
    else {
        resposta[++contador]='(';
        exp(i,mk[i][j],mk,resposta);
        resposta[++contador]='X';
        exp(mk[i][j]+1,j,mk,resposta);
        resposta[++contador]=')';
    }
}

int main (){
    int **m,**mk,**pk,*v,i,n,j;
    n = 1 + MATRIZES;
    m=(int**) malloc(sizeof(int *)*n);
    mk=(int**) malloc(sizeof(int *)*n); // melhor caso
    pk=(int**) malloc(sizeof(int *)*n); // pior caso

    for (i=0;i<n;++i){
        m[i]=(int*)malloc(sizeof(int)*n);
        mk[i]=(int*) malloc(sizeof(int)*n);
        pk[i]=(int*) malloc(sizeof(int)*n);
    }

    v=(int*) malloc (sizeof(int)*n);
    inic_vetor(v,n);

    for(i=0;i<n;++i)
        v[i] = DIMENSOES_DE_MULTIPLICACAO[i];

    inic_matriz(m,n);
    inic_matriz(mk,n);
    inic_matriz(pk,n);

    for(i=0;i<n;++i)  m[0][i]=i;

    MP(m,mk,v,n, 1);
    printf("Custos:\n");
    imprime(m,n);

    printf("Melhor caso:\n");
    imprime(mk,n);

    inic_matriz(m,n);
    MP(m,pk,v,n, 0);
    printf("Custos:\n");
    imprime(m,n);

    printf("Pior caso:\n");
    imprime(pk,n);

    char * ansMK=(char *) malloc(sizeof(char)*200);
    exp(1,4,mk,ansMK);
    ansMK[++contador]='\0';
    printf("Melhor caso = %s\n",ansMK);

    contador=-1;
    char * ansPK=(char *) malloc(sizeof(char)*200);
    exp(1,4,pk,ansPK);
    ansPK[++contador]='\0';
    printf("Pior caso = %s\n",ansPK);

    free(m);
    free(mk);
    free(pk);
    free(ansMK);
    free(ansPK);
    return 0;
}
