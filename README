# NAME

libpenmon - Implementation of Penman-Monteith equation in C++

# WARNING

this library is work-in-progress. Proceed with caution!

# DESCRIPTION

Implements a weather station class (Station) that calculates 
ETo (reference evapotranspiration) given climate data.

# SYNOPSIS

    #include "libpenmon.h"
    #include <iostream>
    
    int main(void) {
        Station station(41.42, 109);
        StationDay day(238, station);
        day.temp_min=19.5;
        day.temp_max=29.5;
        std::cout << "ETo is " << day.eto() << "mm\n";
        return 0;
    }

# AUTHOR

Sherzod B. Ruzmetov <sherzodr@gmail.com>

