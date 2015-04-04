#include "library.h"

//tags para desvios na compilação
#define DEBUG // debuga os resultados e preve movimentos do computador
#define NONCONNECTION // não mata o programa se não conseguir conexão, ao contrario, desabilita o multiplay.
#define waitTIME 15 // tempo de longa espera
#define waitSpace 1 // tempo de sleep em espera

struct message msg;
bool multiPlayer = true;
char buf[1000],jogada[20];
int n;

void error(const char *msg, bool erroCritico) {
    perror(msg);
    if (erroCritico)
        exit(1);
    else {
        #ifdef NONCONNECTION
            multiPlayer = false;
            perror("Because of this error your multiplayer fuctions gone out.\n");
        #else
                exit(1);
        #endif
    }
}

extern inline void socketSend(int,int,char*);
extern inline struct message socketReceive(int, int*);
extern inline int getOption();
extern inline int ifloor();
extern inline int getch();

bool lookMatch(struct message msg, int sd) {
int hdl;
    if (msg.code != foundmatch) {
        printf ("Status: Esperando Adversario\n");
        while (true) {
            for (hdl = 0; hdl < waitTIME && msg.code != foundmatch; hdl+=waitSpace) {
                socketSend(sd,inqueue,"");
                msg = socketReceive(sd,&n);
                sleep(waitSpace);
            }
            if (msg.code != foundmatch) {
                printf ("O adversario demorar, continuar esperando ('S' para continuar) ? ");
                scanf("%s", buf);
                if ( buf[0] == 's' || buf[0] == 'S') socketSend(sd,inqueue,"");
                else {
                    socketSend(sd,leftqueue,"");
                    return true;
                }
                clear();
                printf ("\nStatus: %s", msg.str);
            }
            else {
                sleep(waitSpace);
                break;
            }
        }
    socketSend(sd,leftqueue,"");
    }
    return false;
}

