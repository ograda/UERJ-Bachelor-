#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include "lib.h"

char str[MAX_LINE_LENGTH],labels[MAX_LINE_LENGTH][MAX_LINE_LENGTH];
int labNum[35] = {0};

//DESCOBRE O ENDEREÇO DE UMA TAG
int getTagsAdress(char *tag) {
int i;
   for ( i = 0; i<labNum[0]; i++)
        if (strcmp(labels[i], tag) == 0)
            return labNum[i+1];
   return 0;
}

//DESCOBRE AS TAGS EXISTENTES
void identificarTags(FILE *arq) {
char *labelHDL = NULL;
char strHDL[MAX_LINE_LENGTH];
int line = 0;
   // identifica as tags de jump e salva o numero da linha
   while(fgets(str, MAX_LINE_LENGTH+1, arq) != NULL){
        labelHDL=strchr(str,':');
        if (labelHDL != NULL)  {
            size_t label_len = (labelHDL-str+1);
            strncpy (strHDL, str, labelHDL-str+1);
            strHDL[label_len-1] = '\0';
            strcpy (labels[labNum[0]], strHDL);
            labNum[0]++;
            labNum[labNum[0]] = line;
        }
    line++;
   }
   rewind(arq);
}

//TRADUZ COMANDOS R
void traduzirComandoR(char *comando, char *funct, char *binario, FILE *arq, char op) {
int instruct = 0, i;
char Registradores[3][10];
char *rs="00000", *rt="00000", *rd="00000", opc[]="00000";

    //acha os registradores, explicitos, implicitos e imediatos
     char * Regs;
     Regs = strtok (comando," ,#");
     while (Regs != NULL) {
        sscanf(Regs, "%s", Registradores[instruct]);
        Regs  = strtok (NULL, " ,#");
        instruct++;
     }

     if (instruct < 3) {
        strcpy(Registradores[2], Registradores[0]);
        instruct++;
     }

    // estabelece os registradores, explicitos, implicitos e imediatos
    for(i = 0; i < instruct; i++) {
        if (Registradores[i][0] == '$')
            switch(i) {
                case 0: rs = registerBin(Registradores[i]);
                break;
                case 1: rt = registerBin(Registradores[i]);
                break;
                case 2: rd = registerBin(Registradores[i]);
                break;
            }
        else
           switch(i) {
                case 0: rs = imediateBin(Registradores[i], 4);
                break;
                case 1: rt = imediateBin(Registradores[i], 4);
                break;
                case 2: rd = imediateBin(Registradores[i], 4);
                break;
            }
    }
    printf("\nTipo de Instrução: %s\n",binario);
    printf("Registrador RS: %s\n",rs);
    printf("Registrador RT: %s\n",rt);
    printf("Registrador RD: %s\n",rd);
    printf("Opcode: %s\n",opc);
    printf("Function: %s\n",funct);
    printf("\nCodigo Referente: \n%s%s%s%s%s%s\n",opc,rs,rt,rd,binario,funct);
    fprintf (arq,"%s%s%s%s%s%s",opc,rs,rt,rd,binario,funct);
    if (op == 'l')
    fprintf (arq,"\n");
}

