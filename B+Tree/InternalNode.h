#ifndef InternalNodeH
  #define InternalNodeH

#include "BTreeNode.h"

class InternalNode:public BTreeNode
{
  int internalSize;
  //BTreeNode **children;
  int *keys;
public:
BTreeNode **children;
  InternalNode(int ISize, int LSize, InternalNode *p,
    BTreeNode *left, BTreeNode *right);
  BTreeNode* addPtr(BTreeNode *ptr, int pos);
  void addToLeft(BTreeNode *last);
  void addToRight(BTreeNode *ptr, BTreeNode *last);
  void addToThis(BTreeNode *ptr, int pos); // pos is where the node should go
  int getMaximum() const;
  int getMinimum() const;
  InternalNode* insert(int value); // returns pointer to new
    // InternalNode if it splits else NULL
  void insert(BTreeNode *oldRoot, BTreeNode *node2); // if root splits use this
  void insert(BTreeNode *newNode); // from a sibling
  void print(Queue <BTreeNode*> &queue);
  BTreeNode* remove(int value);
  void resetMinimum(const BTreeNode* child);
  InternalNode* split(BTreeNode *last);

  //BTreeNode* removeNode(BTreeNode* node, int pos);
  //void printValues();
  BTreeNode* borrowRightChild(InternalNode* current, InternalNode* sibling);
  BTreeNode* borrowLeftChild(InternalNode* current, InternalNode* sibling);
  BTreeNode* insertIntoLeft(InternalNode* current, InternalNode* sibling, int pos);
  BTreeNode* insertIntoRight(InternalNode* current, InternalNode* sibling, int pos);
  InternalNode* createNewRoot(InternalNode* current, InternalNode* sibling);
}; // InternalNode class



#endif