// BlackJAck AI em 3 niveis
void blackJack(int sd, int dif) {
    clear();
    char gameNames[13][3] = {"A","2","3","4","5","6","7","8","9","10","J","Q","K"};
    int gameValue[13] = {10,2,3,4,5,6,7,8,9,10,10,10,10};
    char continuar = 's';
    srand(time(NULL));
    // A dificuldade influe em quando o computador ira dar "stand", no facil ele so aceita stand se fizer blackjack, medio com 19, e dificil 17.
    if (dif) {
        int cards[19], i , j, fused = 1, used=1, stand = (dif==1) ? 21:((dif==2) ? 19:17), as = 0, pcPoints=0, total = 0,points=0;
        bool royal= false;
            for (i=0;i<=19;i++) {
                cards[i] = (rand() % 52);
                for (j = (i-1); j>0; j--) if (cards[i] == cards[j]) { i--; break; }
            }
            #ifdef DEBUG
            printf ("\nDEBUG: Pilha de cartas: ");
                for (i=0;i<=19;i++) printf ("%i ", cards[i]);
            #endif
        // jogada do computador até stand;
        for (i=0;i<=10;i++) {
            j = gameValue[cards[i]%13];
            total += j;
            if (j != cards[i]%13)
                if (cards[i]%13) royal = true;
                else as++;
            pcPoints = blackJackPoints(total, as, royal);
            if (pcPoints >= stand) break;
        } fused += i;
        total = 0; as = 0; royal = false;
        printf ("===== BlackJack =====\n\nVoce possui as cartas: ");
            for (i=fused;i<=fused+used;i++) {
                printf ("%s ", gameNames[cards[i]%13]);
                j = gameValue[cards[i]%13];
                total += j;
                if (j != cards[i]%13)
                if (cards[i]%13) royal = true;
                else as++;
            } points = blackJackPoints(total, as, royal);

        while (points <= 21 && ++used) {
            printf ("\nGostaria de parar ('p') ou contiuar ('*') ?");
            continuar = getCharOption();
            if (continuar == 'p') break;
            printf ("\nVoce tirou um: %s", gameNames[cards[used]%13]);
            j = gameValue[cards[used]%13];
            total += j;
            if (j != cards[i]%13)
            if (cards[i]%13) royal = true;
            else as++;
            points = blackJackPoints(total, as, royal);
        }
        if (points <= 21) {
            if (pcPoints > 21)  printf ("\nParabens, voce foi o vencedor! (Pontos %i vs %i PontosPC)\n", points, pcPoints);
            else
                if (pcPoints >= points)
                    printf ("\nQue pena, você perdeu para o computador nivel %i! (Pontos %i vs %i PontosPC) ps: empate eh da casa\n", dif, points, pcPoints);
                else
                    printf ("\nParabens, voce foi o vencedor! (Pontos %i vs %i PontosPC)\n", points, pcPoints);
        }
        else
            printf ("\nQue pena, você perdeu para o computador nivel %i! sua soma passou de 21! (%i)\n", dif, points);
        getch(); getch();
    } // Jogador escolheu fazer conexao ao servidor e jogar em multiplayer -- testa se esta conectado ao servidor ( bool multiPlayer)
    else  {
        socketSend(sd,9,"entering");
        msg = socketReceive(sd,&n);
        bool killed = lookMatch(msg,sd);
        if (!killed) {
            int cards[10], i , j, used=1, as = 0, total = 0,points=0;
            bool royal= false;
            for (i=0;i<=10;i++) {
                cards[i] = (rand() % 52);
                for (j = (i-1); j>0; j--) if (cards[i] == cards[j]) { i--; break; }
            }
            #ifdef DEBUG
            printf ("\nDEBUG: Pilha de cartas: ");
                for (i=0;i<=10;i++) printf ("%i ", cards[i]);
            #endif

        socketSend(sd,GAMEBlackJack,"");
        msg = socketReceive(sd,&n); // initializing game

        printf ("===== BlackJack =====\n\nVoce possui as cartas: ");
        for (i=0;i<=used;i++) {
            printf ("%s ", gameNames[cards[i]%13]);
            j = gameValue[cards[i]%13];
            total += j;
            if (j != cards[i]%13)
            if (cards[i]%13) royal = true;
            else as++;
        } points = blackJackPoints(total, as, royal);

        while (points <= 21 && ++used) {
            printf ("\nGostaria de parar ('p') ou contiuar ('*') ?");
            continuar = getCharOption();
            if (continuar == 'p') break;
            printf ("\nVoce tirou um: %s", gameNames[cards[used]%13]);
            j = gameValue[cards[used]%13];
            total += j;
            if (j != cards[i]%13)
            if (cards[i]%13) royal = true;
            else as++;
            points = blackJackPoints(total, as, royal);
        }

        socketSend(sd,GAMEBlackJack,(sprintf(jogada,"%i",points),jogada));
        msg = socketReceive(sd,&n); // initializing game
        total = atoi(msg.str);
        total -= points;
            if (points <= 21) {
                if (total == points)  printf ("\nO jogo acabou empatado! (Voce %i vs %i Adversario)\n", points, total);
                else
                    if (total > points && total <= 21)
                        printf ("\nQue pena, você perdeu! (Voce %i vs %i Adversario)\n", points, total);
                    else
                        printf ("\nParabens, voce foi o vencedor! (Voce %i vs %i Adversario)\n", points, total);
            }
            else printf ("\nQue pena, você perdeu! sua soma passou de 21! (%i)\n", points);
            getch(); getch();
        }
    }
}

// GAME PAR ou IMPAR AI comum
void parImpar(int sd, int dif) {
    clear();
    char parOuImpar = 'n';
    if (dif) {
        srand(time(NULL));
        int numChosen = -1, comp = (rand() % 11);
        printf ("===== Par ou Impar =====\n");
            #ifdef DEBUG
                printf ("\nDEBUG: o computador colocara %i.", comp);
            #endif
        while (parOuImpar != 'p' && parOuImpar != 'i') {
            printf ("\nVoce e o jogador 1, gostaria de escolher PAR ('p') ou IMPAR ('i') ?");
            parOuImpar = getCharOption();
        }

        printf ("\nQuantos dedos quer levantar ? ");
        numChosen = getOption(); numChosen =  min(numChosen,10); numChosen = max(numChosen,0);

        if ((numChosen + comp) % 2) {
            if (parOuImpar == 'i') printf ("\nParabens, voce foi o vencedor! (soma %i)\n", (numChosen + comp));
            else printf ("\nQue pena, voce perdeu para o computador nivel %i! (soma %i)\n", dif, (numChosen + comp));
        } else {
            if (parOuImpar == 'p') printf ("\nParabens, voce foi o vencedor! (soma %i)\n", (numChosen + comp));
            else printf ("\nQue pena, voce perdeu para o computador nivel %i! (soma %i)\n", dif, (numChosen + comp));
        }
        getch(); getch();
    }
    else  {
        socketSend(sd,8,"entering");
        msg = socketReceive(sd,&n);
        bool killed = lookMatch(msg,sd);
        if (!killed) {
            bool par = true;
            printf ("===== Par ou Impar =====\nAguardando seu turno...\n\n");
            socketSend(sd,GAMEParOuImpar,"");
            msg = socketReceive(sd,&n); // initializing game
            int numChose = atoi(msg.str);
            if (numChose == 1) {
                while (parOuImpar != 'p' && parOuImpar != 'i') {
                    printf ("\nVoce e o jogador 1, gostaria de escolher PAR ('p') ou IMPAR ('i') ?");
                    parOuImpar = getCharOption();
                } socketSend(sd,GAMEParOuImpar, (parOuImpar == 'p') ?"p":"i");
                par = (parOuImpar == 'p') ? true:false;
                msg = socketReceive(sd,&n);
            }
            else {
                par = (msg.str[0]=='p') ? false:true;
                printf ("\nO jogador 1, escolheu %s, voce e %s",(par)?"impar":"par", (par)?"par":"impar");
            }

            printf ("\nQuantos dedos quer levantar ? ");
            numChose = getOption();
            numChose =  min(numChose,10); numChose = max(numChose,0);
            socketSend(sd,GAMEParOuImpar,(char*) (sprintf(jogada,"%i",numChose),jogada));

            msg = socketReceive(sd,&n);
            numChose = atoi(msg.str);
            printf ("\nA soma dos dedos foi %i.\n", numChose);
            if ((!(numChose%2) && par) || ((numChose%2) && !par))
                printf ("Parabens, voce foi o vencedor!\n");
            else
                printf ("Infelizmente voce perdeu!\n");
            getch(); getch();
        }
    }
}

