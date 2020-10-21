/*******************************************************************************
 *   \file Trie.cpp
 * \author Connor Wilding
 *   \desc Trie build and lookup implementation
 ******************************************************************************/
 
#include <places/trie.h>
#include <cctype>
#include <algorithm>
#include <fstream>
#include <strings.h>

// Forward declarations for helper functions
/******************************************************************************/

// Trims whitespace at end
inline std::string& trimRight(std::string& s);

// Removes and erases last word in string. Returns the word.
inline std::string removeLastWord(std::string& s);

// Constructs a city record from places file line. Throws on err.
CityRecord cityRecordFromLine(const std::string &line);

// Loads place records from a file. Throws on IO / parsing / mem errors.
TPlaceRecs loadPlacesFromFile(const char *fname, const size_t approxCount);

// Implementation of public interface methods to init and search
/******************************************************************************/

// State shared between two public api methods
static std::unique_ptr<Trie> trie;

void initTrie(const char *placesPath) {
  log_printf("Loading from file: %s.", placesPath);
  try {
    TPlaceRecs places = loadPlacesFromFile(placesPath, 20000);
    
    trie = std::unique_ptr<Trie>(new Trie(std::move(places)));
  }
  catch (const std::exception &e) {
    exitWithMessage(e.what());
  }
  
  log_printf("Loaded %d places.", (int)trie->size());
}

TrieQueryResult queryPlace(const name_state &cityState) {
  const std::string city = cityState.name;
  const std::string state = cityState.state;
  
  // Get set of cities with same name or ambiguous result
  auto result = trie->query(city);
  
  // Done when found an exact match, or city name is
  if (result.places.size() == 1 || result.isAmbiguous)
    return result;
  
  // When user didn't give us state, but same city is in diff states -> ambig
  if (state.empty()) {
    result.isAmbiguous = true;
    return result;
  }
  
  // Filter the results more, looking for exact ST match (erase-remove idom)
  TFoundPlaces &p1 = result.places;
  p1.erase(std::remove_if(
             p1.begin(), p1.end(),
             [=](const std::reference_wrapper<const CityRecord> &e) {
               return strcasecmp(e.get().state.c_str(),
                                 state.c_str()) != 0;
     
    }), p1.end());
  
  return result;
}

// Helper implementations
/******************************************************************************/

inline std::string& trimRight(std::string& s) {
  const std::string::iterator trimEnd =
    std::find_if_not(s.rbegin(), s.rend(), [](const unsigned char &c) {
      return std::isspace(c);
    }).base();
  s.erase(trimEnd, s.end());
  return s;
}

inline std::string removeLastWord(std::string& s) {
  trimRight(s);
  const std::string::iterator fstSpace =
    std::find_if(s.rbegin(), s.rend(), [](const char &c) {
      return std::isspace(c);
    }).base();
  std::string word(fstSpace, s.end());
  s.erase(fstSpace, s.end());
  trimRight(s);
  return word;
}

CityRecord cityRecordFromLine(const std::string &line) {
  if (!(164 < line.size() || line.size() < 165)) {
    throw std::invalid_argument(
      "Places file is mangled, each line needs to be 164 chars"
    );
  }
  
  std::string name = line.substr(9, 64);  // Name - 9 to 73  len = 64
  const std::string lastWord = removeLastWord(name);
  if (lastWord == "CDP") return {};
  
  name.shrink_to_fit();
  
  return CityRecord{
    std::move(name),
    line.substr(0, 2),            // State: 0 to 2  len = 2
    {
      std::stod(line.substr(143, 10)),  // Lat: 143 to 153 len = 10
      std::stod(line.substr(153, 10))  // Long: 153 to 164 len = 10
    }
  };
}

TPlaceRecs loadPlacesFromFile(const char *fname, const size_t approxCount) {
  std::ifstream placesFile(fname);
  
  if (!placesFile) {
    throw std::invalid_argument(
      "Unable to open " + std::string(fname) + " for reading.");
  }
  
  auto places = std::unique_ptr<std::vector<CityRecord>>(
    new std::vector<CityRecord>());
  auto &p1 = *places;
  p1.reserve(approxCount);
  
  std::string line;
  while (std::getline(placesFile, line)) {
    p1.emplace_back(cityRecordFromLine(line));
    if (p1.back().cityName.empty()) p1.pop_back();
  }
  p1.shrink_to_fit();
  
  std::sort(p1.begin(), p1.end(),
            [](const CityRecord &a, const CityRecord &b) {
              int cmp = strcasecmp(a.cityName.c_str(), b.cityName.c_str());
                return cmp == 0
                  ? strcasecmp(a.state.c_str(), b.state.c_str()) < 0
                  : cmp < 0;
            });
  
  return places;
}

