%#include "place_airport_common.h"

program AIRPORTS_PROG {
  version AIRPORTS_VERS {
    airports_ret AIRPORTS_QRY(location) = 2;
  } = 1;
} = 0x37699174;
