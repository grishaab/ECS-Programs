#include <iostream>
#include <climits>
#include "LeafNode.h"
#include "InternalNode.h"
#include "QueueAr.h"

using namespace std;


LeafNode::LeafNode(int LSize, InternalNode *p,
  BTreeNode *left, BTreeNode *right) : BTreeNode(LSize, p, left, right)
{
  values = new int[LSize];
}  // LeafNode()

void LeafNode::addToLeft(int value, int last)
{
  leftSibling->insert(values[0]);

  for(int i = 0; i < count - 1; i++)
    values[i] = values[i + 1];

  values[count - 1] = last;
  if(parent)
    parent->resetMinimum(this);
} // LeafNode::ToLeft()

void LeafNode::addToRight(int value, int last)
{
  rightSibling->insert(last);

  if(value == values[0] && parent)
    parent->resetMinimum(this);
} // LeafNode::addToRight()

void LeafNode::addToThis(int value)
{
  int i;

  for(i = count - 1; i >= 0 && values[i] > value; i--)
      values[i + 1] = values[i];

  values[i + 1] = value;
  count++;

  if(value == values[0] && parent)
    parent->resetMinimum(this);
} // LeafNode::addToThis() 
//adds values in current leafnode by iterating through the 
//array backwards and shifting it one spot ahead until it sorts
//value into new spot
//count increases and resets minimum


void LeafNode::addValue(int value, int &last)
{
  int i;

  if(value > values[count - 1])
    last = value;
  else
  {
    last = values[count - 1];

    for(i = count - 2; i >= 0 && values[i] > value; i--)
      values[i + 1] = values[i];
    // i may end up at -1
    values[i + 1] = value;
  }
} // LeafNode:: addValue()  


int LeafNode::getMaximum()const
{
  if(count > 0)  // should always be the case
    return values[count - 1];
  else
    return INT_MAX;
} // getMaximum()


int LeafNode::getMinimum()const
{
  if(count > 0)  // should always be the case
    return values[0];
  else
    return 0;

} // LeafNode::getMinimum()


LeafNode* LeafNode::insert(int value)
{
  int last;

  if(count < leafSize)//count(leafnode) is less than the leafsize
  {
    addToThis(value);
    return NULL;
  } // if room for value

  addValue(value, last);

  if(leftSibling && leftSibling->getCount() < leafSize)//checks if leftsibling is null
  {
    addToLeft(value, last);
    return NULL;
  }
  else // left sibling full or non-existent
    if(rightSibling && rightSibling->getCount() < leafSize)
    {
      addToRight(value, last);
      return NULL;
    }
    else // both siblings full or non-existent
      return split(value, last);
}  // LeafNode::insert()

void LeafNode::print(Queue <BTreeNode*> &queue)
{
  cout << "Leaf: ";
  for (int i = 0; i < count; i++)
    cout << values[i] << ' ';
  cout << endl;
} // LeafNode::print()

int LeafNode::removeThis(int value)
{
  //shift the others forward 

  // 17 18 20 29 40  count = 5, delete 20 
  // 17 18 20 29 20 
  
   //position of value in array values 
  //currently i = 2 because 20 is the third value 
  //cout << "for loop for deleting value / sending it to the back" << endl;

  int i = count-1; 
  while(i >= 0 && values[i] > value)
    i--; 
  if (i < 0 || values[i] != value||value>values[count-1]) return 2;
  

  //int delValue = values[i];
  int j; 
  //now we want values[2] =  values [3], values[3] = values[4]
  for (j = i; j < count-1; j++)
  {
    values[j]= values[j+1];
  }
  values[count-1] = -1; 
  //values[count-1]=delValue;

//  cout << "printing current values: " << count << endl; 
//   for (int i = 0; i < count; i++)
//   {
//     cout << values[i] << " "; 
//   }cout << endl;

  count--; 
  return 4; //placeholder for now 
}

