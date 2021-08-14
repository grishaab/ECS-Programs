//Grisha Bandodkar 918511979
#ifndef SITENUMBER_H
#define SITENUMBER_H
#include "Runway.h"
class SiteNumber
{
    public:
        SiteNumber(std::string sn) : sitenum_(sn){}
        bool operator() (Runway* r) {return sitenum().compare(r->site_number()) == 0;}
        std::string sitenum(void) const {return sitenum_;} 
    private:
        const std::string sitenum_; 
};
#endif