// GAME MAIOR MENOR AI em 3 niveis
void maiorMenor(int sd, int dif) {
    clear();
    // Jogador escolheu jogar contra o computador, Jogo Maior Menor auto - dificuldades 1 (pick up aleatorio 50 numeros), 2 (pick up aleatorio entre a faixa de numeros ainda existentes 100 numeros), 3 ( escolhe o numero medio entre maior numero menor e menor numero maior - resposta otima em 10 passos)
    if (dif) {
        srand(time(NULL));
        int numChosen = -1, menor = 0, factor = ((dif==1) ? 50:((dif==2) ? 100:1000)), maior = factor + 1, number = ((rand() % factor) +1), turno = 2;
        printf ("===== Maior Menor =====\nUm numero entre 1 e %i foi sorteado.\n", factor);
        do {
            printf ("\n=======================\n");
            #ifdef DEBUG
                printf ("\nDEBUG: o numero %i foi sorteado.", number);
            #endif
            if (turno%2) { // turnos pares do computador
                numChosen = ((dif == 3) ? ifloor((maior+menor)/2.0) : (1 + ((dif == 1) ? rand()%factor : (rand()%(maior-menor) + menor))));
                printf ("\nO Computador escolheu: %i.", numChosen);
            }
            else { // turnos impares do jogador
                printf ("\nescolha um numero: ");
                numChosen = getOption();
            }

            if (numChosen != number)
                if (numChosen < number) {
                    printf ("\nO numero eh MENOR que o sorteado. (1-%i)\n", factor);
                    menor = max(menor, numChosen);
                }
                else {
                    printf ("\nO numero eh MAIOR que o sorteado. (1-%i)\n", factor);
                    maior = min(maior, numChosen);
                }
            else
                break;
        } while (turno++);
        printf ("\n=======================\n");
        if (turno%2) printf ("\nQue pena, voce perdeu para o computador nivel %i! (numero %i)\n", dif, number);
        else printf ("\nParabens, voce foi o vencedor! (numero %i)\n", number);
        getch();
    }
    else  {
        socketSend(sd,7,"entering");
        msg = socketReceive(sd,&n);
        bool killed = lookMatch(msg,sd);
        if (!killed) {
            int number = 0;
            printf ("===== Maior Menor =====\nUm numero entre 1 e 1000 foi sorteado.\nAguardando seu turno...\n\n");
            socketSend(sd,GAMEMaiorMenor,"");
            while(msg = socketReceive(sd,&n), n && msg.code == GAMEMaiorMenor) { // initializing game
                int numChose = atoi(msg.str);
                if (!number) {
                    number = -numChose;
                    #ifdef DEBUG
                        printf ("\nDEBUG: o numero %i foi sorteado.", number);
                    #endif
                }
                else {
                    if (numChose >0)
                        if (numChose < number) printf ("O adversario escolheu %i (MENOR que o sorteado)\n", numChose);
                        else printf ("O adversario escolheu %i (MAIOR que o sorteado)\n", numChose);
                    printf ("\nescolha um numero entre 1 e 1000: ");
                    numChose = getOption();
                }
            socketSend(sd,GAMEMaiorMenor,(char*) (sprintf(jogada,"%i",numChose),jogada));
            }
            printf ("\nO numero sorteado foi %i.\n", number);
            if (msg.code == winstat)
                printf ("Parabens, voce foi o vencedor!\n");
            else
                printf ("Infelizmente voce perdeu!\n");
            getch(); getch();
        }
    }
}

