#define MAX_LINE_LENGTH 256
#define MIN(X,Y) (X < Y ? X : Y)

// Registradores -- Nome / Referência em binário
struct {
	const char *nome;
	char *binario;
} regMap[] = {
		{ "$zero", "00000" },
		{ "$at", "00001" },
		{ "$v0", "00010" },
		{ "$v1", "00011" },
		{ "$a0", "00100" },
		{ "$a1", "00101" },
		{ "$a2", "00110" },
		{ "$a3", "00111" },
		{ "$t0", "01000" },
		{ "$t1", "01001" },
		{ "$t2", "01010" },
		{ "$t3", "01011" },
		{ "$t4", "01100" },
		{ "$t5", "01101" },
		{ "$t6", "01110" },
		{ "$t7", "01111" },
		{ "$s0", "10000" },
		{ "$s1", "10001" },
		{ "$s2", "10010" },
		{ "$s3", "10011" },
		{ "$s4", "10100" },
		{ "$s5", "10101" },
		{ "$s6", "10110" },
		{ "$s7", "10111" },
		{ "$t8", "11000" },
		{ "$t9", "11001" },
		{ NULL, 0 } };

// Instruções de tipo R -- Nome / Referência em binário
struct {
	const char *nome;
	char *binario;
	char *funct;
} rMap[] = {



		{ "add", "100000", "100000" },
		{ "sub", "100010", "100010" },
		{ "and", "100100", "100100" },
		{ "or",  "100101", "100101" },
		{ "sll", "000000", "000000" },
		{ "slt", "101010", "000010" },
		{ "srl", "000010", "101001" },
		{ "jr",  "001000", "001000" },
		{ NULL, 0, 0 } };

// Instruções de tipo I -- Nome / Referência em binário
struct {
	const char *nome;
	char *binario;
	char *tipo;
} iMap[] = {
		{ "lw",   "100011", "3"},
		{ "sw",   "101011", "3"},
		{ "addi", "001100", "1"},
		{ "bne", "001010", "2"},
		{ "beq",  "000100", "2"},
		{ NULL, 0, 0 } };

// Instruções de tipo J -- Nome / Referência em binário
struct {
	const char *nome;
	char *binario;
} jMap[] = {
        { "jal", "000011" },
		{ "j", "000010" },
		{ NULL, 0 } };

// Retorna a representação em binário do registrador
char *registerBin(char *regNome) {
    size_t i;
	for (i = 0; regMap[i].nome != NULL; i++) {
		if (strcmp(regNome, regMap[i].nome) == 0) {
			return regMap[i].binario;
		}
	}

	return NULL;
}

// Retorna a representação em binário do imediato
char *imediateBin(char *imedNome, int tam) {
int i,k;
char *RET;
RET = (char*)malloc(tam+1);
	for (i = tam; i >= 0; i--) {
        k = atoi(imedNome) >> i;
        *(RET+tam-i) = (k & 1) + '0';
	}
	*(RET+tam+1) = '\0';

	return RET;
}

int imediateInt(int bin) {
    int total  = 0;
    int potenc = 1;

    while(bin > 0) {
        total += bin % 10 * potenc;
        bin    = bin / 10;
        potenc = potenc * 2;
    }

    return total;
}

// remove estrução lida
void removeCom(char *com, int first) {
int i,j;
char tmp[MAX_LINE_LENGTH],*hdl;
    hdl = &*com;
    for (i=first; i< strlen(hdl); i++) {
        tmp[(i-first)] = hdl[i];
        hdl[(i-first)] = tmp[(i-first)];
    }
    for (j = (strlen(hdl)-first); j<strlen(hdl); j++) {
    hdl[j] =  '\0';
    }
}
