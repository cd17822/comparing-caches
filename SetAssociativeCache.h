#include "Cache.h"
#include <deque>

class SetAssociativeCache : public Cache {
public:
  SetAssociativeCache(int lineSize, int cacheSize, int associativity, bool noAllocationOnWriteMissFlag, bool nextLinePrefetchingFlag, bool prefetchOnMissFlag);
  bool store(unsigned long long address);
  bool load(unsigned long long address);
  ~SetAssociativeCache();

private:
  int line_size; // in bytes
  int cache_size; // in bytes
  int num_entries; // cache_size / line_size
  int associativity;
  int num_sets; // num_entries / associativity
  bool no_allocation_on_write_miss_flag;
  bool next_line_prefetching_flag;
  bool prefetch_on_miss_flag;
  std::deque<unsigned long long>* entries;

  unsigned long long indexBitsOf(unsigned long long address);
  unsigned long long tagBitsOf(unsigned long long address);
  unsigned long long addressToPrefetchBasedOn(unsigned long long address);
  bool noAllocationOnWriteMissStore(unsigned long long address);
  bool nextLinePrefetchingAccess(unsigned long long address);
  bool prefetchOnMissAccess(unsigned long long address);
  bool normalLoad(unsigned long long address, bool hitShouldCount);
};