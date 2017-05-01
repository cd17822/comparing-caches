#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include "Cache.h"
#include "DirectMappedCache.h"
#include "SetAssociativeCache.h"
#include "FullyAssociativeCache.h"

#define K *1024
#define HOT_COLD_FLAG_OFF false
#define HOT_COLD_FLAG_ON true
#define NO_ALLOCATION_ON_WRITE_MISS_FLAG_OFF false
#define NO_ALLOCATION_ON_WRITE_MISS_FLAG_ON true
#define NEXT_LINE_PREFETCHING_FLAG_OFF false
#define NEXT_LINE_PREFETCHING_FLAG_ON true
#define PREFETCH_ON_MISS_FLAG_OFF false
#define PREFETCH_ON_MISS_FLAG_ON true
using namespace std;

int main(int argc, char const *argv[]) {
  // Check for correct number of args
  if (argc != 3) {
    fprintf(stderr, "Incorrect number of arguments.\n");
    exit(1);
  }

  // Init caches
  shared_ptr<Cache> dmc1 = make_shared<DirectMappedCache>(32, 1 K);
  shared_ptr<Cache> dmc4 = make_shared<DirectMappedCache>(32, 4 K);
  shared_ptr<Cache> dmc16 = make_shared<DirectMappedCache>(32, 16 K);
  shared_ptr<Cache> dmc32 = make_shared<DirectMappedCache>(32, 32 K);
  
  shared_ptr<Cache> sac2 = make_shared<SetAssociativeCache>(32, 16 K, 2, NO_ALLOCATION_ON_WRITE_MISS_FLAG_OFF, NEXT_LINE_PREFETCHING_FLAG_OFF, PREFETCH_ON_MISS_FLAG_OFF);
  shared_ptr<Cache> sac4 = make_shared<SetAssociativeCache>(32, 16 K, 4, NO_ALLOCATION_ON_WRITE_MISS_FLAG_OFF, NEXT_LINE_PREFETCHING_FLAG_OFF, PREFETCH_ON_MISS_FLAG_OFF);
  shared_ptr<Cache> sac8 = make_shared<SetAssociativeCache>(32, 16 K, 8, NO_ALLOCATION_ON_WRITE_MISS_FLAG_OFF, NEXT_LINE_PREFETCHING_FLAG_OFF, PREFETCH_ON_MISS_FLAG_OFF);
  shared_ptr<Cache> sac16 = make_shared<SetAssociativeCache>(32, 16 K, 16, NO_ALLOCATION_ON_WRITE_MISS_FLAG_OFF, NEXT_LINE_PREFETCHING_FLAG_OFF, PREFETCH_ON_MISS_FLAG_OFF);

  shared_ptr<Cache> facLRU = make_shared<FullyAssociativeCache>(32, 16 K, HOT_COLD_FLAG_OFF);
  
  shared_ptr<Cache> facHCLRU = make_shared<FullyAssociativeCache>(32, 16 K, HOT_COLD_FLAG_ON);

  shared_ptr<Cache> sacNAOWM2 = make_shared<SetAssociativeCache>(32, 16 K, 2, NO_ALLOCATION_ON_WRITE_MISS_FLAG_ON, NEXT_LINE_PREFETCHING_FLAG_OFF, PREFETCH_ON_MISS_FLAG_OFF);
  shared_ptr<Cache> sacNAOWM4 = make_shared<SetAssociativeCache>(32, 16 K, 4, NO_ALLOCATION_ON_WRITE_MISS_FLAG_ON, NEXT_LINE_PREFETCHING_FLAG_OFF, PREFETCH_ON_MISS_FLAG_OFF);
  shared_ptr<Cache> sacNAOWM8 = make_shared<SetAssociativeCache>(32, 16 K, 8, NO_ALLOCATION_ON_WRITE_MISS_FLAG_ON, NEXT_LINE_PREFETCHING_FLAG_OFF, PREFETCH_ON_MISS_FLAG_OFF);
  shared_ptr<Cache> sacNAOWM16 = make_shared<SetAssociativeCache>(32, 16 K, 16, NO_ALLOCATION_ON_WRITE_MISS_FLAG_ON, NEXT_LINE_PREFETCHING_FLAG_OFF, PREFETCH_ON_MISS_FLAG_OFF);

  shared_ptr<Cache> sacNLP2 = make_shared<SetAssociativeCache>(32, 16 K, 2, NO_ALLOCATION_ON_WRITE_MISS_FLAG_OFF, NEXT_LINE_PREFETCHING_FLAG_ON, PREFETCH_ON_MISS_FLAG_OFF);
  shared_ptr<Cache> sacNLP4 = make_shared<SetAssociativeCache>(32, 16 K, 4, NO_ALLOCATION_ON_WRITE_MISS_FLAG_OFF, NEXT_LINE_PREFETCHING_FLAG_ON, PREFETCH_ON_MISS_FLAG_OFF);
  shared_ptr<Cache> sacNLP8 = make_shared<SetAssociativeCache>(32, 16 K, 8, NO_ALLOCATION_ON_WRITE_MISS_FLAG_OFF, NEXT_LINE_PREFETCHING_FLAG_ON, PREFETCH_ON_MISS_FLAG_OFF);
  shared_ptr<Cache> sacNLP16 = make_shared<SetAssociativeCache>(32, 16 K, 16, NO_ALLOCATION_ON_WRITE_MISS_FLAG_OFF, NEXT_LINE_PREFETCHING_FLAG_ON, PREFETCH_ON_MISS_FLAG_OFF);

  shared_ptr<Cache> sacPOM2 = make_shared<SetAssociativeCache>(32, 16 K, 2, NO_ALLOCATION_ON_WRITE_MISS_FLAG_OFF, NEXT_LINE_PREFETCHING_FLAG_OFF, PREFETCH_ON_MISS_FLAG_ON);
  shared_ptr<Cache> sacPOM4 = make_shared<SetAssociativeCache>(32, 16 K, 4, NO_ALLOCATION_ON_WRITE_MISS_FLAG_OFF, NEXT_LINE_PREFETCHING_FLAG_OFF, PREFETCH_ON_MISS_FLAG_ON);
  shared_ptr<Cache> sacPOM8 = make_shared<SetAssociativeCache>(32, 16 K, 8, NO_ALLOCATION_ON_WRITE_MISS_FLAG_OFF, NEXT_LINE_PREFETCHING_FLAG_OFF, PREFETCH_ON_MISS_FLAG_ON);
  shared_ptr<Cache> sacPOM16 = make_shared<SetAssociativeCache>(32, 16 K, 16, NO_ALLOCATION_ON_WRITE_MISS_FLAG_OFF, NEXT_LINE_PREFETCHING_FLAG_OFF, PREFETCH_ON_MISS_FLAG_ON);

  // Collect caches into a vector
  vector< shared_ptr<Cache> > caches = {dmc1, dmc4, dmc16, dmc32, 
                                        sac2, sac4, sac8,  sac16,
                                        facLRU,
                                        facHCLRU,
                                        sacNAOWM2, sacNAOWM4, sacNAOWM8,  sacNAOWM16,
                                        sacNLP2, sacNLP4, sacNLP8,  sacNLP16,
                                        sacPOM2, sacPOM4, sacPOM8,  sacPOM16};

  // Temporary variables
  unsigned long long address;
  string behavior;
  bool is_store_instruction;

  // Open file for reading
  ifstream infile(argv[1]);
  ofstream outfile(argv[2]);

  // The following loop will read a hexadecimal number and
  // a string each time and then output them
  while(infile >> behavior >> std::hex >> address) {
    is_store_instruction = (behavior == "S");

    for (vector< shared_ptr<Cache> >::iterator cache = caches.begin(); cache != caches.end(); ++cache) {
      if (is_store_instruction) {
        cache->get()->store(address);
      } else {
        cache->get()->load(address);
      }
    }
  }  

  // Print results
  int i = 0;
  for (vector< shared_ptr<Cache> >::iterator cache = caches.begin(); cache != caches.end(); ++cache) {
    cout << cache->get()->getHits() << ',' << cache->get()->getQueries() << ';';
    if (i == 3 || i == 7 || i == 8 || i == 9 || i == 13 || i == 17 || i == 21) {
      cout << endl;
    } else {
      cout << ' ';
    }
    ++i;
  }

  return 0;
}
