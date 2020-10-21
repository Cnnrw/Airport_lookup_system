/*******************************************************************************
 *   \file trie.h
 * \author Connor Wilding
 *   \desc Declarations for public API to build and query places trie.
 ******************************************************************************/
#pragma once
#include "common.h"

// Public interface functions
/******************************************************************************/

/** Found places collection with references to the indexed city entries */
using TFoundPlaces = std::vector<std::reference_wrapper<const CityRecord>>;

/**
 * \struct TrieQueryResult
 * \brief Result of a users query
 */
struct TrieQueryResult {
  TFoundPlaces places;      ///< Empty when not found, partially when ambiguous
  bool         isAmbiguous;         ///< Flag to indicate when query is ambiguous
};

/**
 * \brief Initializes the trie lookup data structure with given data.
 *        Throws on IO/file format error.
 * \param placesPath  Path to the places file to load.
 */
extern "C" void initTrie(const char* placesPath);

/**
 * \brief Performs an efficient prefix completion lookup using a Trie data
 *        structure. Uses state to filter ambiguous entries. Returns ref to
 *        stored records and flag if lookup was not found or is ambiguous.
 *
 * \param cityState   City name to lookup
 * \param state       State to use when ambiguous
 * \return A list of references to city records, and a flag if ambiguous
 */
TrieQueryResult queryPlace(const name_state &cityState);

// Trie class that is used to perform an efficient lookup
/******************************************************************************/

/** Type of collection of cities loaded from the places file */
using TPlaceRecs = std::unique_ptr<std::vector<CityRecord>>;

/**
 * \class Trie
 * \brief Trie data structure to hold place information
 */
class Trie {
  public:
    /**
     * \brief Initializes and builds an internal index of the city records
     * \param cityRecords Reference to the city records
     */
    explicit Trie(TPlaceRecs cityRecords);
    
    /**
     * \brief Performs a query on the trie, case-insensitive
     * \param cityName City name of the place
     * \return Trie traversal results
     */
    TrieQueryResult query(const std::string &cityName) const;
    
    /**
     * \brief Get the size of the underlying container.
     * \return Number of records including duplicates.
     */
    size_t size() const;
  
  private:
    struct TrieNode {
      const char            c;                             // Char associated with node
      std::pair<int, int>   idxRange{-1, -1};       // Range of entries that match
      std::vector<TrieNode> next{};                          // Sorted list of next nodes
      
      explicit TrieNode(char ch);
    };
    
    /**
     * \brief Helper to traverse the tree for the given query
     * \param cname City name
     * \param node Current node
     * \param depth Current subtree depth
     * \return Result of the query search
     */
    TrieQueryResult query(const std::string &cname, const TrieNode &node,
                          int depth) const;
    
    /**
     * \brief Attempts to traverse the remaining tree to find the first valid
     *        prefix completion. When shortest prefix search fails,
     *        returns an ambiguous set of results.
     *
     * \param node    Current subtree being searched
     * \return Result of the completion attempt
     */
    TrieQueryResult getFirstCompletion(const TrieNode &node) const;
    
    TrieQueryResult getAmbiguousHints(const TrieNode &node) const;
    
    /**
     * \brief Helper to construct a trie subtree from a given sub-range of records.
     *
     * \param begin   First index of the sub-range constructed
     * \param end     Last exclusive index of the sub-range constructed
     * \param depth   Current depth of the subtree
     * \param node    Current root node of the subtree
     */
    void construct(int begin, int end, int depth, TrieNode &node) const;
    
    /**
     * \brief Helper to find the sub-range where all chars at this range are the
     *        same.
     *
     * \param fm      Beginning index of the sub-range
     * \param to      End index of the sub-range (exclusive)
     * \param depth   Current depth of the subtree
     * \return First index where the character is different from the first index
     */
    int endOfSameLetterRange(int fm, int to, size_t depth) const;
    
    /**
     * \brief Place records owned by the trie. Freed it dtor.
     */
    TPlaceRecs places{};
    
    /**
     * \brief Root node of the Trie
     */
    TrieNode root;
};
