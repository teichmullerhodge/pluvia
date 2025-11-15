#include "apimeteo.h"
#include <stdio.h>

void build_city_query_uri(const char *city_name, char *out, size_t out_size){
  snprintf(out, out_size, METEO_CITY_METADATA_URI_FORMAT, city_name);
}
void build_weather_location_query_uri(f64 latitude, f64 longitude, char *out, size_t out_size){
  snprintf(out, out_size, METEO_LOCATION_INFO_URI_FORMAT, latitude, longitude);
}
