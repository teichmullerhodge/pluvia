#ifndef APIMETEO_H
#define APIMETEO_H 

#include <stdlib.h>
#include "../definitions/definitions.h"

#define METEO_CITY_METADATA_URI_FORMAT "https://geocoding-api.open-meteo.com/v1/search?name=%s"
#define METEO_LOCATION_INFO_URI_FORMAT "https://api.open-meteo.com/v1/forecast?latitude=%s&longitude=%s&current=weather_code,is_day,temperature_2m,apparent_temperature,pressure_msl,wind_speed_10m,visibility&hourly=precipitation_probability&daily=sunrise,sunset&timezone=auto" 


void build_city_query_uri(const char *city_name, char *out, size_t out_size);
void build_weather_location_query_uri(f64 latitude, f64 longitude, char *out, size_t out_size);




#endif 
