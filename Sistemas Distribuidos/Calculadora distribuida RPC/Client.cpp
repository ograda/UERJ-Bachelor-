#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include "Lib.h"

#include "Global.h"

#define clear() system("clear");
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define WHITE   "\033[37m"
#define localhost "127.0.0.1"

bool geometric = false, euclidian = false, cientific = false, especific = false, local = false;
CLIENT *clnt;

// erros em add/sub
int getErrorADDSUB (double x, double y) {
    unsigned long long param;
	x = MODULO(x);
	y = MODULO(y);
	param = (((unsigned long long) x) + ((unsigned long long) y));
	if (param > LL_LIM)
		return ERRORSTATUS_ESTOURO;
	return OKSTATUS;
}
// erros em multiplicacao
int getErrorTMS (double x, double y) {
	x = MODULO(x);
	y = MODULO(y);
	if ((LL_LIM / ((unsigned long long) x)) < ((unsigned long long) y))
		return ERRORSTATUS_ESTOURO;
	return OKSTATUS;
}
//erros em divisao
int getErrorDIV (double x, double y) {
	if (y == 0)
		return ERRORSTATUS_DIVZERO;
	return OKSTATUS;
}


bool isNumber(const string& s) {
    string::const_iterator it = s.begin();
    bool firstdot = true;
    while (it != s.end() && (isdigit(*it) || (firstdot && *it == '.' && it != s.begin()))) {
        if (firstdot && *it == '.') firstdot = false;
        ++it;
    } return (!s.empty() && it == s.end());
}

char *GeoOperations[3] = {"sin","cos","tg"};
int isGeometric(const string& s) {
     int i;
     for (i=0; i < sizeof(GeoOperations)/sizeof(char*); i++)
        if (!strcmp(s.c_str(),GeoOperations[i])) break;
    return (12+i);
}


char *Operations[12] = {"+","-","*","/","pow","root","!","mod", "mmc", "mdc","log","ln"};
int getOperation(string& s) {
   int i;
   for (i=0; i < sizeof(Operations)/sizeof(char*); i++)
        if (!strcmp(s.c_str(),Operations[i])) break;
    return (i);
}

void SendOperation(double A, double B, int op) {
    char cr;
    Reply *R;
    Input I;
    double result = 0;
    int error = 0;
   /* switch(op) {
        case 0:
            cout << A << " + " << B;
            error = getErrorADDSUB (A, B);
            if (!error) result = A+B;
        break;
        case 1:
            cout << A << " - " << B;
            error = getErrorADDSUB (A, B);
            if (!error) result = A-B;
        break;
        case 2:
            cout << A << " * " << B;
            error = getErrorTMS (A, B);
            if (!error) result = A*B;
        break;
        case 3:
            cout << A << " / " << B;
            error = getErrorDIV (A, B);
            if (!error) result = A/B;
        break;
        case 4:
            cout << A << " pow " << B;
            matIn.a = A;
            matIn.b = B;
            matR = (power_1(&matIn,espec_clnt));
            error = matR->status;
            if (!error) result = matR->r;
        break;
        case 5:
            cout << A << " root " << B;
            matIn.a = A;
            matIn.b = B;
            matR = (root_1(&matIn,espec_clnt));
            error = matR->status;
            if (!error) result = matR->r;
        break;
        case 6:
            cout << A << " !";
            matIn.a = A;
            matR = (factorial_1(&matIn,espec_clnt));
            error = matR->status;
            if (!error) result = matR->r;
        break;
        case 7:
            cout << A << " mod " << B;
            eucIn.a = A;
            eucIn.b = B;
            eucR = (mod_1(&eucIn,euc_clnt));
            error = eucR->status;
            if (!error) result = eucR->r;
        break;
        case 8:
            cout << A << " mmc " << B;
            eucIn.a = A;
            eucIn.b = B;
            eucR = (mmc_1(&eucIn,euc_clnt));
            error = eucR->status;
            if (!error) result = eucR->r;
        break;
        case 9:
            cout << A << " mdc " << B;
            eucIn.a = A;
            eucIn.b = B;
            eucR = (mdc_1(&eucIn,euc_clnt));
            error = eucR->status;
            if (!error) result = eucR->r;
        break;

        case 10:
            cout << A << " log " << B;
            cieIn.a = A;
            cieIn.b = B;
            cieR = (log_1(&cieIn,cient_clnt));
            error = cieR->status;
            if (!error) result = cieR->r;
        break;
        case 11:
            cout << A << " ln " << B;
            cieIn.a = A;
            cieIn.b = B;
            cieR = (ln_1(&cieIn,cient_clnt));
            error = cieR->status;
            if (!error) result = cieR->r;
        break;

        // geometric
        case 12:
            cout << "sin " << A;
            geoIn.a = A;
            geoR = (sin_1(&geoIn,geo_clnt));
            error = geoR->status;
            if (!error) result = geoR->r;
        break;
        case 13:
            cout << "cos " << A;
            geoIn.a = A;
            geoR = (cossin_1(&geoIn,geo_clnt));
            error = geoR->status;
            if (!error) result = geoR->r;
        break;
        case 14:
            cout << "tg " << A;
            geoIn.a = A;
            geoR = (tang_1(&geoIn,geo_clnt));
            error = geoR->status;
            if (!error) result = geoR->r;
        break;
        default: return;
        break;
    } */

    std::ostringstream str; str << result;
    cout << " = " << ((error) ? ERRORS[error]:str.str()) << endl;
   // cin.get(cr);
}

