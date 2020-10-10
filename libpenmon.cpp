//============================================================================
// Name        : libpenmon.cpp
// Author      : Sherzod RUZMETOV
// Version     :
// Copyright   : 2020 Sherzod RUZMETOV
// Description : Implements Station, Day, Climate and Crop classes according to
//               Penman-Monteith equation
//============================================================================

#include "libpenmon.h"
#include <math.h>
#include <assert.h>
#include <iostream>

using namespace std;

bool is_set(float n) {
	return (n != -999.99);
}

bool is_set(int n) {
	return (n != -999);
}

Crop::Crop(void) {
	resistance_a = 208;
	albedo = 0.23;
	height = 0.12;
}

Climate::Climate(void) {

	interior_location = true;
	coastal_location = false;
	island_location = false;

	arid_climate = true;
	humid_climate = false;

	dew_point_difference = 2;
	average_wind_speed = 2.0;
}

void Climate::light_winds() {
	this->average_wind_speed = 0.5;
}
void Climate::moderate_winds() {
	this->average_wind_speed = 2.0;
}
void Climate::strong_winds() {
	this->average_wind_speed = 4.0;
}

void Climate::arid() {
	this->arid_climate = true;
	this->humid_climate = false;
	this->dew_point_difference = 2;
}

void Climate::humid() {
	this->arid_climate = false;
	this->humid_climate = true;
	this->dew_point_difference = 0;
}

void Climate::interior() {
	this->interior_location = true;
	this->coastal_location = false;
	this->island_location = false;
	this->k_rs = 0.16;
}

void Climate::coastal() {
	this->interior_location = false;
	this->coastal_location = true;
	this->island_location = false;
	this->k_rs = 0.19;
}

void Climate::island() {
	this->interior_location = false;
	this->coastal_location = false;
	this->island_location = true;
	this->k_rs = 0.19;
}

Station::Station(float lat, int alt) {

	this->latitude = lat;
	this->altitude = alt;
	this->latitude_radians = ( M_PI / 180) * this->latitude;
	this->ref_crop = Crop();
	this->climate = Climate();
}

Station::Station(float lat, int alt, short anem_height) {
	this->latitude = lat;
	this->altitude = alt;
	this->anemometer_height = anem_height;
	this->latitude_radians = (M_PI / 180) * this->latitude;
	this->ref_crop = Crop();
	this->climate = Climate();
}

StationDay Station::get_day(short d) {

	StationDay day(d, this);

	return day;
}

StationDay::StationDay(short d, Station *st) {
	this->day_number = d;
	this->station = st;
}

float StationDay::wind_speed_2m() {

	if (this->wind_speed && (this->station->anemometer_height == 2)) {
		return this->wind_speed;
	}

	if (this->wind_speed && (this->station->anemometer_height != 2)) {
		float converted_wind_speed =
				this->wind_speed
						* (4.87
								/ log(
										(67.8 * this->station->anemometer_height)
												- 5.42));

		return converted_wind_speed;
	}

	if (this->station->climate) {
		return this->station->climate.average_wind_speed;
	}
	return -1.0;
}

float StationDay::dew_point() {
	if (this->temp_dew) {
		return this->temp_dew;
	}
	if (this->temp_min && this->station->climate.dew_point_difference) {
		return this->temp_min - this->station->climate.dew_point_difference;
	}
	return false;
}

float StationDay::atmospheric_pressure() {
	return 101.3 * pow((293 - (0.0065 * this->station->altitude)) / 293, 5.26);
}

float StationDay::atmospheric_pressure_in_mm() {
	return this->atmospheric_pressure() * 7.50;
}

float StationDay::latent_heat_of_vapourization() {
	return 2.45;
}

float StationDay::specific_heat() {
	return 1.013 * pow(10, -3);
}

float StationDay::psychrometric_constant() {
	return 0.665 * pow(10, -3) * this->atmospheric_pressure();
}

