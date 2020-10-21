/*******************************************************************************
 *   File: common.h
 * Author: Connor Wilding
 *   Desc:
 ******************************************************************************/
#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "place_airport_common.h"
#include "../build/src/place_airport_common.h"

#ifndef NDEBUG
#define log_printf(fmt, ...) \
        do { fprintf(stderr, "[%s()]: " fmt "\n", \
                     __func__, __VA_ARGS__); } while (0)
#else
#define log_printf(X, ...)
#endif

// Case-insensitive comparison
#if defined(_WIN32) || defined(_WIN64)
  #define strcasecmp _stricmp
  #define strncasecmp _strnicmp
#endif

// Error handling
void exitWithMessage(const char *msg);
void exitWithMessage(const std::string &msg);

/**
 * \struct CityRecord
 * \brief City record.
 *
 * City record.
 */
 struct CityRecord {
   std::string cityName;
   std::string state;
   location    loc{};
   
   CityRecord();
   
   CityRecord(const std::string &cityName_,
              const std::string &state_,
              const location &loc_);
 };

/**
 * \struct AirportRecord
 * \brief An Airport record.
 * An Airport record.
 */
 struct AirportRecord {
   location loc{};        ///< \var Location in lat / long
   std::string code;    ///< \var Airport 3-digit code
   std::string name;    ///< \var Full airport name.
   std::string state;   ///< \var Airport state
   
   /**
    * \brief Construct an airport record.
    * \param location Location in lat / long
    * \param acode 3-digit airport code
    * \param aname Full airport name
    * \param astate State airport is located in
    */
    explicit AirportRecord(location location,
                           const std::string& acode,
                           const std::string& aname,
                           const std::string& astate);
 };

/**
 * \struct DistAirport
 * \brief Airport record query result
 */
 struct DistAirport {
   const AirportRecord *airport;    ///< \var Airport result (by value since objects are small)
   double dist;                     ///< \var Distance from query target in statute miles
   
   /**
    * \brief Construct query result
    * \param airRef Result airport
    * \param adist Distance to target in statute miles
    */
    DistAirport(const AirportRecord &airRef, double adist);
    
   /**
    * \brief Order the query result by distance
    * \param other Other query result
    * \return True when this object is closer
    */
    bool operator<(const DistAirport &other) const;
 };
 
std::ostream &operator<<(std::ostream &strm, const location &loc);
std::ostream &operator<<(std::ostream &strm, const place &pl);
std::ostream &operator<<(std::ostream &strm, const places_ret &plRet);
std::ostream &operator<<(std::ostream &strm, const CityRecord &rec);
std::ostream &operator<<(std::ostream &strm, const DistAirport &rec);
std::ostream &operator<<(std::ostream &strm, const airport &airp);
std::ostream &operator<<(std::ostream &strm, const airports_ret &airRet);

template<typename TElem>
std::ostream& operator<<(std::ostream &strm, const std::vector<TElem>& elems) {
  for (size_t i = 0; i < elems.size(); ++i) {
    strm << i + 1 << ". " << elems[i] << std::endl;
  }
  return strm;
}