// menu de informacoes para os clientes
void mainMenu() {
    clear();
    cout << endl << "=== Disponibilidade de Servidores ===" << endl;
    cout << "Geometric - " << ((geometric) ? GREEN:RED) << ((geometric) ? "online":"offline") << RESET << endl;
    cout << "Euclidian - " << ((euclidian) ? GREEN:RED) << ((euclidian) ? "online":"offline") << RESET << endl;
    cout << "Cientific - " << ((cientific) ? GREEN:RED) << ((cientific) ? "online":"offline") << RESET << endl;
    cout << "Específic - " << ((especific) ? GREEN:RED) << ((especific) ? "online":"offline") << RESET << endl;
    cout << "=====================================" << endl;
    cout << "'sair', 'ajuda' e 'conectar' sao comandos especiais" << endl;
    cout << "=====================================" << endl << endl;
}

void Help() {
    char cr;
    clear();
    cout << "=====================================" << endl;
    cout << endl << "* O Usuario digita as operacoes da que deseja realizar de forma literal." << endl;
    cout << endl << "* As operacoes disponiveis estao divididas entre o cliente e quatro servidores." << endl;
    cout << endl << "* Estes Servidores possuem as seguintes operacoes:" << endl << endl;
    cout << "~Local:" << endl;
    cout << "Soma: A + B" << endl;
    cout << "Subtracao: A - B" << endl;
    cout << "Multiplicacao: A * B" << endl;
    cout << "Divisao: A / B" << endl << endl;

    cout << "~Especifica:" << endl;
    cout << "Potenciacao: A pow B" << endl;
    cout << "Radiciacao: A root B" << endl;
    cout << "Fatorial: A !" << endl << endl;

    cout << "~Geometrico:" << endl;
    cout << "Seno: sin A" << endl;
    cout << "Cosseno: cos A" << endl;
    cout << "Tangente: tg A" << endl << endl;

    cout << "~Euclidiano:" << endl;
    cout << "MOD: A mod B" << endl;
    cout << "MDC: A mdc B" << endl;
    cout << "MMC: A mmc B" << endl << endl;

    cout << "~Cientifico:" << endl;
    cout << "Logaritmo: A log B (log A na base B)" << endl;
    cout << "Logaritmo Natural: A ln B" << endl << endl;

    cout << "=====================================" << endl;
    cout << RED << "(*) os comandos sao case sensitive, todos separados por espaco!" << RESET <<endl;
    cout << "=====================================" << endl << endl;
    //cin.get(cr);
}

