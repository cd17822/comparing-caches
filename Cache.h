#ifndef _Cache_H_
#define _Cache_H_

class Cache {
public:
  unsigned long long hits = 0;
  unsigned long long queries = 0;

  virtual bool store(unsigned long long address) = 0;
  virtual bool load(unsigned long long address) = 0;
  virtual unsigned long long getHits()    { return hits; }
  virtual unsigned long long getQueries() { return queries; }

};

#endif
