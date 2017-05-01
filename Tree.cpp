#include "Tree.h"

Tree::Tree(int size) {
  this->size = size;
  this->data = new int[size];
  
  for (int i = 0; i < size; ++i) {
    data[i] = 0;
  }
}

int Tree::getIndexToInsertAt(){ 
  int n = 0;
  while (n < size) {
    n = 2 * n + 1 + ((data[n]+1)%2);
  }
  return n - size;
}

void Tree::propUpFrom(int index) {
  int n = index + size;

  while (n > 0) {
    data[(n-1)/2] = n-1 == 0 ? 0 : ((n-1)/2 == (n-2)/2);
    n = (n-1)/2;
  }
}

Tree::~Tree() {
  delete[] data;
}