// distribui o jogador no servidor de acordo com a escolha de jogo que ele fez.
void gameHandler(int sd, int game) {
    int opt = -1;
    int dificuldade = -1;
    TIPODEJOGO:
        clear();
        printf ("Jogo escolhido %s (digite 0 para retornar)\n\n", gameNames[game]);
        if (multiPlayer == true) {
            socketSend(sd,game,"entering");
            msg = socketReceive(sd,&n);
            printf ("status de multijogador: %s", msg.str);
        } else printf ("status de multijogador: desabilitado");
        printf ("\nDeseja Jogar sozinho (1) ou Multijogador (2) : ");
        opt = getOption();
        if (opt <= 2 && opt >=0) {
            switch (opt) {
                case 1 : // singleplayer
                        DIFICULDADES:
                        printf ("\n\nDificuldades- facil (1) medio (2) dificil (3), qual desejas: ");
                        dificuldade = getOption();
                        if (dificuldade <= 3 && dificuldade >=0) {
                            if (dificuldade) {
                                switch (game) {
                                    case 1: maiorMenor(sd,dificuldade); break;
                                    case 2: parImpar(sd,dificuldade); break;
                                    case 3: blackJack(sd,dificuldade); break;
                                    default: break;
                                }
                            }
                        }
                        else
                            goto DIFICULDADES;
                break;
                case 2: // multiplayer
                    if (multiPlayer == true)
                        switch (game) {
                            case 1:
                                maiorMenor(sd,0);
                            break;
                            case 2:
                                parImpar(sd,0);
                            break;
                            case 3:
                                blackJack(sd,0);
                            break;
                            default:
                            break;
                        }
                    else
                        printf ("\n voce nao esta conectado ao servidor.");
                break;
                default: break;
            }
        }
        else
            goto TIPODEJOGO;
}

// menu de escolhas dos jogos disponivels para o jogador jogar
void mainLoop(int sd) {
    int opt = -1;
    GAMEMENU:
        clear();
        printf ("\n=== Jogos Disponíveis ===\n\n");
        printf ("4 - Jogo da Velha(nao impementado ainda)\n3 - BlackJack\n2 - Par ou Impar\n1 - Maior Menor\n0 - Sair\n");
        printf ("\n=========================\n\nQual Jogo Deseja Jogar: ");
        opt = getOption();
        if (opt < games && opt >= 0) {
            if (opt) {
                gameHandler(sd,opt);
                goto GAMEMENU;
            }
        }
        else
            goto GAMEMENU;
}

int main(int argc, char * argv[]) {
        struct  hostent  *ptrh;  /* pointer to a host table entry       */
        struct  protoent *ptrp;  /* pointer to a protocol table entry   */
        struct  sockaddr_in sad; /* structure to hold an IP address     */
        int     sd,port;
        char    *host, *text;

#ifdef WIN32
        WSADATA wsaData;
        WSAStartup(0x0101, &wsaData);
#endif

        memset((char *)&sad,0,sizeof(sad));
        sad.sin_family = AF_INET;
        if (argc > 2) port = atoi(argv[2]);
        else port = PROTOPORT;

        if (port > 0)  sad.sin_port = htons((u_short)port);
        else error("Bad port number\n",false); /* print error message and exit */

        if (argc > 1) host = argv[1]; /* if host argument specified   */
        else host = localhost;

        ptrh = gethostbyname(host);
        if ( ((char *)ptrh) == NULL ) error("Invalid host name\n",false);
        memcpy(&sad.sin_addr, ptrh->h_addr, ptrh->h_length);

        if ( ((int)(ptrp = getprotobyname("tcp"))) == 0) error("Cannot map \"tcp\" to protocol number\n",false);
        sd = socket(AF_INET, SOCK_STREAM, ptrp->p_proto);
        if (sd < 0) error("Socket creation failed\n",false);
        if (connect(sd, (struct sockaddr *)&sad, sizeof(sad)) < 0) error("Connect failed\n",false);

        if (multiPlayer == true) {
            socketSend(sd,0,"entering");
            msg = socketReceive(sd,&n);
            printf ("%s\n", msg.str);
        }
        getchar();
        mainLoop(sd);
        closesocket(sd);
        exit(0);
}
