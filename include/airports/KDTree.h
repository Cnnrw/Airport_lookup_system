/*******************************************************************************
 *   File: airportsKdTree.h
 * Author: Connor Wilding
 *   Desc: Public API to build and lookup KD-Tree.
 ******************************************************************************/
#pragma once
#include <memory>
#include "common.h"

/** Type of collection of airports loaded from the airports file */
using TAirportRecs = std::unique_ptr<std::vector<AirportRecord>>;

// Public interface methods to init and search
/******************************************************************************/

/**
 * \brief Initializes the kd tree data structure with given data.
 * Throws on IO/file format error.
 * \param airportsPath Path to the airports file to load
 */
extern "C" void initKD(const char* airportsPath);

/**
 * \brief Performs a KNN lookup to get 5 closest airports.
 * Returns pointer to static data structure that does not need to be freed.
 * \param target      Latitude / longitude of target location to perform search
 * \return Ptr to a static arr of 5 elems that does not need to be freed.
 */
airport* kd5Closest(location target);

/**
 * \struct KDNode
 * \brief A node in the KD tree.
 */
struct KDNode {
  AirportRecord           airport;                ///< \var airport Airport entry
  std::unique_ptr<KDNode> left;         ///< \var left Left KD subtree
  std::unique_ptr<KDNode> right;        ///< \var right Right KD subtree
  
  /**
   * \brief Constructs a KD-Tree node.
   * \param pt Airport record
   * \param lft Left subtree
   * \param rgt Right subtree
   */
	KDNode(AirportRecord pt, std::unique_ptr<KDNode> lft,
        std::unique_ptr<KDNode> rgt);
};

/**
 * \class AirportsKDTree
 * \brief Airports KD-Tree that allows for a closet locations query
 */
class KDTree {
  public:
    /**
     * \brief Takes ownership of air records and constructs a KD-Tree.
     * \param airRecs Airport records to take ownership of.
     */
    explicit KDTree(TAirportRecs airRecs);
    
    /**
     * \brief Collects k closest locations to the target
     * \param target Target location to collect closest to
     * \param k Number of closest collections to collect
     * \return Closest k locations
     */
    std::vector<DistAirport>
    kClosestLocations(location target, size_t k = 5) const;
    
    /**
     * \brief Get number of airport records loaded into the kd tree.
     * \return Number of airport records in the tree.
     */
    size_t size() const;
  
  private:
    TAirportRecs airports;          ///< Airports loaded from file
    std::unique_ptr<KDNode> root;   ///< Root node of the KD tree.
};