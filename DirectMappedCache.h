#include "Cache.h"

class DirectMappedCache : public Cache {
public:
  DirectMappedCache(int lineSize, int cacheSize);
  bool store(unsigned long long address);
  bool load(unsigned long long address);
  ~DirectMappedCache();

private:
  int line_size; // in bytes
  int cache_size; // in bytes
  int num_entries; // cache_size / line_size
  unsigned long long* entries;

  unsigned long long indexBitsOf(unsigned long long address);
  unsigned long long tagBitsOf(unsigned long long address);
};
