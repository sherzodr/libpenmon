/*
 * libpenmon.h
 *
 *  Created on: Oct 9, 2020
 *      Author: sherzodr
 */

#ifndef LIBPENMON_H_
#define LIBPENMON_H_


const bool CHECK_RADIATION_RANGE = true;
const bool CHECK_SUNSHINE_HOURS_RANGE=true;

#include <math.h>

// forward declarations
class Station;
class StationDay;
// end forward declarations

class Crop {
public:
	int resistance_a;
	float albedo;
	float height;
	Crop(void);
};

class Climate {
public:
	bool interior_location;
	bool coastal_location;
	bool island_location;

	bool arid_climate;
	bool humid_climate;

	short dew_point_difference;
	float average_wind_speed;
	float k_rs = 0.16;

	Climate(void);
	explicit operator bool() const { return true; }

	void light_winds();
	void moderate_winds();
	void strong_winds();

	void arid();

	void humid();

	void interior();
	void coastal();
	void island();
};

class Station {
public:

	float latitude, latitude_radians;
	int altitude;
	short anemometer_height = 2;

	Crop ref_crop;
	Climate climate;

	explicit operator bool() const { return true; }
	Station(float lat, int alt);
	Station(float lat, int alt, short anem_height);
	StationDay get_day(short d);
};

class StationDay {

public:

	short day_number;
	float temp_min { }, temp_max { }, temp_mean { }, humidity_mean { },
			humidity_min { }, humidity_max { };
	float wind_speed { }, radiation_s { }, radiation_a { }, temp_dew { },
			temp_dry { }, temp_wet { };
	float vapour_pressure { }, sunshine_hours {-1 };
	const float stephen_boltzmann_constant = pow(4.903 * 10, -9.0);

	Station* station;

	StationDay(short d, Station* st);

	float wind_speed_2m(void);
	float dew_point(void);
	float atmospheric_pressure(void);
	float atmospheric_pressure_in_mm(void);
	float latent_heat_of_vapourization(void);
	double specific_heat(void);
	float psychrometric_constant(void);
	float Tmean(void);
	float saturation_vapour_pressure(float);
	float mean_saturation_vapour_pressure(void);
	float slope_of_saturation_vapour_pressure(float);
	float actual_vapour_pressure(void);
	float vapour_pressure_deficit(void);
	float relative_sun_distance(void);
	float solar_declination(void);
	float X(void);
	float sunset_hour_angle(void);
	float R_a(void);
	float R_a_in_mm(void);
	float daylight_hours(void);
	float solar_radiation(void);
	float solar_radiation_in_mm(void);
	float R_so(void);
	float R_ns(void);
	float R_nl(void);
	float net_radiation(void);
	float net_radiation_in_mm(void);
	float RH(float);
	float soil_heat_flux(void);
	float eto_hargreaves(void);
	float eto(void);

};

#endif /* LIBPENMON_H_ */
