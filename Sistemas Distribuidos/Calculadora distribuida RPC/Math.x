#define VERSION_NUMBER 1

struct Minput {
	double a;
	double b;
};

struct Mreply {
	int status;
	double r;
};

program ESPECIFIC_PROG {
   version ESPECIFIC_VERSION {
     Mreply POWER(Minput) = 1;
     Mreply ROOT(Minput) = 2;
     Mreply FACTORIAL(Minput) = 3;
   } = VERSION_NUMBER;
} = 0xfd4f3C20;

