#include "library.h"

//tags para desvios na compilação
#define DEBUG // debuga os resultados e preve movimentos do computador

int visits  =  0;              /* counts client connections    */
int gameIdOnList = 0;
int Queue[games] = {-1,0,0,0};

struct message msg; // parametro de mensagem comum
struct connection {
    pthread_t thread_id;
    int  sd, sd2, sock;
};

struct gameMaiorMenor {
    int numChosen, number, turno;
};

struct gameParImpar {
    int p1, p2, turno;
    char p1Opt;
};

struct gameBlackJack {
    int sump1, sump2, turno;
};

union GamesStruct {
    struct gameMaiorMenor MaMe;
    struct gameParImpar ParImpar;
    struct gameBlackJack BlkJack;
};

struct GameOnlineList {
    int pos;
    union GamesStruct GamesOn;
    struct GameOnlineList *next;
    struct GameOnlineList *prev;
};

typedef struct GameOnlineList Deque;
Deque *curr, *head = NULL, *tail = NULL;

void addGame(union GamesStruct jogo, int id) {
    curr = (Deque *)malloc(sizeof(Deque));
    curr->GamesOn = jogo;
    curr->pos = id;
    if (head) {
        curr->prev = head;
        head->next = curr;
    }
    else {
        curr->prev = NULL;
        tail = curr;
    }
    curr->next = NULL;
    head = curr;
}

Deque *listAT(int pos) {
    Deque *TAIL = tail, *HEAD = head, *hdl = NULL;
    while (TAIL->pos != pos && HEAD->pos != pos && TAIL->pos < HEAD->pos) {
        TAIL = TAIL->next;
        HEAD = HEAD->prev;
    }
    if (TAIL->pos <= HEAD->pos) hdl = (TAIL->pos != pos) ? HEAD:TAIL;
    return hdl;
}

void removeGame(int pos) {
    Deque *TAIL = tail, *HEAD = head, *toDelete = NULL;
        if (tail->pos == pos || head->pos == pos) {
            if (tail->pos == pos) {
                toDelete = tail;
                if (head->pos != pos) {
                    (tail->next)->prev = NULL;
                    tail = toDelete->next;
                }
                else { tail = NULL; head = NULL; }
            }
            else {
                toDelete = head;
                (head->prev)->next = NULL;
                head = toDelete->prev;
            }
        }
        else {
            while (TAIL->pos <= HEAD->pos) {
                if (TAIL->pos == pos) break;
                TAIL = TAIL->next;
                if (HEAD->pos == pos) break;
                HEAD = HEAD->prev;
            }

            toDelete = (TAIL->pos != pos) ? ((HEAD->pos != pos) ? toDelete:HEAD):TAIL;
            if (toDelete != NULL) {
                (toDelete->prev)->next = toDelete->next;
                (toDelete->next)->prev = toDelete->prev;
            }
        }

    if (toDelete != NULL) free(toDelete);
}

void printALL() {
    Deque *TAIL = tail;
    #ifdef DEBUG
    while (TAIL != NULL) {
        printf ("|| PREV = %i || POS = %i || NEXT = %i||\n", ((TAIL->prev != NULL) ? (TAIL->prev)->pos:0) , TAIL->pos, ((TAIL->next != NULL) ? (TAIL->next)->pos:9999));
        TAIL = TAIL->next;
    }
    #endif
}

void blackJack(int);
void maiorMenor(int);
void parImpar(int);
extern inline void socketSend(int,int,char*);
extern inline struct message socketReceive(int,int*);
void * connectionHandler(void*); /* tratador de conexão */
void init ();
void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    struct  hostent  *ptrh;  /* pointer to a host table entry       */
    struct  protoent *ptrp;  /* pointer to a protocol table entry   */
    struct  sockaddr_in sad; /* structure to hold server's address  */
    struct  sockaddr_in cad; /* structure to hold client's address  */
    int     cnt=1,port,n,ret,optval=1,sd,sd2,alen=0;
    char    buf[1000];

    memset((char *)&sad, 0, sizeof(sad)); /* clear sockaddr structure */
    sad.sin_family = AF_INET;             /* set family to Internet   */
    sad.sin_addr.s_addr = INADDR_ANY;     /* set the local IP address */

    port = (argc > 1) ? atoi(argv[1]):PROTOPORT;   /* Seleciona a porta   */
    if (port > 0) sad.sin_port = htons((u_short)port); /* testa se a porta é valida */
    else error("Bad port number\n");

    if ( ((int)(ptrp = getprotobyname("tcp"))) == 0) error("cannot map \"tcp\" to protocol number"); /* Map TCP transport protocol name to protocol number */

    sd = socket(AF_INET, SOCK_STREAM, 0 /*ptrp->p_proto*/);  /* Create a TCP socket */
    if (sd < 0) error("Socket creation failed\n");

    if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval , sizeof(int)) < 0) /* Eliminate "Address already in use" error from bind. */
        return -1;

    if (bind(sd, (struct sockaddr *)&sad, sizeof(sad)) < 0) error("Bind failed\n"); /* Bind a local address to the socket */
    if (listen(sd, QLEN) < 0) error("Listen failed\n"); /* Specify size of request queue */

    struct connection *CON;
    CON = calloc(NUM_THREADS, sizeof(struct connection));
    /* Main server loop - accept and handle requests */

    // inits
    printALL();
    do {
        if ((sd2 = accept(sd, (struct sockaddr *)&cad, &alen)) < 0) { perror("Accept failed\n"); cnt--; }
        else {
        CON[cnt].thread_id = cnt;
        CON[cnt].sd = sd;
        CON[cnt].sd2 = sd2;
        CON[cnt].sock = cnt;
        ret = pthread_create( &CON[cnt].thread_id, NULL, connectionHandler, &CON[cnt]);
        }
    } while (cnt++);
    pthread_exit(NULL);
}