//TRADUZ COMANDOS I
void traduzirComandoI(char *comando, char *funct, char *binario, FILE *arq, char op) {
int instruct = 0, i;
char Registradores[3][16];
char *rs="00000", *rt="00000", *imm="0000000000000000";

     // addi
     if (atoi(funct)==1) {
        char * Regs;
        Regs = strtok (comando," ,#");
        while (Regs  != NULL) {
            sscanf(Regs, "%s", Registradores[instruct]);
            Regs  = strtok (NULL, " ,#");
            instruct++;
        }

        if (instruct < 3) {
        strcpy(Registradores[2], Registradores[1]);
        strcpy(Registradores[1], Registradores[0]);
        instruct++;
        }

        for(i = 0; i < instruct; i++) {
            if (Registradores[i][0] == '$')
                switch(i) {
                    case 0: rs = registerBin(Registradores[i]);
                        break;
                    case 1: rt = registerBin(Registradores[i]);
                        break;
                }
            else
               switch(i) {
                    case 1: rt = imediateBin(Registradores[i], 4);
                        break;
                    case 2: imm = imediateBin(Registradores[i], 15);
                        break;
                }
        }
     }

    //bne e beq
    if (atoi(funct)==2) {
            char * Regs;
            Regs = strtok (comando," ,");
            while ((Regs  != NULL) && (instruct < 3)) {
                sscanf(Regs, "%s", Registradores[instruct]);
                if (instruct < 2)
                Regs  = strtok (NULL, " ,");
                instruct++;
            }

            for(i = 0; i < instruct; i++) {
                if (Registradores[i][0] == '$')
                    switch(i) {
                        case 0: rs = registerBin(Registradores[i]);
                        break;
                        case 1: rt = registerBin(Registradores[i]);
                        break;
                    }
                    else { char regTMP[16];
                           if (atoi(Registradores[i])==0)
                               sprintf(regTMP, "%d", getTagsAdress(Registradores[i]));
                            else
                               strcpy(regTMP,Registradores[i]);
                   switch(i) {
                        case 0: rs = imediateBin(regTMP, 4);
                        break;
                        case 1: rt = imediateBin(regTMP, 4);
                        break;
                        case 2: imm = imediateBin(regTMP, 15);
                        break;
                    }
                }
         }
    }

    // lw e sw
    if (atoi(funct)==3) {
            char * Regs;
            Regs = strtok (comando," ,()");
            while ((Regs  != NULL) && (instruct < 3)) {
                sscanf(Regs, "%s", Registradores[instruct]);
                if (instruct < 2)
                Regs  = strtok (NULL, " ,()");
                instruct++;
            }

            for(i = 0; i < instruct; i++) {
                if (Registradores[i][0] == '$')
                    switch(i) {
                        case 0: rs = registerBin(Registradores[i]);
                        break;
                        case 1: { char regTMP[16];
                                  strcpy(regTMP,registerBin(Registradores[i]));
                                  sprintf(regTMP, "%d", imediateInt(atoi(regTMP)));
                                  imm = imediateBin(regTMP, 15);
                        }
                        break;
                        case 2: rt = registerBin(Registradores[i]);
                        break;
                    }
                else
                   switch(i) {
                        case 1: imm = imediateBin(Registradores[i], 15);
                        break;
                        case 2: rt = imediateBin(Registradores[i], 4);
                        break;
                    }
         }
    }

    printf("\nTipo de instrucao: %s\n",binario);
    printf("Registrador RS: %s\n",rs);
    printf("Registrador RT: %s\n",rt);
    printf("Imediato: %s\n",imm);
    printf("\nCodigo Referente: \n%s%s%s%s\n",binario,rs,rt,imm);
    fprintf (arq,"%s%s%s%s",binario,rs,rt,imm);
    if (op == 'l')
    fprintf (arq,"\n");
}

//TRADUZ COMANDOS J
void traduzirComandoJ(char *comando, char *binario, FILE *arq, char op) {
char Registradores[26];
char *imm="00000000000000000000000000";

char * Regs;
Regs = strtok (comando," ,");
sscanf(Regs, "%s", Registradores);

char regTMP[25];
if (atoi(Registradores)==0)
    sprintf(regTMP, "%d", getTagsAdress(Registradores));
else
    strcpy(regTMP,Registradores);
imm = imediateBin(regTMP, 25);

    printf("\nTipo de instrucao: %s\n",binario);
    printf("Imediato: %s\n",imm);
    printf("\nCodigo Referente: \n%s%s\n",binario,imm);
    fprintf (arq,"%s%s",binario,imm);
    if (op == 'l')
    fprintf (arq,"\n");
}

