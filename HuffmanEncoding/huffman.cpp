//include all the other .h files 
#include <iostream>
#include <fstream>
#include "BinaryTree.h"
#include "BinaryHeap.h"
using namespace std; 


int frequencies[128]; //# of possible characters
BinaryHeap<BinaryTreePtr> heap(500);
int* getFrequencies(char * fileName)
{
    ifstream inf(fileName); 
    char c; 
    int counter = 0; // counter to check if there are characters

    //initialize array 
    for (int i = 0; i < 128; i++)
        frequencies[i] = 0; 
    
    // treats enter / spaces as characters 
    while(inf >> noskipws >> c)
    {
        frequencies[c]++; 
        counter++; 
        //cout << c; 
    }

    // access characters w frequencies > 0
    // for (int i = 0; i < 128; i++)
    // {
    //     if (frequencies[i] > 0)
    //         cout << char(i) << ": " << frequencies[i] << endl;
    // }
    if (counter > 0)
        return frequencies;
    return NULL; 
}
BinaryTreePtr makeTries(int * count)
{ 
    int *end = count + 128; 
    int j = 0; //character 
    //each character is a binary tree, add them into the heap
    
    for (int* i = count; i != end; i++)
    {
        if(*i > 0)
        {
            //cout << char(j) << ": " << *i << endl;
            BinaryTree* trie = new BinaryTree(*i, j);
            BinaryTreePtr pointer = trie; 
            heap.insert(pointer);
        }
        j++; 
    }
    
    while (!heap.isEmpty())
    {
        BinaryTreePtr t1 = heap.findMin();
        heap.deleteMin();
        if (heap.isEmpty()) return t1;
        BinaryTreePtr t2 = heap.findMin();
        heap.deleteMin();

        int f1 = t1.getCurrentTree()->getFrequency(); 
        int f2 = t2.getCurrentTree()->getFrequency(); 
        BinaryTree* newTrie = new BinaryTree(f1+f2, t1.getCurrentTree(), t2.getCurrentTree());
        BinaryTreePtr pointer = newTrie; 
        //newTrie->printTree(newTrie);
        //pointer.getCurrentTree()->printTree(newTrie);
        heap.insert(pointer);
    }
    return NULL; 

}
void huffmanize(char * filename)
{
    int * count = getFrequencies(filename);
    if (!count) return; 
    BinaryTreePtr finalTrie = makeTries(count);
    finalTrie.getCurrentTree()->printTree(finalTrie.getCurrentTree());
}

int main(int argc, char **argv)
{
    //filename will be given as a command line args
    if (argc == 1 )
    {
        cout << "Not enough arguments" << endl;
        return 0; 
    } 

    char * fileName = argv[1];

    //read filename for every character 
    huffmanize(fileName);
}
