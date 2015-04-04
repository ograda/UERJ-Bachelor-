#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <signal.h>
#include "criar_matriz.h"

#define DEBUGLOG 1 // 0 para nao fazer debug
#define BUFF_SIZE 5	/* total number of slots */
#define Default_MNumber 15 // quantidade de matrizes
#define SLEEP_BURST 2 // empo do ciclo de sleep para espera do fim das threads

#define LC_N 1
#define LA_N 3
#define MM_N 4
#define DM_N 5
#define EA_N 3

typedef struct {
    char Nome[100];
    int Ordem;
    double **A,**B,**C, E;
} sbuf_matriz;

typedef struct {
    sbuf_matriz buf[BUFF_SIZE];   /* shared var */
    int in;         	  /* buf[in%BUFF_SIZE] is the first empty slot */
    int out;        	  /* buf[out%BUFF_SIZE] is the first full slot */
    sem_t full;     	  /* keep track of the number of full spots */
    sem_t empty;    	  /* keep track of the number of empty spots */
    sem_t mutex;    	  /* enforce mutual exclusion to shared data */
} sbuf_t;

//LC -> LA -> MM -> DM -> EA
sbuf_t shared[4]; // 0 = LC & LA // 1 = LA & MM // 2 = MM e DM // 3 = DM & EA
int qtd_Matriz;
int m_control[5];

void *LC_thread(void *arg) {
    int nit = 0;
    FILE *Catalogo;
    sbuf_matriz item;

    Catalogo = fopen("Catalogo", "r");
    if (Catalogo == NULL) {
        printf("Problemas na abertura do arquivo de catalogo.\n");
        return NULL;
    } else {
        rewind(Catalogo);
        while (m_control[0]) {
            // If there are no empty slots, wait
            sem_wait(&shared[0].empty);
            // If another thread uses the buffer, wait
            sem_wait(&shared[0].mutex);
            if (!m_control[0]) return NULL; // procura itens ainda pendentes
            // define o nome do arquivo na estrutura
            fscanf (Catalogo, "%s", item.Nome);
            shared[0].buf[shared[0].in] = item;
            shared[0].in = (shared[0].in+1)%BUFF_SIZE;
            m_control[0]--;
            // Release the buffer
            sem_post(&shared[0].mutex);
            // Increment the number of full slots
            sem_post(&shared[0].full);
            #if DEBUGLOG
                printf("LC: Arquivo '%s' lido do catalogo.\n", item.Nome);
            #endif
        }
        fclose(Catalogo);
    }
        printf("**- THREAD LC is finished.\n");
    return NULL;
}

void *LA_thread(void *arg) {
    int i,j,tam, nit;
    FILE *Matriz;
    char tmp[50];
    sbuf_matriz item;

    while (m_control[1]) {
        // If there are no filled slots, wait
        sem_wait(&shared[0].full);
        // If another thread uses the buffer, wait
        sem_wait(&shared[0].mutex);
        if (!m_control[1]) return NULL; // procura itens ainda pendentes
        item = shared[0].buf[shared[0].out];
        shared[0].out = (shared[0].out+1)%BUFF_SIZE;
        // Release the buffer
        sem_post(&shared[0].mutex);
            // Increment the number of empty slots
        sem_post(&shared[0].empty);

        Matriz = fopen(item.Nome, "r");
        if (Matriz == NULL) {
            printf("Problemas na abertura do arquivo '%s' contendo uma das estruturas de matriz.\n",item.Nome);
            return NULL;
        } else {
            rewind(Matriz);
            fscanf (Matriz, "%s", tmp);
            tam = atoi(tmp);
            // define a ordem da matriz na estrutura
            item.Ordem = tam;
            // aloca a memoria para as estruturas das matrizes
            item.A = (double**) malloc(tam*sizeof(double*));
            item.B = (double**) malloc(tam*sizeof(double*));
            item.C = (double**) malloc(tam*sizeof(double*));
            for (i=0; i<tam; i++) {
                item.A[i] = (double*) malloc(tam*sizeof(double));
                item.B[i] = (double*) malloc(tam*sizeof(double));
                item.C[i] = (double*) malloc(tam*sizeof(double));
            }
            //preenche a matriz A
            for (i=0; i< tam; i++)
                for (j=0; j< tam; j++)
                    item.A[i][j] = (fscanf (Matriz, "%s", tmp),atof(tmp));
            //preenche a matriz B
            for (i=0; i< tam; i++)
                for (j=0; j< tam; j++)
                    item.B[i][j] = (fscanf (Matriz, "%s", tmp),atof(tmp));

            fclose(Matriz);
            #if DEBUGLOG
                printf("LA: Matrizes do arquivo '%s' foram preenchidas.\n", item.Nome);
            #endif
        }

        // Prepare to write item to buf
        // If there are no empty slots, wait
        sem_wait(&shared[1].empty);
        // If another thread uses the buffer, wait
        sem_wait(&shared[1].mutex);
        shared[1].buf[shared[1].in] = item;
        shared[1].in = (shared[1].in+1)%BUFF_SIZE;

        m_control[1]--;
        // Release the buffer
        sem_post(&shared[1].mutex);
        // Increment the number of full slots
        sem_post(&shared[1].full);
    }
        printf("**- THREAD LA is finished.\n");
    return NULL;
}