// menu de informacoes para os clientes
bool MakeConnect(int id, char *ip) {
 Connection Cin, *Cout;
    Cin.server = id;
    strcpy(Cin.ip,ip);
    switch(Cin.server) {
        case 1:
            if (geometric) return;
	    Cout = connect_1(&Cin,clnt));
            return ((geometric = Cout->geo),geometric);
        break;
        case 2:
            if (euclidian) return;
	    Cout = connect_1(&Cin,clnt));
            return ((euclidian = Cout->euc,euclidian);
        break;
        case 3:
            if (cientific) return;
	    Cout = connect_1(&Cin,clnt));
            return ((cientific = Cout->cie,cientific);
        break;
        case 4:
            if (especific) return;
	    Cout = connect_1(&Cin,clnt));
            return ((especific = Cout->esp,especific);
        break;
        default:
            return false;
        break;
    }
}

// menu de informacoes para os clientes
void Connect() {
    clear();
   char* ipad;
int server;
    cout << endl << "=== Escolha na lista de servidores desconectados ===" << endl;
    if (!geometric) cout << "1 - Geometric" << endl;
    if (!euclidian) cout << "2 - Euclidian" << endl;
    if (!cientific) cout << "3 - Cientific" << endl;
    if (!especific) cout << "4 - Específic" << endl;
    cout << "=====================================" << endl;
    cout << "com qual servidor gostaria de tentar conexão (numero) ? " ;
    cin >> server;
cout << "Qual eh o ip do servidor ? " ;
        cin >> ipad;
    MakeConnect(server,ipad);
}

int main(int argc, char *argv[]) {
    char cr;
    string input;
    double A,B;
    char * ladd;
    int operation;
   strcpy(ladd,localhost);
    if (argc > 2) {
        fprintf(stderr,"Servidor Global: ./%s GlobalHostAddres\n",argv[0]);
	fprintf(stderr,"Servidores Distribuidos: use a guia conect se os os servidores usarem mais de um ip.",argv[0]);
	exit(0);
    }


	clnt = clnt_create((argc==2)?argv[1]:ladd, GLOBAL_PROG, GLOBAL_VERSION, "udp");
	if (clnt == (CLIENT *) NULL) {
		clnt_pcreateerror((argc==2)?argv[1]:ladd);
		return false;
	}

    geometric = MakeConnect(1, ( (argc==2)?argv[1]:ladd ) );
    especific = MakeConnect(4, ( (argc==2)?argv[1]:ladd ) );
    cientific = MakeConnect(3, ( (argc==2)?argv[1]:ladd ) );
    euclidian = MakeConnect(2, ( (argc==2)?argv[1]:ladd ) );
    
    do {
        mainMenu();
        cout << "~Operacao: ";
        cin >> input; // first number or data comands

        if (!strcmp(input.c_str(),"sair") || !strcmp(input.c_str(),"SAIR") || !strcmp(input.c_str(),"Sair")) break; // user wants to exit
        if (!strcmp(input.c_str(),"ajuda") || !strcmp(input.c_str(),"AJUDA") || !strcmp(input.c_str(),"Ajuda")) { // user needs the help window
            Help();
            continue;
        }
        if (!strcmp(input.c_str(),"conectar") || !strcmp(input.c_str(),"CONECTAR") || !strcmp(input.c_str(),"Conectar")) { // user wants to connect into a server
            Connect();
            continue;
        }

        // check if it was entered a valid number or an geometric operation
        if (!isNumber(input)) {
            if (operation = isGeometric(input), operation != 15) {
                cout << "Geometric '" << input << "''" << endl;
                if (cin >> input, !isNumber(input)) { // check if A is numeric
                    cout << "A nao eh numerico!" << endl;
                    continue;
                }
                A = atof(input.c_str());
            }
            else cout << "A ou B nao sao numericos, ou a operacao geometrica '" << input << "' nao foi encontrada!" << endl;
            SendOperation(A, B, operation);
            continue;
        }
        A = atof(input.c_str());

        cin >> input; // get operation and check if its valid
        if (operation = getOperation(input), operation != 12) {
            if (operation != 6) { // check if its not factorial
                if (cin >> input, !isNumber(input)) { // check if B is numeric
                    cout << "A ou B nao sao numericos!" << endl;
                    continue;
                }
                B = atof(input.c_str());
            }
            SendOperation(A, B, operation);
        }
        else {
            cout << "A operacao '" << input << "' nao foi encontrada!" << endl;
            continue;
        }
        
    cout << "Pressione qualquer tecla para iniciar... ";
    cin.get(cr);
    }  while(getline(cin, input));

    return 0;
}

