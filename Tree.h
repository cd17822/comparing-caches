class Tree
{
public:
  int size;
  int* data;

  Tree(int size);
  int getIndexToInsertAt();
  void propUpFrom(int index);
  ~Tree();
};