float StationDay::Tmean() {

	if (this->temp_mean) {
		return this->temp_mean;
	}
	if (this->temp_max && this->temp_min) {
		return (this->temp_max + this->temp_min) / 2;
	}
	return false;
}

float StationDay::saturation_vapour_pressure(float T) {
	return 0.6108 * pow(2.7183, ((17.27 * T) / (T + 237.3)));
}

float StationDay::mean_saturation_vapour_pressure() {
	if (this->temp_max && this->temp_min) {
		float vp_max = this->saturation_vapour_pressure(this->temp_max);
		float vp_min = this->saturation_vapour_pressure(this->temp_min);
		return (vp_max + vp_min) / 2;
	}

	if (this->temp_mean) {
		return this->saturation_vapour_pressure(this->temp_mean);
	}
	return false;
}

float StationDay::slope_of_saturation_vapour_pressure(float T) {
	return 4098 * 0.6108 * pow(2.7183, (17.27 * T) / (T + 237.3))
			/ pow((T + 237.3), 2);
}

float StationDay::actual_vapour_pressure() {
	if (this->vapour_pressure) {
		return this->vapour_pressure;
	}
	if (this->temp_dry && this->temp_wet) {
		float vp_wet = this->saturation_vapour_pressure(this->temp_wet);
		float psychrometric_constant = this->psychrometric_constant();
		return vp_wet
				- (psychrometric_constant * (this->temp_dry - this->temp_wet));
	}
	if (this->humidity_max && this->humidity_min && this->temp_max
			&& this->temp_min) {
		float vp_min = this->saturation_vapour_pressure(this->temp_min);
		float vp_max = this->saturation_vapour_pressure(this->temp_max);
		return (vp_min * (this->humidity_max / 100)
				+ vp_max * (this->humidity_min / 100)) / 2;
	}
	if (this->humidity_max && this->temp_min) {
		float vp_min = this->saturation_vapour_pressure(this->temp_min);
		return vp_min * (this->humidity_max / 100);
	}
	if (this->humidity_mean && this->temp_max && this->temp_min) {
		float vp_min = this->saturation_vapour_pressure(this->temp_min);
		float vp_max = this->saturation_vapour_pressure(this->temp_max);
		return (this->humidity_mean / 100) * ((vp_max + vp_min) / 2);
	}
	if (this->dew_point()) {
		return this->saturation_vapour_pressure(this->dew_point());
	}
	return false;
}

float StationDay::vapour_pressure_deficit() {

	if (this->temp_min && this->temp_max) {
		float vp_min = this->saturation_vapour_pressure(this->temp_min);
		float vp_max = this->saturation_vapour_pressure(this->temp_max);
		float vp_actual = this->actual_vapour_pressure();
		return ((vp_min + vp_max) / 2) - vp_actual;
	}
	return false;
}

float StationDay::relative_sun_distance() {
	return (1 + (0.033 * cos((2 * M_PI / 365) * this->day_number)));
}

float StationDay::solar_declination() {
	return 0.409 * sin(((2 * M_PI / 365) * this->day_number) - 1.39);
}

float StationDay::X() {
	float x = 1
			- (tan(this->station->latitude_radians)
					* tan(this->solar_declination()));
	if (x <= 0) {
		x = 0.00001;
	}
	return x;
}

float StationDay::sunset_hour_angle() {
	return acos(
			-1 * tan(this->station->latitude_radians)
					* tan(this->solar_declination()));
}

float StationDay::R_a() {
	return (24 * 60 / M_PI * 0.0820 * this->relative_sun_distance()
			* this->sunset_hour_angle() * sin(this->station->latitude_radians)
			* sin(this->solar_declination()))
			+ (cos(this->station->latitude_radians)
					* cos(this->solar_declination())
					* sin(this->sunset_hour_angle()));
}

float StationDay::R_a_in_mm() {
	return this->R_a() * 0.408;
}

