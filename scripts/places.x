/*
 * places.x: Remote place lookup protocol
 */

program PLACE_SERVER_PROG {
  version PLACE_SERVER_VERS {
    places_reply PLACES_QUERY(city_state) = 1;
  } = 1;
} = 0x37699174;