//void connectionHandler (int sd, int sd2, int sockID) {
void * connectionHandler (void *args) {
    struct connection *CON = args;
    int queueID = 0, playerID, matchID, gameID, n, sd = CON->sd, sd2 = CON->sd2, sockID = CON->sock;
    char jogada[20];
    printf ("after thr = sd = %i, sd2 = %i, sock = %i\n", sd,sd2,sockID);

    while (msg = socketReceive(sd2,&n), n) {
        switch (msg.code) {
            case 0:
                    printf ("Player %d Connected\n",sockID);
                    socketSend(sd2,msg.code,"Voce Conseguiu se Conectar Com o Servidor, podera jogar em multiplayer se quiser");
            break;
            case 1:
            case 2:
            case 3:
                    #ifdef DEBUG
                        printf ("Player %d Solicita Status de queue de jogo\n", sockID);
                    #endif
                    if (Queue[msg.code]) socketSend(sd2,msg.code, "Conectarse ao Jogador esperando");
                    else socketSend(sd2,msg.code, "Esperar por jogador");
            break;
            case 7:
            case 8:
            case 9:
                    #ifdef DEBUG
                        printf ("Player %d entrou na sala do jogo %i\n", sockID, msg.code%6);
                    #endif
                    if (Queue[msg.code%6]) {
                        #ifdef DEBUG
                            printf ("Player %d iniciou partida com Player %i\n", sockID, Queue[msg.code%6]);
                        #endif
                        playerID = 2;
                        matchID = (++gameIdOnList);
                        gameID = msg.code%6;
                        Queue[gameID] = -matchID;
                        goto FOUNDMATCH;
                    }
                    else {
                        queueID = msg.code%6;
                        gameID = queueID;
                        Queue[queueID] = sockID;
                        socketSend(sd2,inqueue,"Aguardando outro jogador");
                    }
            break;

            case GAMEMaiorMenor:
                MAIORMENORLABEL:
                if ((listAT(matchID)->GamesOn).MaMe.turno) {
                    if ((listAT(matchID)->GamesOn).MaMe.turno == playerID) {
                        #ifdef DEBUG
                            printf ("Game MAIOR MENOR TURN of player %i\n", sockID);
                        #endif
                        socketSend(sd2,GAMEMaiorMenor,(sprintf(jogada,"%i",(listAT(matchID)->GamesOn).MaMe.numChosen),jogada));
                        msg = socketReceive(sd2,&n);
                        int chosen = atoi(msg.str);
                        if (chosen == ((listAT(matchID)->GamesOn).MaMe.number)) {
                            #ifdef DEBUG
                                printf ("Player %d acertou %i\n", sockID, msg.code%6);
                            #endif
                            (listAT(matchID)->GamesOn).MaMe.turno = 0;
                            socketSend(sd2,winstat,"");
                            goto ENDMATCH;
                        }
                        else
                            (listAT(matchID)->GamesOn).MaMe.numChosen = chosen;
                        (listAT(matchID)->GamesOn).MaMe.turno = switchTurn((listAT(matchID)->GamesOn).MaMe.turno);
                    }
                    else sleep(2);
                goto MAIORMENORLABEL;
                }
                socketSend(sd2,endmatch,"");
                goto ENDMATCHCLOSE;
            break;

            case GAMEParOuImpar:
                PAROUIMPARLABEL:
                if ((listAT(matchID)->GamesOn).ParImpar.turno) {
                    if ((listAT(matchID)->GamesOn).ParImpar.turno == playerID) {
                        #ifdef DEBUG
                            printf ("Game Par Impar TURN of player %i\n", sockID);
                        #endif
                        if (playerID == 1)
                            if ((listAT(matchID)->GamesOn).ParImpar.p1Opt == 'n') {
                                socketSend(sd2,GAMEParOuImpar,(sprintf(jogada,"%i",playerID),jogada));
                                msg = socketReceive(sd2,&n);
                                (listAT(matchID)->GamesOn).ParImpar.p1Opt = (msg.str[0]);
                            } else {
                                socketSend(sd2,GAMEParOuImpar,(sprintf(jogada,"%i",playerID),jogada));
                                msg = socketReceive(sd2,&n);
                                (listAT(matchID)->GamesOn).ParImpar.p1 = atoi(msg.str);

                                (listAT(matchID)->GamesOn).ParImpar.turno = 0;
                                socketSend(sd2,endmatch,(sprintf(jogada,"%i",((listAT(matchID)->GamesOn).ParImpar.p1+ (listAT(matchID)->GamesOn).ParImpar.p2)),jogada));
                                goto ENDMATCH;
                            }
                        else {
                            socketSend(sd2,GAMEParOuImpar,((listAT(matchID)->GamesOn).ParImpar.p1Opt == 'p') ? "p":"i");
                            msg = socketReceive(sd2,&n);
                            (listAT(matchID)->GamesOn).ParImpar.p2 = atoi(msg.str);
                        }
                        (listAT(matchID)->GamesOn).ParImpar.turno = switchTurn((listAT(matchID)->GamesOn).ParImpar.turno);
                    }
                    else sleep(2);
                goto PAROUIMPARLABEL;
                }
                socketSend(sd2,endmatch,(sprintf(jogada,"%i",((listAT(matchID)->GamesOn).ParImpar.p1+ (listAT(matchID)->GamesOn).ParImpar.p2)),jogada));
                goto ENDMATCHCLOSE;
            break;

            case GAMEBlackJack:
                BLACKJACKLABEL:
                if ((listAT(matchID)->GamesOn).BlkJack.turno < 3) {
                    if ((listAT(matchID)->GamesOn).ParImpar.turno < playerID) {
                        #ifdef DEBUG
                            printf ("Game blackjack TURN of player %i\n", sockID);
                        #endif
                        socketSend(sd2,GAMEBlackJack,"");
                        msg = socketReceive(sd2,&n);
                        if (playerID ==1) (listAT(matchID)->GamesOn).BlkJack.sump1 = atoi(msg.str);
                        else (listAT(matchID)->GamesOn).BlkJack.sump2 = atoi(msg.str);
                        if ((listAT(matchID)->GamesOn).BlkJack.turno + playerID == 3) {
                            socketSend(sd2,endmatch,(sprintf(jogada,"%i",((listAT(matchID)->GamesOn).BlkJack.sump2 + (listAT(matchID)->GamesOn).BlkJack.sump1)),jogada));
                            (listAT(matchID)->GamesOn).BlkJack.turno += playerID;
                            goto ENDMATCH;
                        } else (listAT(matchID)->GamesOn).BlkJack.turno += playerID;
                    }
                    else sleep(2);
                goto BLACKJACKLABEL;
                }
                    socketSend(sd2,endmatch,(sprintf(jogada,"%i",((listAT(matchID)->GamesOn).BlkJack.sump2 + (listAT(matchID)->GamesOn).BlkJack.sump1)),jogada));
                goto ENDMATCHCLOSE;
            break;

            case inqueue:
                    if (Queue[queueID] == sockID)
                        socketSend(sd2,inqueue,"Esperando Adversario");
                    else {
                        playerID = 1;
                        matchID = -Queue[queueID];
                        Queue[queueID] = 0;
                        socketSend(sd2,foundmatch,"initializing match");
                    }
                    #ifdef DEBUG
                        printf ("Player %d esta na lista de espera\n", sockID);
                    #endif
            break;
            case leftqueue:
                    #ifdef DEBUG
                        printf ("Player %d saiu da lista de espera\n", sockID);
                    #endif
                    queueID = (queueID ? (Queue[queueID] = 0,0):0);
            break;
            case foundmatch:
                FOUNDMATCH:
                    #ifdef DEBUG
                        printf ("FOUND MATCH player %i\n", sockID);
                    #endif
                    switch (gameID) {
                        default: case 0: break;
                        case 1:
                              maiorMenor(matchID);
                        break;
                        case 2:
                              parImpar(matchID);
                        break;
                        case 3:
                              blackJack(matchID);
                        break;
                    }
                    printALL();
                    socketSend(sd2,foundmatch,"initializing match");
            break;
            case endmatch:
                ENDMATCHCLOSE:
                    removeGame(matchID);
                    printALL();
                ENDMATCH:
                    queueID = 0;
                    matchID = 0;
                    gameID = 0;
                    playerID = 0;
            break;
            default:  break;
        }
    }
    if (queueID) Queue[queueID] = 0;
    printf ("Player %d desconectou...\n", sockID);
    close (sd2);
}

void blackJack(int id) {
    union GamesStruct jogo;
    jogo.BlkJack.turno = 0;
    addGame(jogo, id);
    printf ("\nBlackJack comecou.\n");
}

void parImpar(int id) {
    union GamesStruct jogo;
    jogo.ParImpar.turno = 1;
    jogo.ParImpar.p1Opt = 'n';
    addGame(jogo, id);
    printf ("\nPar impar comecou.\n");
}

void maiorMenor(int id) {
    union GamesStruct jogo;
    srand(time(NULL));
    jogo.MaMe.number = ((rand() % 1000) +1);
    jogo.MaMe.numChosen = -jogo.MaMe.number;
    jogo.MaMe.turno = 1;
    addGame(jogo, id);
    printf ("\nUm numero entre 1 e %i foi sorteado.\n", 1000);
}
