/*******************************************************************************
 *   File: places_airports_clnt.c
 * Author: Connor Wilding
 *   Desc:
 ******************************************************************************/
#include <memory.h> /* for memset */
#include "airports/airports.h"
#include "places/places.h"
#include "place_airport_common.h"

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 5, 0 };

places_ret *
places_qry_1(places_req *argp, CLIENT *clnt)
{
  static places_ret clnt_res;
  
  memset((char *)&clnt_res, 0, sizeof(clnt_res));
  if (clnt_call (clnt, PLACES_QRY,
                 (xdrproc_t) xdr_places_req, (caddr_t) argp,
                 (xdrproc_t) xdr_places_ret, (caddr_t) &clnt_res,
                 TIMEOUT) != RPC_SUCCESS) {
    return (NULL);
  }
  return (&clnt_res);
}

airports_ret *
airports_qry_1(location *argp, CLIENT *clnt)
{
  static airports_ret clnt_res;
  
  memset((char *)&clnt_res, 0, sizeof(clnt_res));
  if (clnt_call (clnt, AIRPORTS_QRY,
                 (xdrproc_t) xdr_location, (caddr_t) argp,
                 (xdrproc_t) xdr_airports_ret, (caddr_t) &clnt_res,
                 TIMEOUT) != RPC_SUCCESS) {
    return (NULL);
  }
  return (&clnt_res);
}