#include <iostream>
#include <climits>
#include "InternalNode.h"
#include "LeafNode.h"

using namespace std;

InternalNode::InternalNode(int ISize, int LSize,
  InternalNode *p, BTreeNode *left, BTreeNode *right) :
  BTreeNode(LSize, p, left, right), internalSize(ISize)
{
  keys = new int[internalSize]; // keys[i] is the minimum of children[i]
  children = new BTreeNode* [ISize];
} // InternalNode::InternalNode()


BTreeNode* InternalNode::addPtr(BTreeNode *ptr, int pos)
{ // called when original was full, pos is where the node belongs.
  if(pos == internalSize)
    return ptr;

  BTreeNode *last = children[count - 1];

  for(int i = count - 2; i >= pos; i--)
  {
    children[i + 1] = children[i];
    keys[i + 1] = keys[i];
  } // shift things to right to make room for ptr, i can be -1!

  children[pos] = ptr;  // i will end up being the position that it is inserted
  keys[pos] = ptr->getMinimum();
  ptr->setParent(this);
  return last;
} // InternalNode:: addPtr()


void InternalNode::addToLeft(BTreeNode *last)
{
  ((InternalNode*)leftSibling)->insert(children[0]);

  for(int i = 0; i < count - 1; i++)
  {
    children[i] = children[i + 1];
    keys[i] = keys[i + 1];
  }

  children[count - 1] = last;
  keys[count - 1] = last->getMinimum();
  last->setParent(this);
  if(parent)
    parent->resetMinimum(this);
} // InternalNode::ToLeft()


void InternalNode::addToRight(BTreeNode *ptr, BTreeNode *last)
{
  ((InternalNode*) rightSibling)->insert(last);
  if(ptr == children[0] && parent)
    parent->resetMinimum(this);
} // InternalNode::addToRight()



void InternalNode::addToThis(BTreeNode *ptr, int pos)
{  // pos is where the ptr should go, guaranteed count < internalSize at start
  int i;

  for(i = count - 1; i >= pos; i--)
  {
      children[i + 1] = children[i];
      keys[i + 1] = keys[i];
  } // shift to the right to make room at pos

  children[pos] = ptr;
  keys[pos] = ptr->getMinimum();
  count++;
  ptr->setParent(this);

  if(pos == 0 && parent)
    parent->resetMinimum(this);
} // InternalNode::addToThis()



int InternalNode::getMaximum() const
{
  if(count > 0) // should always be the case
    return children[count - 1]->getMaximum();
  else
    return INT_MAX;
}  // getMaximum();


int InternalNode::getMinimum()const
{
  if(count > 0)   // should always be the case
    return children[0]->getMinimum();
  else
    return 0;
} // InternalNode::getMinimum()


InternalNode* InternalNode::insert(int value)
{  // count must always be >= 2 for an internal node
  int pos; // will be where value belongs

  for(pos = count - 1; pos > 0 && keys[pos] > value; pos--);

  BTreeNode *last, *ptr = children[pos]->insert(value);
  if(!ptr)  // child had room.
    return NULL;

  if(count < internalSize)
  {
    addToThis(ptr, pos + 1);
    return NULL;
  } // if room for value

  last = addPtr(ptr, pos + 1);

  if(leftSibling && leftSibling->getCount() < internalSize)
  {
    addToLeft(last);
    return NULL;
  }
  else // left sibling full or non-existent
    if(rightSibling && rightSibling->getCount() < internalSize)
    {
      addToRight(ptr, last);
      return NULL;
    }
    else // both siblings full or non-existent
      return split(last);
} // InternalNode::insert()


void InternalNode::insert(BTreeNode *oldRoot, BTreeNode *node2)
{ // Node must be the root, and node1
  children[0] = oldRoot;
  children[1] = node2;
  keys[0] = oldRoot->getMinimum();
  keys[1] = node2->getMinimum();
  count = 2;
  children[0]->setLeftSibling(NULL);
  children[0]->setRightSibling(children[1]);
  children[1]->setLeftSibling(children[0]);
  children[1]->setRightSibling(NULL);
  oldRoot->setParent(this);
  node2->setParent(this);
} // InternalNode::insert()


