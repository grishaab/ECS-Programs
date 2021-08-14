#include <iostream>  // for debugging
#include <cstring> 
#include "spreader.h"
using namespace std;
int totalpopulation;
Spreader::Spreader(const Person *people, int population)
{
	//initializer lists?? lwk makes it slower 
	// totalpopulation = population;
	day = 0;
	totalpopulation = population;

	//hash table sorted by ID
	//population 20 -> people id 0-19.
	ppl = new Person2[population];
	//initialize values in ppl to data from people
	for (int i = 0; i < population; ++i)
	{
		
		ppl[i].peopleMet = people[i].peopleMet;
		
		 
		ppl[i].visitors = new Visitor [people[i].peopleMet];
		memcpy(ppl[i].visitors, people[i].visitors, sizeof(Visitor)*ppl[i].peopleMet);
		// for (int j = 0; i < ppl[j].peopleMet; j++)
		// {
		// 	ppl[i].visitors[j] = people[i].visitors[j]; 
		// }
	}

} 

int Spreader::simulate(int starterIDs[], int starterCount, int commandLineOption)
{
	BinaryHeap<Person3> heap (totalpopulation);

	for (int i = 0; i < starterCount; ++i)
	{
		ppl[starterIDs[i]].dv = 0; 
		//ppl[starterIDs[i]].known = true; 

		//heap.insert(ppl[starterIDs[i]]);
		heap.insert(Person3(starterIDs[i],0));
	}
	Person3 source;
	while (heap.currentSize != 0)
	{
		heap.deleteMin(source);
		ppl[source.ID].known = true;

		if (day < source.dv)
			day = source.dv;
		
		for (int i = 0; i < ppl[source.ID].peopleMet; ++i)
		{
			int id = ppl[source.ID].visitors[i].ID; //go through visitors 
			if (ppl[id].known) 
				continue; //ignore everything if we've alr accounted for it

			int frequency = ppl[source.ID].visitors[i].period;
			//int increment = 0; 
			int infectingday = source.dv+3; //when will source be infectious 


			//if infecting day for A is 3 and A meets B every 7 days, 
			// 3+1 % 7= 4, 3+4%7 -> new dv is 7
			int mod = infectingday % frequency; 
			// while ( (infectingday+increment) % frequency != 0) 
			// 	++increment;
			
			if (mod != 0)
				frequency = infectingday + frequency - mod; 
			else 
				frequency = infectingday; 
			
			if (ppl[id].dv > frequency) 
			{
				ppl[id].dv = frequency; //if its a smaller dv then enter else don't bother
				heap.insert(Person3(id, frequency));
			}
		}
	}
	return day; 
}
