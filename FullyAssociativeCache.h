#include "Cache.h"
#include <deque>
#include <tuple>
#include <climits>
#include "Tree.h"

class FullyAssociativeCache : public Cache {
public:
  FullyAssociativeCache(int lineSize, int cacheSize, bool hotColdFlag);
  bool store(unsigned long long address);
  bool load(unsigned long long address);
  void testTree();
  ~FullyAssociativeCache();

private:
  int line_size; // in bytes
  int cache_size; // in bytes
  int num_entries; // cache_size / line_size
  bool hot_cold_flag;
  std::deque<unsigned long long>* entries;
  Tree* tree;

  unsigned long long nonOffsetBitsOf(unsigned long long address);
  bool normalAccess(unsigned long long address);
  bool hotColdAccess(unsigned long long address);
};
