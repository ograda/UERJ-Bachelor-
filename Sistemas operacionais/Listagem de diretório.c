#include <stdio.h>
#include <dirent.h>
#include <unistd.h>

int main(void)
{
    char diretorio[256];
    DIR *dir;
    struct dirent *lsdir;

    //checa o diretorio de execução para listar as matrizes
    if (getcwd(diretorio, sizeof(diretorio)) == NULL) {
        perror("getcwd() error");
        return 0;
    }
    else {
        dir = opendir(diretorio);
            /* carrega todas as matrizes no diretorio para fazer a lista */
            while ( ( lsdir = readdir(dir) ) != NULL ) {
                printf ("%s\n", lsdir->d_name); //lista o nome dos arquivos na tela, editem para carregar o arquivo
            }
         closedir(dir);
    }

    return 0;

}