//ASSOCIA ENDEREÇO DE LABEL
void traduzirLabel(char *comando, int label, FILE *arq, char op) {
char regTMP[32];
char Registradores[3][32];
int instruct = 0;
char *address="00000000000000000000000000000000";

// registradores
char * Regs;
Regs = strtok (comando," ,#");
      while (Regs  != NULL) {
      sscanf(Regs, "%s", Registradores[instruct]);
      Regs  = strtok (NULL, " ,#");
      instruct++;
}

label += 4;
label *= 4;
sprintf(regTMP, "%d", label);
address = imediateBin(regTMP, 31);

    printf("\nTipo de instrucao: label\n");
    printf("Referencia: %i\n",label);
    printf("\nCodigo Referente: \n%s\n",address);
    fprintf (arq,"%s",address);
    if (op == 'l')
    fprintf (arq,"\n");
}


char *encontrarComando(char *frase, int *estruturaTipo, char **func) {
size_t i;
    // identifica o tipo de estrutura encontrado
    while (*estruturaTipo < 0) {
        for (i = 0; i<labNum[0] && *estruturaTipo == -1; i++) {
            if (strncmp(frase, labels[i], strlen(labels[i])) == 0) {
             *estruturaTipo = 4;
             removeCom(&*frase,strlen(labels[i])+1);
             return labels[i];
             break;
            }
        }

        for (i = 0; iMap[i].nome != NULL && *estruturaTipo == -1; i++) {
            if (strncmp(frase, iMap[i].nome, strlen(iMap[i].nome)) == 0) {
             *estruturaTipo = 2;
             *func = iMap[i].tipo;
             removeCom(&*frase,strlen(iMap[i].nome));
             return iMap[i].binario;
             break;
            }
        }

        for (i = 0; rMap[i].nome != NULL && *estruturaTipo == -1; i++) {
            if (strncmp(frase, rMap[i].nome, strlen(rMap[i].nome)) == 0) {
             *estruturaTipo = 1;
             *func = rMap[i].funct;
             removeCom(&*frase,strlen(rMap[i].nome));
             return rMap[i].binario;
            }
        }


        for (i = 0; jMap[i].nome != NULL && *estruturaTipo == -1; i++) {
            if (strncmp(frase, jMap[i].nome, strlen(jMap[i].nome)) == 0) {
             *estruturaTipo = 3;
             removeCom(&*frase,strlen(jMap[i].nome));
             return jMap[i].binario;
             break;
            }
        }

        if (*estruturaTipo == -1) *estruturaTipo = 0;
    }
    return "";
}

//Abertura dos arquivos
FILE *Load(char *fileName, char *type){ //Abre os arquivos
     FILE *arq;
     arq = fopen(fileName, type);
     if (arq == NULL){
        printf("Erro na abertura do arquivo!\n");
        getch();
        exit(0);
        return NULL;
    }
    rewind(arq);
    return arq;
}

int main()
{
   char option;
   FILE *entrada,*saida;
   entrada = Load("input.txt", "rt");
   saida = Load("output.txt", "w+");

   //identifica as tags de jump
   identificarTags(entrada);

   printf("Para traduzir o codigo formatado em linhas, aperte L, \ncaso contrario ele retornara um codigo binario padrao.");
   option = getch();

//decodifica o arquivo de texto
while(fgets(str, MAX_LINE_LENGTH+1, entrada) != NULL){
    char *binario,*funct;
    int estruturaTipo = -1;
    printf("\n-----------------------------------------------------\n");
    printf("%s",str);
    // identifica o tipo de estrutura encontrado
    binario = encontrarComando(str, &estruturaTipo, &funct);
        // direciona o tipo de estrutura para a sua tradução
         switch(estruturaTipo) {
                 case 1: traduzirComandoR(str,funct,binario,saida,option);
                 break;
                 case 2: traduzirComandoI(str,funct,binario,saida,option);
                 break;
                 case 3: traduzirComandoJ(str,binario,saida,option);
                 break;
                 case 4: traduzirLabel(str,getTagsAdress(binario),saida,option);
                 break;
                 default:
                 case 0: printf("Estrutura nao reconhecida em ASSEMBLY!\nNao passada para o arquivo de saida.");
                 break;
         }
     getch();
   } fclose(entrada);
fclose(saida);
return 0;
}
