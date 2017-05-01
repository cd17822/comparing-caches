#include "FullyAssociativeCache.h"

FullyAssociativeCache::FullyAssociativeCache(int lineSize, int cacheSize, bool hotColdFlag) {
  this->line_size = lineSize;
  this->cache_size = cacheSize;
  this->num_entries = cacheSize / lineSize;
  this->hot_cold_flag = hotColdFlag;
  this->entries = new std::deque<unsigned long long>;
  this->tree = nullptr;

  if (hotColdFlag) {
    this->tree = new Tree(num_entries-1);
    for (int i = 0; i < num_entries; ++i) {
      this->entries->push_front(-1);
    }
  }
}

bool FullyAssociativeCache::store(unsigned long long address) {
  return hot_cold_flag ? hotColdAccess(address) : normalAccess(address);
}

bool FullyAssociativeCache::load(unsigned long long address) {
  return store(address);
}

bool FullyAssociativeCache::hotColdAccess(unsigned long long address) {
  ++queries;

  for (std::deque<unsigned long long>::iterator i = entries->begin(); i != entries->end(); ++i) {
    if (*i == nonOffsetBitsOf(address)) {
      ++hits;
      tree->propUpFrom(i - entries->begin());
      return true;
    }
  }

  int index = tree->getIndexToInsertAt();
  (*entries)[index] = nonOffsetBitsOf(address);
  tree->propUpFrom(index);

  return false;
}

bool FullyAssociativeCache::normalAccess(unsigned long long address) {
  bool hit = false;
  ++queries;

  for (std::deque<unsigned long long>::iterator i = entries->begin(); i != entries->end(); ++i) {
    if (*i == nonOffsetBitsOf(address)) {
      ++hits;
      hit = true;
      entries->erase(i);
      break;
    }
  }
 
  if (entries->size() == num_entries) {
    entries->pop_back();
  }

  entries->push_front(nonOffsetBitsOf(address));

  return hit;
}

unsigned long long FullyAssociativeCache::nonOffsetBitsOf(unsigned long long address) {
  return address / line_size;
} 

void FullyAssociativeCache::testTree() {
  Tree* t = new Tree(7);

  int i = t->getIndexToInsertAt();
  printf("%d\n", i);
  t->propUpFrom(i);
  for (int i = 0; i < 7; ++i) {
    // printf("%d\n", t->data[i]);
  }

  int j = t->getIndexToInsertAt();
  printf("%d\n", j);
  t->propUpFrom(j);
  for (int i = 0; i < 7; ++i) {
    printf("%d\n", t->data[i]);
  }

  int k = t->getIndexToInsertAt();
  printf("%d\n", k);
  t->propUpFrom(k);
  for (int i = 0; i < 7; ++i) {
    // printf("%d\n", t->data[i]);
  }

  int q = t->getIndexToInsertAt();
  printf("%d\n", q);
  t->propUpFrom(q);
  for (int i = 0; i < 7; ++i) {
    // printf("%d\n", t->data[i]);
  }
}

FullyAssociativeCache::~FullyAssociativeCache() {
  delete entries;
}