void *MM_thread(void *arg) {
    int i, j ,k, nit;
    sbuf_matriz item;
    while (m_control[2]) {
	// Prepare to read item from buf
        // If there are no filled slots, wait
        sem_wait(&shared[1].full);
        // If another thread uses the buffer, wait
        sem_wait(&shared[1].mutex);
        if (!m_control[2]) return NULL; // procura itens ainda pendentes
        item = shared[1].buf[shared[1].out];
        shared[1].out = (shared[1].out+1)%BUFF_SIZE;
        // Release the buffer
        sem_post(&shared[1].mutex);
        // Increment the number of empty slots
        sem_post(&shared[1].empty);

        // BONUS 3 parte 1
        #pragma omp parallel num_threads(2)
        #pragma omp for
        for(i=0;i<item.Ordem ;i++) {
            for(j=0;j<item.Ordem ;j++) {
                item.C[i][j] = 0;
                for(k=0;k<item.Ordem;k++)
                    item.C[i][j] += item.A[i][k]*item.B[k][j];
            }
        }

        #if DEBUGLOG
            printf("MM: Matriz C do arquivo '%s' foi gerada por C=AxB.\n", item.Nome);
        #endif
        // Prepare to write item to buf
        // If there are no empty slots, wait
        sem_wait(&shared[2].empty);
        // If another thread uses the buffer, wait
        sem_wait(&shared[2].mutex);
        shared[2].buf[shared[2].in] = item;
        shared[2].in = (shared[2].in+1)%BUFF_SIZE;

        m_control[2]--;
        // Release the buffer
        sem_post(&shared[2].mutex);
        // Increment the number of full slots
        sem_post(&shared[2].full);
    }
        printf("**- THREAD MM is finished.\n");
    return NULL;
}

void crout(double **A, double **L, double **U, int n) {
	int i, j, k;
	double sum = 0;
	// BONUS 3 parte 2
	// cria as matrizes L e U (lower e uper triangular)
	#pragma omp parallel num_threads(2)
    #pragma omp for
	for (i = 0; i < n; i++) {
		U[i][i] = 1;
	}
    #pragma omp for
	for (j = 0; j < n; j++) {
		for (i = j; i < n; i++) {
			sum = 0;
			for (k = 0; k < j; k++) {
				sum = sum + L[i][k] * U[k][j];
			}
			L[i][j] = A[i][j] - sum;
		}

		for (i = j; i < n; i++) {
			sum = 0;
			for(k = 0; k < j; k++) {
				sum = sum + L[j][k] * U[k][i];
			}
			if (L[j][j] == 0) {
				printf("det(L) close to 0!\n Can't divide by 0...\n");
				exit(EXIT_FAILURE);
			}
			U[j][i] = (A[j][i] - sum) / L[j][j];
		}
	}
}

