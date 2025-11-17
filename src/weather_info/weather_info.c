#include "weather_info.h"
#include "../apimeteo/apimeteo.h"
#include "../logger/logger.h"
#include <cjson/cJSON.h>
#include <string.h>
#include <time.h>


int current_hour() {
    time_t rawtime;
    struct tm *info;
    int current_hour;
    time(&rawtime);
    info = localtime(&rawtime);
    current_hour = info->tm_hour;
    return current_hour;
}

WeatherInfo get_city_weather(GeoCoordinates coord, NettResponse *res){


  WeatherInfo info = {0};
  char weather_location_url[1024];
  build_weather_location_query_uri(coord.latitude, coord.longitude, weather_location_url, sizeof(weather_location_url));
  g_print("Current url: %s\n", weather_location_url);
  nett_get(weather_location_url, NULL, res);
  if(!nett_ok(res->status_code) || res->contents == NULL) return info;
  cJSON *json = cJSON_Parse(res->contents);
  if(json == NULL) {
    LOGGER_ERROR("Error parsing json at get_city_weather.\n");
    return info;
  }

  cJSON *city_res = cJSON_IsArray(json) ? cJSON_GetArrayItem(json, 0) : json;


  snprintf(info.city_name, sizeof(info.city_name), "%s", coord.name);
  info.latitude = cJSON_GetObjectItem(city_res, "latitude")->valuedouble;
  info.longitude = cJSON_GetObjectItem(city_res, "longitude")->valuedouble;   
  info.elevation = cJSON_GetObjectItem(city_res, "elevation")->valuedouble;
  cJSON *current = cJSON_GetObjectItem(city_res, "current");
  snprintf(info.time, sizeof(info.time), "%s", cJSON_GetObjectItem(current, "time")->valuestring);
  info.is_day = cJSON_GetObjectItem(current, "is_day")->valueint == 1 ? true : false;
  info.wcode = (WMOWeatherCode)cJSON_GetObjectItem(current, "weather_code")->valueint;

  cJSON *daily_obj = cJSON_GetObjectItem(city_res, "daily");
  cJSON *sunrise_arr = cJSON_GetObjectItem(daily_obj, "sunrise");
  cJSON *sunrise_obj = cJSON_IsArray(sunrise_arr) ? cJSON_GetArrayItem(sunrise_arr, 0) : NULL;

  cJSON *sunset_arr = cJSON_GetObjectItem(daily_obj, "sunset");
  cJSON *sunset_obj = cJSON_IsArray(sunset_arr) ? cJSON_GetArrayItem(sunset_arr, 0) : NULL;


  if(sunrise_obj != NULL){
    printf("%s\n", sunrise_obj->valuestring);
    memcpy(info.sunrise, sunrise_obj->valuestring + 11, 5);
    info.sunrise[5] = '\0';
  }
  if(sunset_obj != NULL){
    memcpy(info.sunset, sunset_obj->valuestring + 11, 5);
    info.sunset[5] = '\0';
 
  }


  info.temperature = cJSON_GetObjectItem(current, "temperature_2m")->valuedouble;
  info.feels_like = cJSON_GetObjectItem(current, "apparent_temperature")->valuedouble;

  cJSON *precipitation_probability = cJSON_GetObjectItem(city_res, "precipitation_probability");
  cJSON *precipitation_arr = cJSON_IsArray(precipitation_probability) ? precipitation_probability : NULL;
  if(precipitation_arr != NULL) {

    cJSON *prob = cJSON_GetArrayItem(precipitation_arr, current_hour());
    if(prob != NULL && cJSON_IsNumber(prob)){ 
      info.chance_of_rain = (u8)prob->valueint;
    }
  }

  info.pressure = cJSON_GetObjectItem(current, "pressure_msl")->valuedouble;
  info.wind_speed = cJSON_GetObjectItem(current, "wind_speed_10m")->valuedouble;
  info.visibility = cJSON_GetObjectItem(current, "visibility")->valueint;
  cJSON_Delete(json);
  return info;  

}

