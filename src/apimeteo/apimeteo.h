#ifndef APIMETEO_H
#define APIMETEO_H 

#include <stdlib.h>
#include "../definitions/definitions.h"

#define METEO_CITY_METADATA_URI_FORMAT "https://geocoding-api.open-meteo.com/v1/search?name=%s"
#define METEO_LOCATION_INFO_URI_FORMAT "https://api.open-meteo.com/v1/forecast?latitude=%.2f&longitude=%.2f&current=temperature_2m,wind_speed_10m&hourly=temperature_2m,relative_humidity_2m,wind_speed_10m" 


void build_city_query_uri(const char *city_name, char *out, size_t out_size);
void build_weather_location_query_uri(f64 latitude, f64 longitude, char *out, size_t out_size);




#endif 
