/*******************************************************************************
 *   File: common.cpp
 * Author: Connor Wilding
 ******************************************************************************/
#include <iomanip>
#include <iostream>
#include <utility>
#include "common.h"

// Error handling
/******************************************************************************/
void exitWithMessage(const char *msg) {
  if (errno) perror(msg);
  else       std::cerr << msg << std::endl;
  exit(-1);
}

CityRecord::CityRecord() = default;

CityRecord::CityRecord(std::string cityName_, const std::string &state_,
                       const location &loc_) :
                       cityName(std::move(cityName_)), state(std::move(state_)),
                       loc(loc_) { }

AirportRecord::AirportRecord(location location, std::string acode,
                             std::string &aname, std::string &astate) :
                             loc(location), code(std::move(acode)),
                             name(std::move(aname)), state(std::move(astate)) { }
  
DistAirport::DistAirport(const AirportRecord &airRef, const double adist) :
  airport(&airRef) , dist(adist) {}

bool DistAirport::operator<(const DistAirport &other) const {
  return dist < other.dist;
}

std::ostream &operator<<(std::ostream &stream, const location &loc) {
  stream << ": " << loc.latitude << ", " << loc.longitude;
  return stream;
}

std::ostream &operator<<(std::ostream &stream, const DistAirport &rec) {
  stream << "distance="<< rec.dist << ", code=" << rec.airport;
  return stream;
}

std::ostream &operator<<(std::ostream &stream, const airport &airp) {
  stream << "distance=" << airp.dist << ", code=" << airp.code 
       << ", state=" << airp.state << ", name=" << airp.name;
  return stream;
}

std::ostream& listAirports(std::ostream &stream, const airport *airp) {
  for (int i = 0; i < NRESULTS; ++i)
    stream << airp[i] << std::endl;
  
  return stream;
}

std::ostream &operator<<(std::ostream &stream, const airports_ret &airRet) {
  if (airRet.err)
    stream << "Error: " << airRet.airports_ret_u.error_msg;
  else
    listAirports(stream, &airRet.airports_ret_u.results[0]);
  
  return stream;
}

std::ostream &operator<<(std::ostream &stream, const place &p1) {
  return stream << p1.name << ", " << p1.state << " " << p1.loc;
}

std::ostream &operator<<(std::ostream &stream, const places_ret &p1Ret) {
  if (p1Ret.err) {
    stream << "Error: " << p1Ret.places_ret_u.err_msg;
  } else {
    stream << p1Ret.places_ret_u.results.request << std::endl;
    listAirports(stream, &p1Ret.places_ret_u.results.results[0]);
  }
  
  return stream;
}

std::ostream &operator<<(std::ostream &stream, const CityRecord &rec) {
  stream << std::setw(25) << std::left << rec.cityName;
  stream << std::setw(0) << ", " << rec.state << " " << rec.loc;
  
  return stream;
}
