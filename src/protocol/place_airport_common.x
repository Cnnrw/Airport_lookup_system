/*******************************************************************************
 *    File: place_airport_stubs.x
 * Authors: Ben Targan, Connor Wilding
 *    Date: 10/10/20
 *    Desc:
 ******************************************************************************/

/******************************************************************************
 * Places Data Structures
 ******************************************************************************/
struct location {
  double    latitude;
  double    longitude;
};

struct place {
  string     name<MAX_NAME>;
  string     state<MAX_STATE>;
  location   loc;
};

/******************************************************************************
 * Airports data structures
 ******************************************************************************/

struct airport {
  location  loc;
  double    dist;
  string    code<MAX_AIRCODE>;
  string    name<MAX_NAME>;
  string    state<MAX_STATE>;
};

typedef airport airports[NRESULTS];

/******************************************************************************
 * Request & reply data structures
 ******************************************************************************/

/* City + state based request from the client */
struct name_state {
  string    name<MAX_NAME>;
  string    state<MAX_STATE>;
};

/* Union representing the type of client request - by name or by lat / long */
union places_req switch (int req_type) {
  case 0:
    name_state named;
  case 1:
    location loc;
  default:
    void;
};

/* Reply data to client with a name record and closest airports */
struct place_airports {
  place     request;
  airports  results;
};

/* Reply from places to client with an optional errno */
union places_ret switch(int err) {
  case 0:
    place_airports results;
  default:
    string err_msg<MAX_ERRMSG>;
};

/* Reply from airports to places with an optional errno */
union airports_ret switch (int err) {
  case 0:
    airports results;
  default:
    string error_msg<MAX_ERRMSG>;
};
