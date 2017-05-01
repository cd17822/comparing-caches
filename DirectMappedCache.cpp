#include "DirectMappedCache.h"

DirectMappedCache::DirectMappedCache(int lineSize, int cacheSize) {
  this->line_size = lineSize;
  this->cache_size = cacheSize;
  this->num_entries = cacheSize / lineSize;
  this->entries = new unsigned long long[num_entries];
}

bool DirectMappedCache::store(unsigned long long address) {
  ++queries;

  if (entries[indexBitsOf(address)] == tagBitsOf(address)) {
    ++hits;
    return true;
  }

  entries[indexBitsOf(address)] = tagBitsOf(address);

  return false;
}

bool DirectMappedCache::load(unsigned long long address) {
  return store(address);
}

unsigned long long DirectMappedCache::indexBitsOf(unsigned long long address) {
  return (address / line_size) % num_entries;
}

unsigned long long DirectMappedCache::tagBitsOf(unsigned long long address) {
  return (address / line_size) / num_entries;
}

DirectMappedCache::~DirectMappedCache() {
  delete[] entries;
}