// Implementation of the Trie members
/******************************************************************************/
Trie::Trie(TPlaceRecs cityRecords) :
  places(std::move(cityRecords)),
  root(0) {
  construct(0, (int)places->size(), 0, root);
}

TrieQueryResult
Trie::query(const std::string &cityName) const {
  return query(cityName, root, 0);
}

size_t Trie::size() const { return places->size(); }

Trie::TrieNode::TrieNode(const char ch) : c(ch) { }

TrieQueryResult Trie::query(const std::string &cname,
                            const TrieNode &node,
                            const int depth) const {
  // Base case, search string exhausted
  if ((int)cname.size() == depth) return getFirstCompletion(node);
  
  // Binary search on the next node to see if next char is in trie
  const char c = (char)std::tolower(cname[depth]);
  const auto it = std::lower_bound(node.next.begin(), node.next.end(), c,
                                   [](const TrieNode &tn, const char ch) {
                                     return tn.c < ch;
                                   });
  
  // Return empty result when not found
  if (it == node.next.cend() || c != it->c)
    return TrieQueryResult{TFoundPlaces(places->cend(), places->cend()), false };
  
  // Continue searching at next depth
  return query(cname, *it, depth + 1);
}

TrieQueryResult Trie::getFirstCompletion(const TrieNode &node) const {
  // Return the range of records stored in this node when nonempty
  if (node.idxRange.first != -1)
    return TrieQueryResult{
      TFoundPlaces(places->cbegin() + node.idxRange.first,
                 places->cbegin() + node.idxRange.second),
                 false
    };
  
  // Return not found when last node (shouldn't happen if constructed right)
  if (node.next.empty())
    return TrieQueryResult{TFoundPlaces(places->cend(), places->cend()), false};
  
  // Return the empty sentinel when this is the last node or is ambiguous
  if (node.next.size() > 1)
    return getAmbiguousHints(node);
  
  // Continue searching the rest of the trie chain
  return getFirstCompletion(node.next[0]);
}

TrieQueryResult Trie::getAmbiguousHints(const TrieNode &node) const {
  // Find leftmost of matched prefix
  const TrieNode *curr = &node.next.front();
  while (curr->idxRange.first == -1) {
    // Check in case tree not properly constructed
    if (curr->next.empty())
      return TrieQueryResult{TFoundPlaces(places->cend(), places->cend()), false};
    curr = &curr->next.front();
  }
  const int idxLeft = curr->idxRange.first;
  
  // Find rightmost of matched prefix
  curr = &node.next.back();
  while (curr->idxRange.first == -1) {
    // Check in case tree not properly constructed
    if (curr->next.empty())
      return TrieQueryResult{TFoundPlaces(places->cend(), places->cend()), false};
    curr = &curr->next.back();
  }
  const int idxRight = curr->idxRange.first;
  
  return TrieQueryResult{
    TFoundPlaces(places->cbegin() + idxLeft,
                 places->cbegin() + idxRight),
                 true
  };
}

void Trie::construct(const int begin, const int end,
                     const int depth, TrieNode &node) const {
  // Start of the current sub-range being constructed
  int idx = begin;
  while (idx < end) {
    // End of the sub-range being constructed
    const int nextEnd = endOfSameLetterRange(idx, end, depth);
    
    const char c = (char)std::tolower((*places)[idx].cityName[depth]);
    if (c == '\0') {
      // Save the range of entries with same value
      node.idxRange = { idx, nextEnd };
    } else {
      node.next.emplace_back(c);
      construct(idx, nextEnd, depth + 1, node.next.back());
    }
    
    // Continue building the next chunk of the sub-range
    idx = nextEnd;
  }
}

int Trie::endOfSameLetterRange(const int fm, const int to,
                               const size_t depth) const {
  const char c = (char)std::tolower((*places)[fm].cityName[depth]);
  for (int i = fm + 1; i < to; ++i) {
    if (std::tolower((*places)[i].cityName[depth]) != c)
      return i;
  }
  
  return to;
}
