#include <math.h>
#include "SCient.h"
#include "Lib.h"

static Creply MSG;

int getError ( double x, double y) {
	if (x >= exp (y*log (DBL_MAX)))
		return ERRORSTATUS_ESTOURO;
	
	return OKSTATUS;
}

Creply * log_1_svc(Cinput *argp, struct svc_req *rqstp) {
	MSG.status = (argp->b == 0.0) ?  ERRORSTATUS_NEPERIANOZERO:OKSTATUS;
	if (MSG.status == OKSTATUS)
		MSG.r = pow (argp->b, (1.0/argp->a));
	return (&MSG);
}

Creply * ln_1_svc(Cinput *argp, struct svc_req *rqstp) {
	MSG.status = getError ( argp->a, argp->b);
	if (MSG.status == OKSTATUS)
		MSG.r = exp (pow (argp->b, argp->a));
	return (&MSG);
}
