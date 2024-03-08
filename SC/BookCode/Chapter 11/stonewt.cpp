// 程序清单11.17 P412
// stonewt.cpp -- Stonewt methods
#include <iostream>
#include "stonewt.h"

using namespace std;

// constructor Stonewt object from double value
Stonewt::Stonewt(double lbs)
{
    stone = int(lbs) / Lbs_per_stn; // integer division
    pds_left = int(lbs) % Lbs_per_stn + lbs - int(lbs);
    pounds = lbs;
}

// constructor Stonewt object frome stone, double values
Stonewt::Stonewt(int stn, double lbs)
{
    stone = stn;
    pds_left = lbs;
    pounds = stn * Lbs_per_stn + lbs;
}

Stonewt::Stonewt() // default constructor, wt = 0
{
    stone = pounds = pds_left = 0;
}

Stonewt::~Stonewt() // destructor
{
}
// show weight in stones
void Stonewt::show_btn() const
{
    cout << stone << " stone, " << pds_left << " pounds\n";
}

// show weight in pounds
void Stonewt::show_lbs() const
{
    cout << pounds << " pounds\n";
}