void *DM_thread(void *arg) {
    int i, j, nit;
    sbuf_matriz item;
    double **L, **U, detL, detU;
    while (m_control[3]) {
	// Prepare to read item from buf
        // If there are no filled slots, wait
        sem_wait(&shared[2].full);
        // If another thread uses the buffer, wait
        sem_wait(&shared[2].mutex);
        if (!m_control[3]) return NULL; // procura itens ainda pendentes
        item = shared[2].buf[shared[2].out];
        shared[2].out = (shared[2].out+1)%BUFF_SIZE;
        // Release the buffer
        sem_post(&shared[2].mutex);
        // Increment the number of empty slots
        sem_post(&shared[2].empty);

        // aloca a memoria para as matrizes triangulares
        L = (double**) malloc(item.Ordem*sizeof(double*));
            U = (double**) malloc(item.Ordem*sizeof(double*));
            for (i=0; i<item.Ordem; i++) {
                L[i] = (double*) malloc(item.Ordem*sizeof(double));
                U[i] = (double*) malloc(item.Ordem*sizeof(double));
            }

        detL = 0; detU = 0;
        // cria as matrizes triangulares
        crout(item.C,L,U,item.Ordem);
        // BONUS 3 parte 3
        // faz o determinante de U e L em paralelo
        #pragma omp parallel num_threads(2)
        #pragma omp for
        for(i=0; i<item.Ordem ;i++) {
            detL += L[i][i];
            detU += U[i][i];
        } item.E = detL*detU;

        // libera a memoria alocada para as matrizes triangulares
        for (i=0; i<item.Ordem; i++) {
            free(L[i]);
            free(U[i]);
        }
        free(L); free(U);


        #if DEBUGLOG
            printf("DM: O determinante de C foi calculado para o arquivo '%s'\n", item.Nome);
        #endif
        // Prepare to write item to buf
        // If there are no empty slots, wait
        sem_wait(&shared[3].empty);
        // If another thread uses the buffer, wait
        sem_wait(&shared[3].mutex);
        shared[3].buf[shared[3].in] = item;
        shared[3].in = (shared[3].in+1)%BUFF_SIZE;

        m_control[3]--;
        // Release the buffer
        sem_post(&shared[3].mutex);
        // Increment the number of full slots
        sem_post(&shared[3].full);
    }
        printf("**- THREAD DM is finished.\n");
    return NULL;
}


void *EA_thread(void *arg) {
    int i, j, nit = 0;
    sbuf_matriz item;
    FILE *output;
    char tmp[50];

    while (m_control[4]) {
        // Prepare to read item from buf
        // If there are no filled slots, wait
        sem_wait(&shared[3].full);
        // If another thread uses the buffer, wait
        sem_wait(&shared[3].mutex);
        if (!m_control[4]) return NULL; // procura itens ainda pendentes

        item = shared[3].buf[shared[3].out];
        shared[3].out = (shared[3].out+1)%BUFF_SIZE;
        sprintf(tmp, "%s.out", item.Nome);
        output = fopen(tmp, "w");
        if (output == NULL) {
            printf("Problemas na criação do arquivo de saida para a entrada %s.\n", item.Nome);
            return NULL;
        } else {
            fprintf(output, "================================================================\n");
            fprintf(output, "<%s> - <%i>\n", item.Nome,item.Ordem);
            fprintf(output, "----------------------------------------------------------------\n");
            // passando a matriz A
            for (i=0; i<item.Ordem; i++)
                for (j=0; j<item.Ordem; j++)
                    if ((item.Ordem-j) > 1 ) fprintf(output,"%.2f ",item.A[i][j]);
                    else fprintf(output,"%.2f\n",item.A[i][j]);
            fprintf(output, "----------------------------------------------------------------\n");
            // passando a matriz B
            for (i=0; i<item.Ordem; i++)
                for (j=0; j<item.Ordem; j++)
                    if ((item.Ordem-j) > 1 ) fprintf(output,"%.2f ",item.B[i][j]);
                    else fprintf(output,"%.2f\n",item.B[i][j]);
            fprintf(output, "----------------------------------------------------------------\n");
            // passando a matriz C
            for (i=0; i<item.Ordem; i++)
                for (j=0; j<item.Ordem; j++)
                    if ((item.Ordem-j) > 1 ) fprintf(output,"%.2f ",item.C[i][j]);
                    else fprintf(output,"%.2f\n",item.C[i][j]);
            fprintf(output, "----------------------------------------------------------------\n");
            fprintf(output,"%.2f\n",item.E);
            fprintf(output, "================================================================\n");
            #if DEBUGLOG
                printf("EA: Arquivo de saida referente a '%s' gerado com sucesso!\n",item.Nome);
            #endif
        fclose(output);
        // libera a memoria alocada para as matrizes
        for (i=0; i<item.Ordem; i++) {
            free(item.A[i]);
            free(item.B[i]);
            free(item.C[i]);
        }
        free(item.A); free(item.B); free(item.C);
    }
    m_control[4]--;
    // Release the buffer
    sem_post(&shared[3].mutex);
    // Increment the number of empty slots
    sem_post(&shared[3].empty);
    }
        printf("**- THREAD EA is finished.\n");
    return NULL;
}