void InternalNode::insert(BTreeNode *newNode)
{ // called by sibling so either at beginning or end
  int pos;

  if(newNode->getMinimum() <= keys[0]) // from left sibling
    pos = 0;
  else // from right sibling
    pos = count;

  addToThis(newNode, pos);
} // InternalNode::insert(BTreeNode *newNode)


void InternalNode::print(Queue <BTreeNode*> &queue)
{
  int i;

  cout << "Internal: ";
  for (i = 0; i < count; i++)
    cout << keys[i] << ' ';
  cout << endl;

  for(i = 0; i < count; i++)
    queue.enqueue(children[i]);

} // InternalNode::print()

InternalNode* InternalNode::createNewRoot(InternalNode* current, InternalNode* sibling)
{ 
  //insert current node + both sibling nodes so new root has total M 
  
  int length = sibling->getCount(); 
  for (int i = 0; i < length; i++)
  {
    current->insert(sibling->children[i]);
    current->resetMinimum(current->children[current->getCount()+i]);
    
  }
  int j = 0; 
  while(j < length) 
  {
    sibling->count--; 
    j++; 
  } //sibling should now be zero 
  current->parent->setRightSibling(NULL);
  parent->count--;
  InternalNode* newRoot = new InternalNode(internalSize, leafSize, NULL, NULL, NULL); 
  newRoot->insert(current->children[0]);
  newRoot->children[0]->setLeftSibling(NULL);
  // newRoot->insert(current->children[1]);
  // newRoot->insert(current->children[2]);

  
  for(int i = 1; i < current->getCount(); i++)
  {
      //current->children[i]->printValues();
      newRoot->insert(current->children[i]);
      newRoot->children[i-1]->setRightSibling(newRoot->children[i]);
      newRoot->children[i]->setLeftSibling(newRoot->children[i-1]);
  }

  // cout <<"\n\nTESTING NEW ROOT " << endl;
  // newRoot->printValues();
  // cout <<"\n\n";

  //return (InternalNode*)current; 
  //return NULL;
  //cout << "this ran" <<endl;
  return newRoot;
}
BTreeNode* InternalNode::borrowLeftChild (InternalNode* current, InternalNode* sibling)
{
  BTreeNode* newSibling = sibling->children[sibling->getCount()-1];
  newSibling->setLeftSibling(NULL);
  newSibling->setRightSibling(current->children[0]);
  current->children[0]->setLeftSibling(newSibling);
  current->insert(newSibling);



  //current->resetMinimum(newSibling);
  //current->children[0]->setLeftSibling(NULL);
  sibling->children[sibling->getCount()-1] = NULL ;
  sibling->count--; 


  return NULL; 
}

BTreeNode* InternalNode::insertIntoLeft (InternalNode* current, InternalNode* sibling, int pos)
{
  //cout << "enterd" <<endl;
  // cout <<"\n\ncurrent"<<endl;
  // current->printValues();
  // cout << "\n\nsibling" <<endl;
  // sibling->printValues();

  //parent->printValues();
  // cout << "\n\n\n";
  // insert current into sibling 

  //testing: insert more than one nodes into left internal node

  BTreeNode* newSibling = NULL; 
  int currentCount = current->count; 
  for (int i = 0; i < currentCount; i++)
  {
    newSibling = current->children[i]; 
    if (current->getRightSibling())
      newSibling->setRightSibling( ((InternalNode*) current->getRightSibling())->children[0]);
    else newSibling->setRightSibling(NULL);
    newSibling->setLeftSibling(sibling->children[sibling->getCount()-1]);
    //newSibling->setRightSibling(current->parent->children[pos+1]);
    sibling->insert(newSibling);
  }

  sibling->setRightSibling(current->getRightSibling());
 //shift all the nodes over one 

  // InternalNode* parentNode = ((InternalNode* )sibling->parent); 
  // for (int j = pos; j<parentNode->getCount()-1; j++)
  // {
  //   parentNode->children[j]->printValues();
  //   cout << "\n\n";
  //   parent->children[j] = parent->children[j+1];
  //   parent->children[j]->setRightSibling(parent->children[j+1]);
  //   if (pos-1<0)
  //     parent->children[j]->setLeftSibling(NULL);
  //   else parent->children[j]->setLeftSibling(parent->children[j-1]);
  // }
  //sibling->parent->count--; 
  current->count = 0;


  // cout <<"\n\ncurrent"<<endl;
  // current->printValues();
  // cout << "\n\nsibling" <<endl;
  // sibling->printValues();

  //parent->printValues();

  return sibling; 
}

