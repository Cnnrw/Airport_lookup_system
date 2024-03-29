/*******************************************************************************
 *   File: airports_server.cpp
 * Author: Ben Targan
 *   Desc: Airports Server
 ******************************************************************************/
#include <cstdio>
#include <cstdlib>
#include <rpc/pmap_clnt.h>
#include <cstring>
#include <netinet/in.h>

#include "airports/airports.h"
#include "airports/KDTree.h"
#include "place_airport_common.h"

#ifndef SIG_PF
#define SIG_PF void(*)(int)
#endif

static void airports_prog_1(struct svc_req *rqstp,  register SVCXPRT *transp)
{
	union {
		location airports_qry_1_arg;
	} argument{};
	char *result;
	xdrproc_t _xdr_argument, _xdr_result;
	char *(*local)(char *, struct svc_req *);

	switch (rqstp->rq_proc) {
    case NULLPROC:
      (void) svc_sendreply (transp, (xdrproc_t) xdr_void, (char *)NULL);
      return;
    case AIRPORTS_QRY:
      _xdr_argument = (xdrproc_t) xdr_location;
      _xdr_result = (xdrproc_t) xdr_airports_ret;
      local = (char *(*)(char *, struct svc_req *)) airports_qry_1_svc;
      break;
    default:
      svcerr_noproc (transp);
      return;
	}
	memset ((char *)&argument, 0, sizeof (argument));
	if (!svc_getargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		svcerr_decode (transp);
		return;
	}
	result = (*local)((char *)&argument, rqstp);
	if (result != NULL && !svc_sendreply(transp, (xdrproc_t) _xdr_result, result)) {
		svcerr_systemerr (transp);
	}
	
	if (!svc_freeargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		fprintf (stderr, "%s", "unable to free arguments");
		exit (1);
	}
}

/**
 * Query.
*/
airports_ret *airports_qry_1_svc(location *argp, struct svc_req *rqstp) {
  static airports_ret result;
  
  result = { };
  airport* const closest = kd5Closest(*argp);
  
  memcpy(&result.airports_ret_u.results[0], closest, sizeof(airports));
  
  return &result;
}

int main (int argc, char **argv) {
  const char* airportsPath = "airport-locations.txt";
  if (argc < 2)
    printf("Note: airports path not specified, using `airports-locations.txt`\n");
  else
    airportsPath = argv[1];
  
  initKD(airportsPath);
  
  register SVCXPRT *transp;

  pmap_unset (AIRPORTS_PROG, AIRPORTS_VERS);

  transp = svcudp_create(RPC_ANYSOCK);
  if (transp == NULL) {
    fprintf (stderr, "%s", "cannot create udp service.");
    exit(1);
  }
  if (!svc_register(transp, AIRPORTS_PROG, AIRPORTS_VERS,
                    airports_prog_1, IPPROTO_UDP)) {
    fprintf (stderr, "%s", "unable to register (AIRPORTS_PROG, AIRPORTS_VERS, udp).");
    exit(1);
  }

  transp = svctcp_create(RPC_ANYSOCK, 0, 0);
  if (transp == NULL) {
    fprintf (stderr, "%s", "cannot create tcp service.");
    exit(1);
  }
  if (!svc_register(transp, AIRPORTS_PROG, AIRPORTS_VERS,
                    airports_prog_1, IPPROTO_TCP)) {
    fprintf (stderr, "%s", "unable to register (AIRPORTS_PROG, AIRPORTS_VERS, tcp).");
    exit(1);
  }

  svc_run ();
  fprintf (stderr, "%s", "svc_run returned");
  exit (1);
}
