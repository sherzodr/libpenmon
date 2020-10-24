/* * main.cpp
 *
 *  	Created on: Oct 9, 2020
 *      Author: sherzodr
 */

#include <iostream>
#include "libpenmon.h"
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
		//cout << message << "... PASSED";
	} else {
		cout << message << "... \e[1;31mNOT PASSED. Expected " << b_str << ". Received  " << a_str << "\e[0m";
		//cout << message << "... NOT PASSED. Expected " << b_str
		//		<< ". Received  " << a_str;
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
	testEq(day.atmospheric_pressure(), 87.897, "atmospheric pressure");
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
	testEq(south_day.R_a(), 32.194, "R_a");
	testEq(south_day.R_a_in_mm(), 13.135, "R_a_in_mm");
	testEq(south_day.daylight_hours(), 11.666, "daylight hours");

	Station south_station2(-22.90, 1200);
	StationDay day135 = south_station2.get_day(135);
	day135.sunshine_hours = 7.10;
	testEq(day135.solar_radiation(), 14.46, "solar radiation");
	testEq(day135.R_so(), 18.833, "clear sky radiation");

	testEq(day135.R_ns(), 11.134, "net shortwave radiation");

	day135.temp_min=25.1;
	day135.temp_max=19.1;
	day135.vapour_pressure=2.1;
	testEq(day135.actual_vapour_pressure(), 2.1, "actual vapour pressure");
	day135.sunshine_hours=7.1;
	testEq(day135.R_nl(), 3.51, "Net longwave raditaion");
	testEq(day135.net_radiation(), 7.624, "net radiation");

	day135.wind_speed=5;
	testEq(day135.wind_speed_2m(), 5, "wind speed" );

	day135.station->anemometer_height=10;
	day135.wind_speed=3.2;
	testEq(day135.wind_speed_2m(), 2.393, "wind_speed");

	Station qarmish(41.42, 109);
	StationDay day150 = qarmish.get_day(150);
	day150.temp_min = 19.5;
	day150.temp_max = 36.50;
	day150.wind_speed = 2;
	testEq(day150.eto(), 6.98, "eto()");

	return 0;
}

// test