BTreeNode* InternalNode::insertIntoRight (InternalNode* current, InternalNode* sibling, int pos)
{
  BTreeNode* newSibling = NULL; 
  int currentCount = current->getCount();
  //cout << "\n\n"; 
  for (int i = 0; i < currentCount; i++)
  {
    newSibling=current->children[i];
    //newSibling->printValues();
    BTreeNode* leftptr = current->getLeftSibling();
    if (leftptr)
      newSibling->setLeftSibling(( (InternalNode*) leftptr)->children[leftptr->getCount()-1]);
    else newSibling->setLeftSibling(NULL);
    sibling->addToThis(newSibling,i);
    //cout << "count: " <<sibling->count << endl;
  }

  //sibling->setLeftSibling(current->getLeftSibling());
sibling->children[1]->setLeftSibling(sibling->children[0]);
sibling->children[0]->setRightSibling(sibling->children[1]);
  //sibling->children[1]->setLeftSibling(sibling->children[0]);
  
  //sibling->printValues();
  //shift all the nodes over one 

  // for (int j = pos-1; j >= 0 && j < sibling->parent->getCount()-1; j++)
  // {
  //   //parent->children[j] = parent->children[j+1]; 
  //   //parent->children[j]->setRightSibling(parent->children[j+1]);
  //   // if (j-1>=0)
  //   // {
  //   //   parent->children[j]->setLeftSibling(parent->children[j-1]);
  //   // }
  //   // else 
  //   // {
  //   //   parent->children[j]->setLeftSibling(NULL);
  //   // }

  // }//sibling->parent->count--; 

  //  sibling->children[1]->printValues(); 
  //  sibling->children[0]->printValues(); 

  //  if (sibling->children[1]->getLeftSibling())
  //  {
  //    cout << "\n\nHALJFLADSKFJLKSDFJLKSDJFLKSDLF\n\n" << endl;
  //    sibling->children[1]->getLeftSibling()->printValues(); 
  //  } 
  // sibling->children[1]->getRightSibling()->printValues(); 
  // sibling->parent->printValues();
  //sibling->children[1]->getLeftSibling()->printValues();

  
  current->count=0; 

  //if insert into right then the left one is empty right now 
  //current also has no left pointer
  sibling->setLeftSibling(current->leftSibling);
  InternalNode* parentNode = ( (InternalNode*)sibling->parent); 
  //parentNode->printValues(); 
  for (int i = 0; i < parent->getCount()-1; i++)
  {
    parentNode->children[i] = parentNode->children[i+1];
    parentNode->resetMinimum(parentNode->children[i]);
  }parentNode->count--; 
  //sibling->parent->count--;
  //cout << "\n\n";

  return parentNode;
}
BTreeNode* InternalNode::borrowRightChild (InternalNode* current, InternalNode *sibling)
{
  //insert sibling->children[0] into current->children[1]
  BTreeNode* newSibling = sibling->children[0];
  current->insert(newSibling);
  current->children[0]->setRightSibling(newSibling);
  

  //shift the other siblings and reduce sibling count
  // cout << "\n\nTESTING TESTING " <<endl;
  // cout <<"THIS PTR: " << endl;
  // current->printValues();

  //remove sibling->children[0];
  BTreeNode* rightptr = newSibling->getRightSibling();
  rightptr->setLeftSibling(NULL);
  sibling->children[0] = rightptr; 

  sibling->resetMinimum(sibling->children[0]);

  for (int i = 1; i<sibling->getCount()-1; i++)
  {
    sibling->children[i] = sibling->children[i+1];
    sibling->resetMinimum(sibling->children[i]);
    //cout << "minimum: " << sibling->children[i]->getMinimum();
  }

  sibling->count--; 

  
  // cout <<"\n\this PTR:"<<endl;
  //current->printValues();

  return NULL;
}

