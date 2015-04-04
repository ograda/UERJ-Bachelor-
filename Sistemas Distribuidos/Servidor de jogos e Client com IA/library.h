#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>


#define PROTOPORT 7171            /* default protocol port number */
#define QLEN 10               /* size of request queue */
#define NUM_THREADS 20  /* maximum thread size -- numero de conexões aceitas após*/
#define games 4

#define inqueue 700
#define leftqueue 701
#define foundmatch 702
#define endmatch 3412
#define initializegame 52342
#define GAMEMaiorMenor 40341
#define GAMEParOuImpar 42323
#define GAMEBlackJack 33231
#define winstat 4324325

#define max(a,b) (a>b) ? a:b;
#define min(a,b) (a<b) ? a:b;
#define clear() system("clear");
#define switchTurn(a) (a==1) ? 2:1;
#define games 4
char gameNames[games][20] = {"","Maior Menor", "Par ou Impar", "Black Jack", "Jogo da Velha"};
#define localhost "127.0.0.1";    /* default host name */
#define closesocket close

typedef enum { false, true } bool;
struct message { int code; char str[132];};

// receptor e tratador de erros de recepção de pacotes
inline void socketSend(int socket,int code, char *mensagem) {
   int n;
   struct message msg;
   msg.code = code;
   memcpy((char *)&msg.str, mensagem, sizeof mensagem);
   n = send(socket, &msg, sizeof msg, 0);
   if (n < 0) perror("ERROR writing to socket");
}

// Envio e tratamento de erros de envio de pacotes
inline struct message socketReceive(int socket, int *n) {
   struct message msg;
   *n = recv(socket, &msg, sizeof msg, 0);
   if (n < 0) perror("ERROR reading from socket");
   return msg;
}

int ifloor(double a) {
    int b = a;
    return (b<=a) ? b:b-1;
}

int getch() {
    unsigned char c;
    if ((c = getchar(), sizeof(c)) <= 0) return 0;
    else return c;
}

int getOption() {
    char opc[3];
    scanf("%s", opc);
    int a = atoi(opc);
    return a ? a : ((opc[0] == '0') ? 0:-1);
}

char putMin(char a) {
    int i = (int) a;
    return ((char) (i < 90) ? i+32:i);
}

char getCharOption() {
    char opc[3], ret;
    scanf("%s", opc);
    ret = putMin(opc[0]);
    return ret;
}

int blackJackPoints(int sum, int as, bool royal) {
    if (sum > 21)
        if (royal)
            while (as-- && sum > 21) sum -=9;
    return sum;
}