LeafNode* LeafNode::merge(int value, BTreeNode *sibling)
{
  //merge w sibling
  //remove value from leaf regardless of leafsize 
  removeThis(value);
  // if (value == 24) {
     //sibling->printValues();
  //   this->printValues();
  //   sibling->printValues();
  //}
  //insert all current values into sibling 

  for (int i = 0; i < count; i++)
  {
    sibling->insert(values[i]);
  }
  //print out merged leaf values 
  // if (value == 9)
  // { cout << "new merged values: " ; 
  //  for (int i = 0; i< sibling->getCount(); i++)
  //    cout << ((LeafNode*)sibling)->values[i] << " ";
  //  cout << endl; }

  return (LeafNode*)sibling; 

   
}
LeafNode* LeafNode::remove(int value)
{   // To be written by students
  //Assume leafsize = 6; current count = 
  //remove by simply removing it from leafnode 

  int i = count-1; 
  while(i >= 0 && values[i] > value)
    i--; 
  if (i < 0 || values[i] != value||value>values[count-1]) return NULL; //value not found in values array
  //if (value == 9) cout << "\n\nentered\n\n";
  //if (value==2)cout<<"entered"<<endl;
  // if (value == 19) 
  // {
  //   printValues();
  //   parent->printValues();
  //   //leftSibling->printValues();
  //   cout << "\n\nentered\n\n";
  // }
  // if (value == 12)
  // {
  //   cout <<"\n\n";
  //   printValues();
  //   if (leftSibling)
  //   leftSibling->printValues();
  // }
  // if (value==15) 
  // {
  //   cout << "\n\nbefore, value = " << value << endl;
  //   printValues();
  //   cout <<"\n\n parent node: " <<endl;
  //   parent->printValues();

  //   cout <<"\n\n" <<endl;
  // }
  // if(value==24) 
  // {
  //   cout << "after, value = "<< value<< endl;
  //   printValues();
  //   cout <<"\n\n parent node: " <<endl;
  //   parent->printValues();

  //   cout <<"\n\n" <<endl;

  // // }
  // if (leftSibling)
  // {
  //   cout <<"\n\nLEFT SIBLING OF " <<value<<endl;
  //   leftSibling->printValues();
  //   cout <<"\n\n";
  // }
  if(count > (leafSize+1)/2)//count(leafnode) is less than the leafsize
  {
    //cout <<"entered1" <<endl;
    //if (value == 9) cout << "\n\nentered2\n\n";
    removeThis(value);
    if (parent) parent->resetMinimum(this);
    return NULL; 
  } // if count-1 of current leaf node will be < leafsize/2
  //remove by removing and then borrowing from left/right sibling but don't merge
  
  if(leftSibling && leftSibling->getCount() > (leafSize+1)/2)//checks if leftsibling is null
  {
 //if (value == 9) cout << "\n\nentered3\n\n";
    //cout <<"entered2" <<endl;
    int borrowed = leftSibling->getMaximum();
    ( (LeafNode*) leftSibling)->removeThis(borrowed);
    insert(borrowed);
    removeThis(value);
    if (parent) parent->resetMinimum(leftSibling);
    return NULL; 
  } 
  else
  {
    if(leftSibling)
    {
      //if (value == 9) cout << "\n\nentered4\n\n";
   //cout <<"entered3" <<endl;
      //cout<< "merging left " << value << endl; 
      // if (value == 24)
      
      // {leftSibling->printValues();
      // ( (InternalNode*) parent)->children[0]->printValues();}
      //cout << leftSibling->getMinimum()<<endl;
      return merge(value, leftSibling);
    }
  }
  //cout << "RIGHT COUNT: " <<rightSibling->getCount()<<endl;
  //rightSibling->printValues();
  if(rightSibling && rightSibling->getCount() > (leafSize+1)/2)//checks if leftsibling is null
  {
    //if (value == 9) cout << "\n\nentered5\n\n";
    //cout <<"entered4" <<endl;
    int borrowed = rightSibling->getMinimum(); // removes minimum from left sibling and inserts it into the current 
    ((LeafNode*)rightSibling)->removeThis(borrowed);
    insert(borrowed);
    removeThis(value);
    if (parent) 
    {parent->resetMinimum(rightSibling);
    parent->resetMinimum(this);
    }
    return NULL; 
    
  } 
  else 
  {
    if (rightSibling)
    {
     //if (value == 9) cout << "\n\nentered6\n\n";
      //cout<< "merging right " << value << endl; 
      // if (value ==14)
      // {
      //   printValues();

      //   rightSibling->printValues();
      // }
      // cout <<"\n\nTHIS PTR:"<<endl;
      // //this->printValues();
      // parent->printValues();

      // cout <<"\n\nSIBLING PTR:"<<endl;
      //cout << "entered 5, value = " <<value <<endl;
      // rightSibling->printValues();
      return merge(value, rightSibling);
    }
  }
//if (value == 9) cout << "\n\nentered7\n\n";
    //cout <<"entered6" <<endl;
  removeThis(value);
  return NULL; //no merge

}  // LeafNode::remove()



LeafNode* LeafNode::split(int value, int last)
{
  LeafNode *ptr = new LeafNode(leafSize, parent, this, rightSibling);

   
  if(rightSibling)
    rightSibling->setLeftSibling(ptr);

  rightSibling = ptr;

  for(int i = (leafSize + 1) / 2; i < leafSize; i++)
    ptr->values[ptr->count++] = values[i];

  ptr->values[ptr->count++] = last;
  count = (leafSize + 1) / 2;

  if(value == values[0] && parent)
    parent->resetMinimum(this);
  return ptr;
} // LeafNode::split()

// void LeafNode::printValues()
// {
//   cout << "PRINTING LEAF VALUES: "; 
//   for (int i = 0; i < count; i++)
//   {
//     cout << values[i] << " ";
//   }cout<<endl;
// }