float StationDay::daylight_hours() {
	return (24 / M_PI) * this->sunset_hour_angle();
}

float StationDay::R_so() {
	int alt = this->station->altitude;
	if (alt < 100) {
		return (0.25 + 0.50) * this->R_a();
	}
	return (0.75 + (pow(210, -5) * alt)) * this->R_a();
}

float StationDay::solar_radiation() {
	if (this->radiation_s) {
		if (CHECK_RADIATION_RANGE && (this->radiation_s > this->R_so())) {
			throw("RADIATION_RANGE");
		}
		return this->radiation_s;
	}

	// IMPORTANT: sunshine_hours will be negative if it's not initialized
	float n = this->sunshine_hours;

	if (n < 0) {
		// it means sunshine_hours is NOT set
		if (this->station->climate && this->station->climate.island_location) {
			return (0.7 * this->R_a()) - 4;
		}

		if (this->station->climate and this->temp_min && this->temp_max) {
			float krs = 0.16;
			if (this->station->climate.coastal_location) {
				krs = 0.16;
			}
			return krs * sqrt(this->temp_max - this->temp_min) * this->R_a();
		}
		n = this->daylight_hours();
	} // n < 0

	if (CHECK_SUNSHINE_HOURS_RANGE && (n > this->daylight_hours())) {
		throw("SUNSHINE_RANGE");
	}

	return (0.25 + (0.50 * (n / this->daylight_hours()))) * this->R_a();
}

float StationDay::solar_radiation_in_mm() {
	return (this->solar_radiation() * 0.408);
}

float StationDay::R_ns() {
	return (1 - this->station->ref_crop.albedo) * this->solar_radiation();
}

float StationDay::R_nl() {

	float TmaxK = this->temp_max + 273.16;
	float TminK = this->temp_min + 273.16;
	float ea = this->actual_vapour_pressure();
	float rs = this->solar_radiation();
	float rso = this->R_so();
	float sbc = this->stephen_boltzmann_constant;

	return ((pow(TmaxK, 4) + pow(TminK, 4)) / 2) * (0.34 - (0.14 * sqrt(ea)))
			* ((1.35 * (rs / rso)) - 0.35);
}

float StationDay::net_radiation() {
	if (this->R_ns() && this->R_nl()) {
		return this->R_ns() - this->R_nl();
	}
	return false;
}

float StationDay::net_radiation_in_mm() {
	if (this->net_radiation()) {
		return this->net_radiation() * 0.408;
	}
	return false;
}

float StationDay::RH(float T) {
	return 100
			* (this->actual_vapour_pressure()
					/ this->saturation_vapour_pressure(T));
}

float StationDay::soil_heat_flux() {
	return 0.00;
}

float StationDay::eto_hargreaves() {
	assert(this->temp_max && this->temp_min);

	float Tmean = (this->temp_max + this->temp_min) / 2;
	return (Tmean + 17.8) * pow((this->temp_max - this->temp_min), 0.5)
			* this->R_a();
}

float StationDay::eto() {

	if (this->wind_speed_2m() < 0) {
		return this->eto_hargreaves();
	}

	float Tmax = this->temp_max;
	float Tmin = this->temp_min;
	float Tmean = this->temp_mean;

	if (Tmax && Tmin) {
		Tmean = (Tmax + Tmin) / 2;
	}

	float slope = this->slope_of_saturation_vapour_pressure(Tmean);
	float net_radiation = this->net_radiation();
	float G = this->soil_heat_flux();
	float u2m = this->wind_speed_2m();

	cout << "Net radiation is " << net_radiation << endl;

	float nominator = (0.408 * slope * (net_radiation - G))
			+ (this->psychrometric_constant() * (900 / (Tmean + 273)) * u2m
					* this->vapour_pressure_deficit());
	float denominator = slope
			+ (this->psychrometric_constant() * (1 + (0.34 * u2m)));

	return nominator / denominator;
}

