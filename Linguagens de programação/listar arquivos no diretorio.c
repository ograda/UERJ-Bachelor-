#include <stdio.h>
#include <dirent.h>
#include <unistd.h>

int identify(char *str) {
    int j, numb = 0, iTemp1=0, iTemp2=0;
    char TEMP;

    for (j = 6; j <= strlen(str); j++) {
        if (str[j] == ']') numb = 0; // fim do numero
        if (numb == 1) {
            TEMP = str[j]; // grava o caractere na variavel temporaria
            iTemp1 = atoi(&TEMP); //recebe o caracter como inteiro
            iTemp2 *= 10; // deslocamento de bits
            iTemp2 += iTemp1; // soma a nova unidade
        }
        if (str[j] == '[') numb = 1; // inicio do numero

    }

    return iTemp2;
}

int main(void) {
    char diretorio[256]; // nome do diretorio
    DIR *dir; // declaração do diretorio
    struct dirent *lsdir; //uso do dirent para listar os arquivos na pasta
    FILE *arq; // declaração do arquivo
    int i,tam=0, max=0;

    //checa o diretorio de execuçao para listar as matrizes
    if (getcwd(diretorio, sizeof(diretorio)) == NULL) {
        perror("getcwd() error");
        return 0;
    }
    else {
        dir = opendir(diretorio);
            /* carrega todas as matrizes no diretorio para fazer a lista */
            while ( ( lsdir = readdir(dir) ) != NULL ) {
                char var[50];
                strcpy (var,lsdir->d_name);
                tam = identify(var); //carrega o id do arquivo da matriz
                if (tam>max) max = tam; //define o total de arquivos de matriz a serem utilizados

            }
         closedir(dir);
    }

    arq = fopen("matrizes.in", "w+");
    if (arq == NULL) {
        printf("Problemas na criação do arquivo matrizes.in.\n");
        return 0;
    }
    else {
        rewind(arq);
        for (i=1; i<=max; i++) {
            fprintf(arq, "matriz[%i].in\n", i);
        }
    }

    return 0;
}
