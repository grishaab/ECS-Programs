//Grisha Bandodkar 918511979
#include "Runway.h"
#include <iostream>
using namespace std; 

Runway::Runway(string s) : site_number_(s.substr(0,10)), name_(s.substr(13, 7)), length_(convert_length(s.substr(20,5)))
{}

int Runway::convert_length(string s) const
{
    return stoi(s); 
}

string Runway::site_number(void) const
{
    return site_number_;
}

string Runway::name(void) const
{
    return name_;
}

int Runway::length(void) const
{
    return length_;
}