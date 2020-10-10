/*
 * airport_server.x:
 */

program AIRPORT_SERVER_PROG {
  version AIRPORT_SERVER_VERS {
    airports_reply AIRPORT_QUERY(lat_lon) = 1;
  } = 1;
} = 0x27699174
