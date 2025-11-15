#include "weather_info.h"
#include "../apimeteo/apimeteo.h"
#include "../logger/logger.h"
#include <cjson/cJSON.h>
#include <string.h>

WeatherInfo get_city_weather(GeoCoordinates coord, NettResponse *res){


  WeatherInfo info = {0};
  char weather_location_url[1024];
  build_weather_location_query_uri(coord.latitude, coord.longitude, weather_location_url, sizeof(weather_location_url));
  nett_get(weather_location_url, NULL, res);
  cJSON *json = cJSON_Parse(res->contents);
  if(json == NULL) {
    LOGGER_ERROR("Error parsing json at get_city_weather.\n");
    return info;
  }

  info.latitude = cJSON_GetObjectItem(json, "latitude")->valuedouble;
  info.longitude = cJSON_GetObjectItem(json, "longitude")->valuedouble;
  info.generationtime_ms = cJSON_GetObjectItem(json, "generationtime_ms")->valueint;
  info.utc_offset_seconds = cJSON_GetObjectItem(json, "utc_offset_seconds")->valueint;

  info.elevation = cJSON_GetObjectItem(json, "elevation")->valuedouble;
  cJSON *current = cJSON_GetObjectItem(json, "current");
  info.temperature = cJSON_GetObjectItem(current, "temperature_2m")->valuedouble;
  info.wind_speed = cJSON_GetObjectItem(current, "wind_speed_10m")->valuedouble;
  strcpy(info.time, cJSON_GetObjectItem(current, "time")->valuestring);
  return info;  

}

