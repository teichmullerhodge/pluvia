#include "apimeteo.h"
#include <stdio.h>

void build_city_query_uri(const char *city_name, char *out, size_t out_size){
  snprintf(out, out_size, METEO_CITY_METADATA_URI_FORMAT, city_name);
}

void format_coord(char *out, size_t out_size, double value) {
    char temp[64];
    snprintf(temp, sizeof(temp), "%.2f", value);

    for (char *p = temp; *p; p++) {
        if (*p == ',')
            *p = '.';
    }

    snprintf(out, out_size, "%s", temp);
}

void build_weather_location_query_uri(f64 latitude, f64 longitude, char *out, size_t out_size){
  
  char lat_str[64];
  char lon_str[64];

  format_coord(lat_str, sizeof(lat_str), latitude);
  format_coord(lon_str, sizeof(lon_str), longitude);
  snprintf(out, out_size, METEO_LOCATION_INFO_URI_FORMAT, lat_str, lon_str);
}
