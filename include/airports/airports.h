/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _AIRPORTS_H_RPCGEN
#define _AIRPORTS_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif

#include "place_airport_common.h"

#define AIRPORTS_PROG 0x37699175
#define AIRPORTS_VERS 2

#if defined(__STDC__) || defined(__cplusplus)
#define AIRPORTS_QRY 2
extern  airports_ret * airports_qry_1(location *, CLIENT *);
extern  airports_ret * airports_qry_1_svc(location *, struct svc_req *);
extern int airports_prog_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define AIRPORTS_QRY 2
extern  airports_ret * airports_qry_1();
extern  airports_ret * airports_qry_1_svc();
extern int airports_prog_1_freeresult ();
#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_AIRPORTS_H_RPCGEN */
