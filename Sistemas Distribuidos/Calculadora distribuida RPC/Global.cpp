#include <math.h>
#include "Geo.h"
#include "Math.h"
#include "SCient.h"
#include "Euclid.h"
#include "Global.h"
#include "Lib.h"

bool geometric = false, euclidian = false, cientific = false, especific = false, local = false;
CLIENT *geo_clnt , *euc_clnt , *cient_clnt , *espec_clnt;

bool connectGEO (char *ip) {
	geo_clnt = clnt_create(ip, GEOMETRIC_PROG, GEOMETRIC_VERSION, "udp");
	if (geo_clnt == (CLIENT *) NULL) {
		clnt_pcreateerror(ip);
		return false;
	}
	return true;
}

bool connectESP (char *ip) {
	espec_clnt = clnt_create(ip, ESPECIFIC_PROG, ESPECIFIC_VERSION, "udp");
	if (espec_clnt == (CLIENT *) NULL) {
		clnt_pcreateerror(ip);
		return false;
	}
	return true;
}

bool connectEUC (char *ip) {
	euc_clnt = clnt_create(ip, EUCLIDIAN_PROG, EUCLIDIAN_VERSION, "udp");
	if (euc_clnt == (CLIENT *) NULL) {
		clnt_pcreateerror(ip);
		return false;
	}
	return true;
}

bool connectCIE (char *ip) {
	cient_clnt = clnt_create(ip, CIENTIFIC_PROG, CIENTIFIC_VERSION, "udp");
	if (cient_clnt == (CLIENT *) NULL) {
		clnt_pcreateerror(ip);
		return false;
	}
	return true;
}

Connection * connect_1_svc(Connection *argp, struct svc_req *rqstp) {
	static connection c;
	switch(argp->server) {
        case 1: c.geo = connectGEO (argp->ip);
        break;
        case 2: c.euc = connectEUC (argp->ip);
        break;
        case 3: c.cie = connectCIE (argp->ip);
        break;
        case 4: espc = connectESP (argp->ip);
        break;
        default: return;
        break;
       }
	return (&c);
}


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
