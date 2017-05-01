#include "SetAssociativeCache.h"

#define HIT_SHOULD_COUNT true
#define HIT_SHOULD_NOT_COUNT false

SetAssociativeCache::SetAssociativeCache(int lineSize, int cacheSize, int associativity, bool noAllocationOnWriteMissFlag, bool nextLinePrefetchingFlag, bool prefetchOnMissFlag) {
  this->line_size = lineSize;
  this->cache_size = cacheSize;
  this->num_entries = cacheSize / lineSize;
  this->associativity = associativity;
  this->num_sets = num_entries / associativity;
  this->no_allocation_on_write_miss_flag = noAllocationOnWriteMissFlag;
  this-> next_line_prefetching_flag = nextLinePrefetchingFlag;
  this-> prefetch_on_miss_flag = prefetchOnMissFlag;
  // ^ should make these enums i know im sorry
  this->entries = new std::deque<unsigned long long>[num_entries];
}

bool SetAssociativeCache::store(unsigned long long address) {
  if (no_allocation_on_write_miss_flag) { 
    return noAllocationOnWriteMissStore(address);
  } else {
    return load(address);
  }
}

bool SetAssociativeCache::load(unsigned long long address) {
  if (next_line_prefetching_flag) {
    return nextLinePrefetchingAccess(address);
  } else if (prefetch_on_miss_flag) {
    return prefetchOnMissAccess(address);
  } else {
    return normalLoad(address, HIT_SHOULD_COUNT);
  }
}

bool SetAssociativeCache::noAllocationOnWriteMissStore(unsigned long long address) {
  bool hit = false;
  ++queries;

  std::deque<unsigned long long>* set = &entries[indexBitsOf(address)];
  unsigned long long tag_bits = tagBitsOf(address);
  for (std::deque<unsigned long long>::iterator i = set->begin(); i != set->end(); ++i) {
    if (*i == tag_bits) {
      ++hits;
      hit = true;
      set->erase(i);
      break;
    }
  }

  if (hit) {
    set->push_front(tag_bits);
  }

  return hit;
}

bool SetAssociativeCache::nextLinePrefetchingAccess(unsigned long long address) {
  bool hit = normalLoad(address, HIT_SHOULD_COUNT);
  normalLoad(addressToPrefetchBasedOn(address), HIT_SHOULD_NOT_COUNT);
  return hit;
}

bool SetAssociativeCache::prefetchOnMissAccess(unsigned long long address) {
  if (!normalLoad(address, HIT_SHOULD_COUNT)) {
    normalLoad(addressToPrefetchBasedOn(address), HIT_SHOULD_NOT_COUNT);
    return false;
  }

  return true;
}

bool SetAssociativeCache::normalLoad(unsigned long long address, bool hitShouldCount) {
  bool hit = false;
  if (hitShouldCount) {
    ++queries;  
  }

  std::deque<unsigned long long>* set = &entries[indexBitsOf(address)];
  unsigned long long tag_bits = tagBitsOf(address);
  for (std::deque<unsigned long long>::iterator i = set->begin(); i != set->end(); ++i) {
    if (*i == tag_bits) {
      if (hitShouldCount) {
        ++hits;
      }
      hit = true;
      set->erase(i);
      break;
    }
  }

  if (set->size() == associativity) {
    set->pop_back();
  }

  set->push_front(tag_bits);

  return hit;
}

unsigned long long SetAssociativeCache::indexBitsOf(unsigned long long address) {
  return (address / line_size) % num_sets;
}

unsigned long long SetAssociativeCache::tagBitsOf(unsigned long long address) {
  return (address / line_size) / num_sets;
}

unsigned long long SetAssociativeCache::addressToPrefetchBasedOn(unsigned long long address) {
  return ((address / line_size) + 1) * line_size;
}

SetAssociativeCache::~SetAssociativeCache() {
  delete[] entries;
}
