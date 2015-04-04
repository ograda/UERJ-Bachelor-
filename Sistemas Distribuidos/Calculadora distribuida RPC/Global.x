#define VERSION_NUMBER 1

struct Input {
	double a;
	double b;
};

struct Connection {
	char ip[15]
	bool geo;
	bool euc;
	bool esp;
	bool cie;
	int server;
};

struct Reply {
	int status;
	double r;
};

program GLOBAL_PROG {
   version GLOBAL_VERSION {
     Connection CONNECT(Connection) = 1;	
     
   } = VERSION_NUMBER;
} = 0xef4a30c7;

