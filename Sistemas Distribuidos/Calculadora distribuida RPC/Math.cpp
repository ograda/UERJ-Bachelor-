#include <math.h>
#include "Math.h"
#include "Lib.h"
#define FATMAX 1000

static Mreply MSG;

int getError ( double x, double y, bool sqr) {
	if (sqr)
		if (y==0.0)
			return ERRORSTATUS_NEPERIANOZERO;

	if (x >= exp (y*log (DBL_MAX)))
		return ERRORSTATUS_ESTOURO;
	
	return OKSTATUS;
}

double factorial (long long n) {
	long long k = 1;
	if (n<0) return 0;
	
	do  k *= n;
	while (--n);
	return ((double)k);
}

Mreply * power_1_svc(Minput *argp, struct svc_req *rqstp) {
	MSG.status = getError ( argp->a, argp->b, false);
	if (MSG.status == OKSTATUS)
		MSG.r = pow (argp->a, argp->b);

	return (&MSG);
}

Mreply * root_1_svc(Minput *argp, struct svc_req *rqstp) {
	MSG.status = getError ( argp->a, argp->b, true);
	if (MSG.status == OKSTATUS)
		MSG.r = pow (argp->a, (1.0/argp->b));
	return (&MSG);
}

Mreply * factorial_1_svc(Minput *argp, struct svc_req *rqstp) {
	MSG.status = ((argp->a > FATMAX) ? ERRORSTATUS_ESTOURO:OKSTATUS);
	if (MSG.status == OKSTATUS)
		MSG.r = factorial ((long long) argp->a);
	if (!MSG.r)
		MSG.status = ERRORSTATUS_FATORIALNEGATIVO;
	return (&MSG);
}

