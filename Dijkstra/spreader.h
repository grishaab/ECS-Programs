#ifndef SPREADER_H
#define SPREADER_H

#include "spreaderRunner.h"
#include "BinaryHeapArray.h"
class Person3
{
  public:
  int ID; 
  int dv; 
  bool operator<(const Person3& b) const { return dv < b.dv; };
  
  Person3(int i = 0, int d = 0): ID (i), dv(d) {};
};
class Person2
{
  public:
    
    int dv; 
    bool known; 
    Visitor *visitors; // adjacency list 
    int peopleMet; 
    Person2(): dv(5000), known(false){};
}; //holds all info for dijsktra 

class Spreader
{
  //int totalpopulation;
  int day;
  public:
  Person2 * ppl;  
  Spreader(const Person *people, int population);
  int simulate(int starterIDs[], int starterCount, int commandLineOption);
  //void dijkstra(Person2 &source, BinaryHeap<Person2> &hp);
};


#endif /* SPREADER_H */

