#include <math.h>
#include "Geo.h"
#include "Lib.h"
#define PI 3.14159265

static Greply MSG;

Greply * sin_1_svc(Ginput *argp, struct svc_req *rqstp) {
	MSG.status = OKSTATUS;
	MSG.r = sin (argp->a*PI/180);
	return (&MSG);
}

Greply * cossin_1_svc(Ginput *argp, struct svc_req *rqstp) {
	MSG.status = OKSTATUS;
	MSG.r = cos (argp->a*PI/180);
	return (&MSG);
}

Greply * tang_1_svc(Ginput *argp, struct svc_req *rqstp) {
	MSG.status = OKSTATUS;
	MSG.r = tan (argp->a*PI/180);
	return (&MSG);
}



