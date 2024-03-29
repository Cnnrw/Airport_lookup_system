/*******************************************************************************
 *   File: places_client.cpp
 * Author: Connor Wilding
 *   Desc: Places server client
 *
 ******************************************************************************/
#include <iostream>
#include <unistd.h>
#include "common.h"
#include "airports/airports.h"
#include "places/places.h"

const char *programUsage[] = {
  "Usage: To search by name with optional state when result is ambiguous",
  "       client <places-host> <city> [state]",
  "",
  "       Use -p flag to search by latitude / longitude:",
  R"(     client -p <places-host> "<latitude>" "<longitude>")",
};

// Exit the program, showing usage.
void showUsageAndExit();

// Helper to parse user's arguments into host and request object given.
void parseArgs(int argc, char **argv, char **host, places_req &req);

int main(int argc, char *argv[])
{
  char *host = nullptr;   // Host of the places server
  places_req req{};       // Request to the places server
  
  // Parse arguments and build a request to send
  parseArgs(argc, argv, &host, req);
  
  // Create a clinet handle
  CLIENT *clnt = clnt_create(host, PLACES_PROG, PLACES_VERS, "udp");
  if (clnt == NULL) {
    clnt_pcreateerror(host);
    exit(1);
  }
  
  // Query the places server
  places_ret *placesResult = places_qry_1(&req, clnt);
  if (placesResult == nullptr) {
    clnt_perror(clnt, "call failed");
    clnt_destroy(clnt);
    exit(1);
  }
  
  // Display result
  std::cout << *placesResult << std::endl;
  
  // Free resouces
  clnt_freeres(clnt, (xdrproc_t)xdr_places_ret, (caddr_t)(placesResult));
  clnt_destroy(clnt);
  
  exit(1);
}

void showUsageAndExit() {
  for (const char *line : programUsage)
    std::cerr << line << std::endl;
  exit(1);
}

void parseArgs(int argc, char **argv, char **host, places_req &req) {
  bool isLatLongQuery = false;
  
  int c;
  
  while((c = getopt(argc, argv, "p")) != -1) {
    switch (c) {
      case 'p':
        isLatLongQuery = true;
        break;
      case '?':
        if (isprint(optopt))
          std::cerr << "Unknown option '-" << (char)optopt << "'.\n";
        else
          std::cerr << "Unknown option char '" << std::hex << optopt << "'.\n";
        showUsageAndExit();
        break;
      default:
        abort();
    }
  }
  
  argc -= optind;
  argv += optind;
  
  if (argc < 2 || 3 < argc ||
    (isLatLongQuery && argc !=3)) {
    showUsageAndExit();
  }
  
  req.req_type = isLatLongQuery ? REQ_LAT_LONG : REQ_NAMED;
  
  *host = argv[0];
  
  if (!isLatLongQuery) {
    req.places_req_u.named.name = argv[1];
    req.places_req_u.named.state = (argc == 3) ? argv[2] : (char*)"";
  } else {
    double latitude;
    double longitude;
    try {
      latitude = std::stod(argv[1]);
      longitude = std::stod(argv[2]);
    }
    catch (...) {
      std::cerr << "Invalid latitude / longitude argument." << std::endl;
      exit(1);
    }
    req.places_req_u.loc.latitude = latitude;
    req.places_req_u.loc.longitude = longitude;
  }
}