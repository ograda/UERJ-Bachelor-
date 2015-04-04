#define VERSION_NUMBER 1

struct Cinput {
	double a;
	double b;
};

struct Creply {
	int status;
	double r;
};

program CIENTIFIC_PROG {
   version CIENTIFIC_VERSION {
     Creply LOG(Cinput) = 1;
     Creply LN(Cinput) = 2;
   } = VERSION_NUMBER;
} = 0xeb413d20;

