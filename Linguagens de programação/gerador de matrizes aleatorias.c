#include <stdio.h>
#include <stdlib.h>

int main() {
    srand(time(NULL)); //semente do rand
    int Reg = rand()%10+1,i,j; //numero randomico de linhas e colunas
    FILE *arq; // declaração do arquivo
    double **matrizA, **matrizB; // declaração das futuras matrizes

    matrizA = (double**) malloc(Reg*sizeof(double*)); // aloca o vetor para as linhas
    matrizB = (double**) malloc(Reg*sizeof(double*)); // aloca o vetor para as linhas
        for (i=0; i<Reg; i++) {
            matrizA[i] = (double*) malloc(Reg*sizeof(double)); // aloca o vetor para cada coluna
            matrizB[i] = (double*) malloc(Reg*sizeof(double)); // aloca o vetor para cada coluna
            for (j=0; j<Reg; j++) {
                matrizA[i][j] = rand()%1000+1; // atribui um valor randomico para cada elemento de cada linha X coluna
                matrizB[i][j] = rand()%1000+1; // atribui um valor randomico para cada elemento de cada linha X coluna
            }
    }

    arq = fopen("matriz[n].txt", "w");
    if (arq == NULL) {
        printf("Problemas na criação do arquivo.\n");
        return 0;
    }
    else {
        fprintf(arq, "%i\n", Reg);//define o tamanho das matrizes (1 linha)

        for (i=0; i<Reg; i++) { //passagem da matriz 1
            for (j=0; j<Reg; j++) {
                fprintf(arq,"%.0f",matrizA[i][j]);
                if ((Reg-j) > 1 ) fprintf(arq," ");
            }
            fprintf(arq,"\n");
        }

        for (i=0; i<Reg; i++) { //passagem da matriz 2
            for (j=0; j<Reg; j++) {
                fprintf(arq,"%.0f",matrizB[i][j]);
                if ((Reg-j) > 1 ) fprintf(arq," ");
            }
            fprintf(arq,"\n");
        }
        printf("Matrizes geradas com sucesso, arquivo salvo\n");
        fclose(arq);
    }

    for (i=0; i<Reg; i++) { // libera a memoria alocada para as matrizes
            free(matrizA[i]);
            free(matrizB[i]);
    }
    free(matrizA);
    free(matrizB);

    return 0;
}
