/* * main.cpp
 *
 *  	Created on: Oct 9, 2020
 *      Author: sherzodr
 */

#include <iostream>
#include "libpenmon.h"
#include <assert.h>
#include <string>
#include <stdio.h>
#include <string.h>

using namespace std;

void testEq(float a, float b, string message) {

	char a_str[50];
	char b_str[50];

	sprintf(a_str, "%.3f", a);
	sprintf(b_str, "%.3f", b);

	if (strcmp(a_str, b_str) == 0) {
		cout << message << "... \e[1;92mPASSED\e[0m";
	} else {
		cout << message << "... \e[1;31mNOT PASSED. Expected " << b_str
				<< ". Received  " << a_str << "\e[0m";
	}
	cout << "\n";
}

void testTrue(bool param, string message) {
	if (param == true) {
		cout << message << "... \e[1;92mPASSED\e[0m";
	} else {
		cout << message << "... \e[1;31mNOT PASSED\e[0m";
	}
	cout << endl;
}

int main(void) {

	Station station(41.42, 109);
	StationDay day = station.get_day(130);

	testEq(station.latitude, 41.42, "testing for latitude");
	testEq(station.altitude, 109.00, "testing for altitude");
	testEq(station.anemometer_height, 2.0, "anemometer height");
	testEq(day.day_number, 130, "day_number");

	Station station_1080(41.42, 1800);
	StationDay day_130 = station_1080.get_day(130);
	testEq(day_130.atmospheric_pressure(), 81.756, "atmospheric pressure");
	testEq(day_130.psychrometric_constant(), 0.054397,
			"psychrometric_constant");
	testEq(day.specific_heat(), pow(1.013 * 10, -3), "specific heat");
	testEq(day.latent_heat_of_vapourization(), 2.45,
			"latent_heat_of_vapourziation");

	day.temp_max = 24.5;
	day.temp_min = 15;
	testEq(day.mean_saturation_vapour_pressure(), 2.39,
			"mean saturation vapour pressure");

	day.temp_dew = 19.5;
	testEq(day.RH(35), 40.316, "RH at 35 degrees");

//	Station station2(41.42, 109);
//	StationDay day2=station2.get_day(130);
//	day2.temp_mean=19.75;

	day.temp_max = false;
	day.temp_min = false;
	day.temp_dew = false;
	day.temp_mean = 19.75;
	testEq(day.mean_saturation_vapour_pressure(), 2.302,
			"mean saturation vapour pressure ");

	day.temp_mean = false;
	testEq(day.slope_of_saturation_vapour_pressure(24.5), 0.183837,
			"slove of vapour pressure curve");

	// actual pscychrometric vapour pressure

	station.altitude = 1200;
	day.temp_wet = 19.5;
	day.temp_dry = 25.6;
	testEq(day.atmospheric_pressure(), 87.9, "atmospheric pressure");
	testEq(day.saturation_vapour_pressure(19.5), 2.267,
			"saturation vapour pressure");
	testTrue(day.temp_dew != true, "Temp dew temperature is NOT set");
	testEq(day.actual_vapour_pressure(), 1.91, "Actual vapour pressure");

	day.temp_dew = 17.0;
	day.temp_wet = false;
	day.temp_dry = false;

	testEq(day.temp_dew, 17.0, "Dew temp is set");
	testEq(day.actual_vapour_pressure(), 1.938, "Actual vapour pressure");

	day.temp_dew = false;
	day.temp_min = 18;
	day.temp_max = 25;
	day.humidity_max = 82;
	day.humidity_min = 54;

	testEq(day.saturation_vapour_pressure(18), 2.064,
			"saturation vapour pressure");
	testEq(day.saturation_vapour_pressure(25), 3.168,
			"saturation vapour pressure");
	testEq(day.actual_vapour_pressure(), 1.702, "actual vapour pressure");

	day.temp_min = 18;
	day.humidity_max = 82;
	day.temp_dew = false;
	day.temp_max = false;
	day.humidity_min = false;

	testEq(day.actual_vapour_pressure(), 1.692, "actual vapour pressure");

	day.humidity_min = false;
	day.temp_dew = false;
	day.humidity_max = false;
	day.temp_min = 18;
	day.temp_max = 25;
	day.humidity_mean = 68;

	testEq(day.actual_vapour_pressure(), 1.779,
			"actual vapour pressure is 1.779");

	day.humidity_mean = false;
	day.temp_min = 18;
	day.temp_max = 25;
	day.humidity_max = 82;
	day.humidity_min = 54;

	testEq(day.vapour_pressure_deficit(), 0.914,
			"determining vapour pressure deficit");

	Station station_bangkok(13.73, 1200);
	testEq(station_bangkok.latitude_radians, 0.240, "latitude radians");

	Station station_rio(22.90, 1200);
	testEq(station_rio.latitude_radians, 0.400, "latitude radians");

	StationDay day_246 = station.get_day(246);
	testEq(day_246.relative_sun_distance(), 0.985, "relative sun distance");

	testEq(day_246.solar_declination(), 0.120, "solar declination");

	Station south_station(-20.0, 1200);
	StationDay south_day = south_station.get_day(246);
	testEq(south_day.sunset_hour_angle(), 1.527, "sunset hour angle");

	return 0;
}

// test
