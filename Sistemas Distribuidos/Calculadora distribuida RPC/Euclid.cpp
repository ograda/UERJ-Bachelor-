#include <math.h>
#include "Euclid.h"
#include "Lib.h"

static Ereply MSG;

double gcd ( long long a, long long b ) {
  long long c;
  while ( a != 0 ) {
     c = a; a = b%a;  b = c;
  }
  return ((double) b);
}

long long fgcd(long long a, long long b) {
    for (;;) {
        if (a == 0) return b;
        b %= a;
        if (b == 0) return a;
        a %= b;
    }
}

double lcm(long long a, long long b) {
    int t = fgcd(a, b);

    return ((double) (t ? (a / t * b) : 0));
}

Ereply * mod_1_svc(Einput *argp, struct svc_req *rqstp) {
	MSG.status = OKSTATUS;
	MSG.r = ((double) ( ((long long) argp->b) % ((long long)argp->a) ) );
	return (&MSG);
}

Ereply * mmc_1_svc(Einput *argp, struct svc_req *rqstp) {
	MSG.status = OKSTATUS;
	MSG.r = lcm ( ((long long) argp->b), ((long long) argp->a) );
	return (&MSG);
}

Ereply * mdc_1_svc(Einput *argp, struct svc_req *rqstp) {
	MSG.status = OKSTATUS;
	MSG.r = gcd ( ((long long) argp->b), ((long long) argp->a) );
	return (&MSG);
}
