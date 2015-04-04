#define VERSION_NUMBER 1

struct Einput {
	double a;
	double b;
};

struct Ereply {
	int status;
	double r;
};

program EUCLIDIAN_PROG {
   version EUCLIDIAN_VERSION {
	Ereply MOD(Einput) = 1;
	Ereply MMC(Einput) = 2;
	Ereply MDC(Einput) = 3;
   } = VERSION_NUMBER;
} = 0xc349fd01;