//Bonus 1 - tratamento do sislog - incompleto
void signal_handler(sig)
int sig;
{
		switch(sig){
			case SIGKILL:
			exit(0);
            break;
			case SIGTERM:
            break;
		}
	}

int main(int argc, char** args) {
    pthread_t iLC, iLA, iMM, iDM, iEA;
    int index;
    int LC[LC_N], LA[LA_N], MM[MM_N], DM[DM_N], EA[EA_N];
    bool problem = false;

    if (argc <= 1) {
	    printf("A quantidade de matrizes nao foi enviada, por default estamos usando %d.\n", Default_MNumber); // checagem do numero de arquivos
	    qtd_Matriz = Default_MNumber;
	} else qtd_Matriz = atoi(args[1]);

    //Bonus 1 - fazendo o programa executar como em Daemon e receba sinais de sislog
	pid_t pid = 0;
    pid_t sid = 0;
    pid = fork();
    // Indication of fork() failure
    if (pid < 0) { printf("fork failed!\n");
    exit(1);
    }
    // Hora de matar o pai
    if (pid > 0) { printf("pid of child process %d \n", pid);
    exit(0);
    }
    // cria um novo grupo de sessao
    sid = setsid();
    if(sid < 0) exit(1);
    // fecha todos os descriptors
    for (index = getdtablesize(); index >= 0; --index) close(index);

    //cria um usermask para previnir problemas de privilegio e consequentemente segurança
    umask(027);

    // receptores de sinal
    signal(SIGTSTP,SIG_IGN);
	signal(SIGTTOU,SIG_IGN);
	signal(SIGTTIN,SIG_IGN);
    signal(SIGKILL,signal_handler);
    signal(SIGTERM,signal_handler);
    // pronto, daemon funcionando e recebendo sinais.

	problem = cria_Input(qtd_Matriz); // para de executar se tiver um problema criando a matriz
	if (problem == true) return -1;
    printf("-----------------------------------------\nArquivos Criados com sucesso\n-----------------------------------------\n");
    printf("**- Configurando semaforos e parametros inciiais.\n");
    // inicia o controle dos semáforos
    for (index = 0; index < 4; index++) {
    	sem_init(&shared[index].full, 0, 0);
    	sem_init(&shared[index].empty, 0, BUFF_SIZE);
    	sem_init(&shared[index].mutex, 0, 1);
    	m_control[index] = qtd_Matriz;
    } m_control[4] = qtd_Matriz;

    printf("** -Iniciando a execucao do programa.\n\n");
    for (index = 0; index < LC_N; index++) {
       LC[index] = index;
       pthread_create(&iLC, NULL, LC_thread, (void*) &LC[index]);
    }

    for (index = 0; index < LA_N; index++) {
       LA[index]=index;
       pthread_create(&iLA, NULL, LA_thread, (void*) &LA[index]);
    }

    for (index = 0; index < MM_N; index++) {
        MM[index]=index;
        pthread_create(&iMM, NULL, MM_thread, (void*) &MM[index]);
    }

    for (index = 0; index < DM_N; index++) {
        DM[index]=index;
        pthread_create(&iDM, NULL, DM_thread, (void*) &DM[index]);
    }

    for (index = 0; index < EA_N; index++) {
        EA[index]=index;
        pthread_create(&iEA, NULL, EA_thread, (void*) &EA[index]);
    }

    // Espera a primera finalizacao da thread iEA (decrementando assim o valor das matrizes que ela espera para 0
    while (m_control[4]) sleep(SLEEP_BURST);
    // mata as threads em aberto ja que a execucao terminou
    pthread_cancel(iEA);
    pthread_cancel(iDM);
    pthread_cancel(iMM);
    pthread_cancel(iLA);
    pthread_cancel(iLC);
    printf("**- Execucao terminada com sucesso\n");
    return 0;
}

