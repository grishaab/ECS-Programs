#ifndef CLOSER_H
#define CLOSER_H
#include "Facility.h"
class Closer 
{
    public:
        Closer(double currlat, double currlon) : latitude(currlat), longitude(currlon){}
        bool operator()(Facility* a, Facility* b){ return a->distance(latitude, longitude) < b->distance(latitude, longitude); }
    private:
        const double latitude; 
        const double longitude; 
};
#endif
