#ifndef WEATHER_INFO_H 
#define WEATHER_INFO_H 

#include "../definitions/definitions.h"
#include "../geo_coord/geo_coord.h"
#include "../nett/nett.h"



typedef struct {

  f64 latitude;
  f64 longitude;
  f64 generationtime_ms;
  u64 utc_offset_seconds;
  u64 elevation;
  f64 temperature;
  f64 wind_speed;
  char time[32];

} WeatherInfo;

WeatherInfo get_city_weather(GeoCoordinates coord, NettResponse *res);



#endif 
