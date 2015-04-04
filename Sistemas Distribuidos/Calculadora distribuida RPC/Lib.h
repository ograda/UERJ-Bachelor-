#include <iostream>

#define OKSTATUS 0
#define ERRORSTATUS_ESTOURO 1
#define ERRORSTATUS_DIVZERO 2
#define ERRORSTATUS_INFINITE 3
#define ERRORSTATUS_FATORIALNEGATIVO 4
#define ERRORSTATUS_NEPERIANOZERO 5
#define MODULO(a) ( (a) > 0 ) ? (a):(-a)
#define LL_LIM (0x7fffffffffffffff)
#define ULL_LIM (0xffffffffffffffff)
#define DBL_MAX 1.7976931348623158e+308


using namespace std;

string ERRORS[6] = {"","O valor Calculado eh maior que o suportado", "Divisao por 0 resulta em indeterminacao", "O resultado que voce esta buscando tende ou eh igual a infinito por um dos lados", "Nao existem fatoriais negativos","nao ln de zero"};
