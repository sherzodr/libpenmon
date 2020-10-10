/*
 * main.cpp
 *
 *  	Created on: Oct 9, 2020
 *      Author: sherzodr
 */

#include <iostream>
#include "libpenmon.h"
#include <assert.h>


using namespace std;

int main(void) {

	Station station(41.42, 109);
	StationDay day = station.get_day(150);

	cout << day.R_a() << "\n";

	return 0;
}


// test
