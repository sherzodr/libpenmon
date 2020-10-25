# NAME

libpenmon - Implements weather station in C++ that supports ETo calculation. 

# SYNOPSIS

    #include "libpenmon.h"
    #include <iostream>
    
    int main(void) {
        Station station(41.42, 109);
        StationDay day238 = station.day(238, station);
        day238.temp_min=19.5;
        day238.temp_max=29.5;
        std::cout << "ETo is " << day238.eto() << "mm\n";
        return 0;
    }

# WARNING

This library is work-in-progress. Proceed with caution!

# INSTALL

    ./configure
    make
    make install
    
It also includes *example.cpp* file. You can compile the program by doing:

    make example
    
It will generate an executable file *example* in your current folder.

# LINKING WITH LIBPENMON

This is how I compile and link source code that uses *libpenmon*:

    g++ source_code.cpp -lpenmon
    
If compiler complains saying *-lpenmon* is not found, you should provide the 
library path where you installed it:

    g++ -L"your/lib/path" source_code.cpp -lpenmon
    
In case it cannot find *libpenmon.h* header file, direct it to the location
of the header file:

    g++ -I"your/include/path" -L"your/libpath" source_code.cpp -lpenmon 

# DESCRIPTION

Implements a weather station class (Station) that calculates 
ETo (reference evapotranspiration) given climate data. Based on 
[Penman-Monteith equation][1] as detailed in UN-FAO's 
"[Irrigation and Drainage Paper 56"][2].

# CLASSES

libpenmon implements few classes to accomplish it's mission as intuitively as 
possible:

# Station

This is the main class that holds other classes together. It can either
accept two arguments, *latitude* and *altitude* of the location:

    Station station_in_uzb(41.42, 109)
    
or three arguments, *latitude*, *altitude* and *anemometer_height*:

    Station station_in_uzb(41.42, 109, 10)
    
If the third argument is missing it defaults to *2*

# StationDay

This class represents a single entry related to the *Station*. All of the heavy-lifting
happens in this class. It's also responsiblel to calculate ETo:

    StationDay day138=station_in_uzb.get_day(138);
    day138.temp_min=26.5;
    day138.temp_max=38.5
    
    float eto = day138.eto();

# Crop

This class represents a crop, against which evapotranspiration is calculated. 
if you do not set any crop details library defaults to reference crop as
detailed in the [original paper][2].

# Client

Algorithms detailed in the [paper][2] are able to make assumptions regarding
the climate to recreate missing data. To make these assumptions it relies
on *Client* class. If you do not instantiate this class it creates a default climate
class.

# SEE ALSO

*libpenmon* started off as a port of [penmon.eto][3] Python module. You should
refer to that module to learn more about this library. Method and classnames
are preserved as is.

# AUTHOR

Sherzod B. Ruzmetov <sherzodr@gmail.com>


[1]: http://www.fao.org/3/x0490e/x0490e06.htm
[2]: http://www.fao.org/3/x0490e/x0490e00.htm
[3]: https://github.com/sherzodr/penmon
