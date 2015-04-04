#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <math.h>
#include <set>
#include <time.h>

using namespace std;
string Cartas[13] = {"2","3","4","5","6","7","8","9","10","J","Q","K", "A"};

int pontos(int a[5]) {
    int cardset[13], naipe = floor(a[0]/13.0), seq=0, maior = 0, sz=1;
    int par=0, trio=0, quadra=0,i,j;
    bool Same = true, Seq = true;
    memset(cardset,0,sizeof(cardset));

    for (i=0; i<5; i++) {
        if (floor(a[i]/13.0)!=naipe) Same=false;
        cardset[(a[i]%13)]++;
    }

    for (i=0; i<13; i++)
       if(cardset[i]){
            if (cardset[i]==2) par++;
            if (cardset[i]==3) trio++;
            if (cardset[i]==4) quadra++;

            if (cardset[i] >= sz) {
                maior = i;
                sz = cardset[i];
            }

            if(seq) {
                if ((i-seq) == 1) seq = i;
                else Seq = false;
            }
            else seq = i;
        }

        if (quadra) return (700+maior); // 4 o a kind
        if (trio)
            if (par) return (600+maior); // full house
            else return (300+maior); // 3 of a kind
        if (par)
            if (par==2) return (200+maior); // double 2 of a kind
            else return (100+maior); // 2 of a kind
        if (Seq)
            if (Same) {
                if (maior==12) return (900+maior); // royal straight flush
                else return (800+maior); // straight flush
            }
            else return (400+maior); // sequencia
       if (Same) return (500+maior); // Flush

    return maior; // nenhuma combinacao
}

void combName(int p) {
    int a = floor(p/100.0);
    switch(a) {
        case 0: cout << " nao tem nenhuma combinacao na sua mao, maior carta "; break;
        case 1: cout << " possui um par em sua mao de "; break;
        case 2: cout << " possui dois pares em sua mao, o maior de "; break;
        case 3: cout << " possui um trio sua mao de "; break;
        case 4: cout << " possui uma sequencia em sua mao, a maior carta e "; break;
        case 5: cout << " possui um FLUSH em sua mao, a maior carta e "; break;
        case 6: cout << " possui um FULL HOUSE em sua mao, a maior carta e "; break;
        case 7: cout << " possui uma QUADRA em sua mao de "; break;
        case 8: cout << " possui um SRAIGHT FLUSH em sua mao!"; break;
        case 9: cout << " possui um ROAYL SRAIGHT FLUSH em sua mao, WOW!"; break;
    }
    if (p < 800) cout << Cartas[(p-a*100)] << "!"<< endl;
    else cout << endl;
}

char CARD(int a) {
    switch(a) {
        case 0: return 'C'; break;
        case 1: return 'P'; break;
        case 2: return 'O'; break;
        case 3: return 'E'; break;
    }
}

void Result(int p1, int p2, int A[5], int B[5], bool comp) {
    int i;
    cout << "RESULTADO: " << endl << endl;
    cout << "Seu Resultado: " << endl;
        for (i=0; i<5; i++)
            cout << "|" << CARD(floor(A[i]/13.0)) << " " << Cartas[A[i]%13] << "| ";
    cout << endl << "Voce "; combName(p1);

    cout << "Adversario: " << endl;
        for (i=0; i<5; i++)
            cout << "|" << CARD(floor(B[i]/13.0)) << " " << Cartas[B[i]%13] << "| ";
    cout << endl << "O " << ((comp) ? "Computador ":" outro Jogador "); combName(p2);

    cout << endl << ((p1>p2)?"Voce foi o vencedor, sua combinacao e melhor que a do adversario!":"Voce foi o perdedor, sua combinacao e pior que a do adversario!") << endl;
}

set <int> pilha;
int cards[5];
int DrawCards(bool single,char *game) {
    set<int>::iterator it;
    char * buf;
    int id = 0;
    if (single) {
        srand(time(NULL));
        while (pilha.size()<16)
        pilha.insert((rand()%52));
        return 1;
    } else {
        buf = strtok (game,",");
        id = atoi(buf);
        while (buf = strtok (NULL, ","), buf != NULL)
            pilha.insert(atoi(buf));
    }

    return id;
}

string poker(bool single, char *game) {
    string ret;
    char buf[5];
    set<int>::iterator it;

    int i=0, j=0, p1S,p2S, t, c;
    int P1[8], P2[8], ESC1[5], ESC2[5];

    pilha.clear();
    int id = DrawCards(single,game);

    system ("clear");
        for (it=pilha.begin(); it!=pilha.end(); ++it, i++)
            if (i%2) P1[j++] = (*it);
            else P2[j] = (*it);

        for (i=0; i<5; i++) ESC1[i] = P1[i];
        for (i=0; i<5; i++) ESC2[i] = P2[i];

        cout << "Voce Possui: " << endl;
        for (i=0; i<5; i++)
            if (id==1) cout << "|" << CARD(floor(P1[i]/13.0)) << " " << Cartas[P1[i]%13] << "| ";
            else cout << "|" << CARD(floor(P2[i]/13.0)) << " " << Cartas[P2[i]%13] << "| ";
        cout << endl;

        cout << "Quantas cartas voce gostaria de trocar (0-3) ? " << endl;
        cin >> t;
        t = min(max(t,0),3);

        while (t) {
            cout << "Digite uma carta para trocar (1-5) ? " << endl;
            cin >> c;
            c = min(max((c-1),0),4);
            if (id==1) P1[c] = P1[8-t];
            else P2[c] = P2[8-t];
            t--;
        }

        if (id==1) for (i=0; i<5; i++) cards[i] = P1[i];
        else for (i=0; i<5; i++) cards[i] = P2[i];

        ret.clear();
        p1S = pontos(P1);
        p2S = pontos(P2);
        if (single) {
            Result(p1S,p2S,P1,P2,true);
            getchar();
            getchar();
        } else {
            if (id==1) sprintf(buf,"%d",p1S);
            else sprintf(buf,"%d",p2S);
            ret.append(buf);
            for (i=0; i<5; i++) {
                ret.append(",");
                if (id==1) sprintf(buf,"%d",P1[i]);
                else sprintf(buf,"%d",P2[i]);
                ret.append(buf);
            }
        }
  return ret;
}

void COMPARE(char *game) {
    int P,A,aCards[5], i=0;
    P = pontos(cards);
    char* buf;

    buf = strtok (game,",");
    A = atoi(buf);
    while (buf = strtok (NULL, ","), buf != NULL)
        aCards[i++] = atoi(buf);

    Result(P,A,cards,aCards,false);
}

int menu() {
    int op;
    system ("clear");
    cout << "P O K E R" << endl;
    cout << "1. Singleplayer" << endl;
    cout << "2. Multiplayer" << endl;
    cout << "3. Sair" << endl;
    cout << endl << "Escolha: ";
    cin >> op;
    return (op == 1 || op == 2) ? op:0;
}

int main(int argc, char * argv[]) {
    poker(true,"");
    exit(0);
}


