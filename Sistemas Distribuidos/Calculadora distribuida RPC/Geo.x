#define VERSION_NUMBER 1

struct Ginput {
	double a;
};

struct Greply {
	int status;
	double r;
};

program GEOMETRIC_PROG {
   version GEOMETRIC_VERSION {
     Greply SIN(Ginput) = 1;
     Greply COSSIN(Ginput) = 2;
     Greply TANG(Ginput) = 3;
   } = VERSION_NUMBER;
} = 0xff4f3020;