BTreeNode* InternalNode::remove(int value)
{  // to be written by students
  int i = 0; 
  while (i < count && value > children[i]->getMaximum())
  {
    i++; 
  }if (value > children[count-1]->getMaximum()) return NULL ;
  
  //cout <<"ENTERED ENTERNED ETNERERENTENRED " <<value <<endl;
  //remove value, if merge happens then return the pointer of the new leaf
  BTreeNode* rightptr = NULL; 
  BTreeNode* leftptr = NULL; 
  if (i+1 < count) rightptr = children[i+1];
  if (i-1 > 0) leftptr = children[i-1];

  BTreeNode* merged = children[i]->remove(value);
  if (merged) // if not null then is the merged pointer to fix siblings
  {

  // if (value == 19) cout << "here" << endl;
  //   printValues();
  //   cout << i << endl;
    if (!( ((InternalNode*)  merged)->parent) )
    {
      return merged; 
    }
    
    children[i] = merged;
    if (rightptr && rightptr->getMaximum() == children[i]->getMaximum()) // after the line children[i]=merged; 
    { rightptr=rightptr->getRightSibling(); }
    children[i]->setRightSibling(rightptr);
    
    if (leftptr && leftptr->getMaximum() == children[i]->getMaximum())
    {
      leftptr=leftptr->getLeftSibling();
    }
    children[i]->setLeftSibling(leftptr);
    
     for (int pos = i+1; pos+1 < count; pos++)
     {
      children[pos] = children[pos+1]; 
      children[pos]->setRightSibling(children[pos+1]);
      if (pos-1 < 0) 
        children[pos]->setLeftSibling(NULL);
      else  children[pos]->setLeftSibling(children[pos-1]);

       resetMinimum(children[pos]);
     }count--; 
     children[count-1]->setRightSibling(NULL);
    

    
  }

  leftptr=getLeftSibling();
  rightptr=getRightSibling();
    if(parent && parent->getCount() == 1 && parent->children[0]->getCount()==1)
    {
      // if (value == 6)
      // {
      //   //parent->printValues();
      //   cout << i << endl;
      //   parent->printValues();
      //   //parent->children[i]->printValues();

      // }
     // parent->printValues();
      //parent->children[0]->printValues();
      BTreeNode* rightsib = parent->children[i]->getRightSibling();
      BTreeNode* leftsib = parent->children[i]->getLeftSibling(); 
      
      if (!rightsib && !leftsib)
        return children[0];
      
    }
  
  //if (!parent && count == 1 && !children[0]->getLeftSibling() && !children[0]->getRightSibling()) 
    if (!parent && count == 1)// chances are that this should be the root 
    {
      // (value == 4)cout<< "enterd" << endl;
      //cout << "I have no parent sir and this is my count " << count << endl;
      // cout <<"gulitlytlyt\n\n\n" <<endl;
      //  children[0]->printValues();
      // children[0]->getRightSibling()->printValues();
      //  children[1]->printValues();
      return children[0]; 
      //this->printValues(); 
    }
   
  //merge internal nodes if needed 

  // if current internal node has less than M/2, then it looks left
  // takes left child 
  if (parent && count < (internalSize+1)/2)
  {
    //printValues();
    //if (value == 26) cout << "entered internal" <<endl;
    //cout << "MERGEEEEEEE  but first, value = " << value << endl;
    // if(parent->getCount()==1)
    // {
    //   if (value == 21) cout << "\n\nENTERED6\n\n";
    // }
    if( leftptr && leftptr->getCount() >  (internalSize+1)/2 )//IS leftptr NULL?
    {
      //cout << "is this a left situation" << endl;
      //leftptr->printValues();
      //if (value == 26) cout << "entered internal2" <<endl;
      return borrowLeftChild(((InternalNode*)this),(InternalNode*)leftptr);
    }

    if (leftptr && (leftptr->getCount()+count) <= internalSize)
    {
      //cout << "enterd here " << endl;
      //if (value == 26) cout << "entered internal3" <<endl;
      return insertIntoLeft(((InternalNode*)this),(InternalNode*)leftptr, i); 
    }
    
    if (rightptr && rightptr->getCount() > (internalSize+1)/2)
    {
      //if (value == 26) cout << "entered internal4" <<endl;
      //cout << "is this a right situation" << endl;
      //parent->children[i-1]->printValues();
      //if ( (count+rightptr->getCount()) == internalSize ) //is this a root situation 
      // make this if statement out, what if we're deleting something that has no right ptr but 
      // has total of two and therefore needs a new root 
      
      //borrow right child 
    //cout << "borrow right child" <<endl;
      return borrowRightChild((InternalNode*)this, (InternalNode*)rightptr);
    }

    if (rightptr && (rightptr->getCount()+count) <= internalSize)
    {
      //cout << "enterd here " << endl;
      // if (value == 26) 
      // {
      //   cout << "entered internal5" <<endl;
      // }

      if(parent && parent->getCount()==2 && (count+rightptr->getCount()) == internalSize)
      {
        //cout <<" root situation \n\n" <<endl;
        //if (value == 26) cout << "entered internal6" <<endl;
        parent= createNewRoot((InternalNode*)this, (InternalNode*)rightptr);
        
        //parent =  ((InternalNode*) parent->children[0]); 
        //  cout << "PARENT:"<<endl;
        //  parent->printValues();
        //  cout <<"\n\n";
        return parent;
        
        //return NULL; 
      }
      return insertIntoRight(((InternalNode*)this),(InternalNode*)rightptr, i); 
    }
    //look left, merge left 
    //look right, merge right

    //if (value == 21) cout << "\n\nENTERED6\n\n";
  }

  //if (value == 26) cout << "entered internal7" <<endl;
  //cout << "is this a nothing situation" << endl;

  //reset minimum of node but check first to see if has parent
  return NULL; // if value not found 
} // InternalNode::remove(int value)

void InternalNode::resetMinimum(const BTreeNode* child)
{
  for(int i = 0; i < count; i++)
    if(children[i] == child)
    {
      keys[i] = children[i]->getMinimum();
      if(i == 0 && parent)
        parent->resetMinimum(this);
      break;
    }
} // InternalNode::resetMinimum()


InternalNode* InternalNode::split(BTreeNode *last)
{
  InternalNode *newptr = new InternalNode(internalSize, leafSize,
    parent, this, rightSibling);


  if(rightSibling)
    rightSibling->setLeftSibling(newptr);

  rightSibling = newptr;

  for(int i = (internalSize + 1) / 2; i < internalSize; i++)
  {
    newptr->children[newptr->count] = children[i];
    newptr->keys[newptr->count++] = keys[i];
    children[i]->setParent(newptr);
  }

  newptr->children[newptr->count] = last;
  newptr->keys[newptr->count++] = last->getMinimum();
  last->setParent(newptr);
  count = (internalSize + 1) / 2;
  return newptr;
} // split()

// void InternalNode::printValues()
// {
//   for (int i = 0; i < count;i++)
//   {
//     cout << "PARENT: " << keys[i] << endl;
//     cout << "NODE " << i << endl;
//     children[i]->printValues();
//   }
// }