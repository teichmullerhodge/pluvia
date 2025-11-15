#include "geo_coord.h"
#include <cjson/cJSON.h>
#include "../apimeteo/apimeteo.h"
#include "../logger/logger.h"
#include <stdio.h>

GeoCoordinates get_city_latitude_longitude(const char *city_name, NettResponse *res){

  GeoCoordinates coord = { "", 0.0f, 0.0f };
  char location_url[1024];
  build_city_query_uri(city_name, location_url, sizeof(location_url));
  nett_get(location_url, NULL, res);
  if(res->contents == NULL) return coord;
  cJSON *json = cJSON_Parse(res->contents);
  if(json == NULL) {
    LOGGER_ERROR("Error parsing json result.\n");
    return coord;
  }
  cJSON *results = cJSON_GetObjectItem(json, "results");
  if(!cJSON_IsArray(results)){
    LOGGER_ERROR("Results doesn't returned an array.\n");
    cJSON_Delete(json);
    return coord;
  }
  cJSON *item = cJSON_GetArrayItem(results, 0);
  cJSON *name = cJSON_GetObjectItem(item, "name");
  cJSON *latitude =  cJSON_GetObjectItem(item, "latitude");
  cJSON *longitude =  cJSON_GetObjectItem(item, "longitude");
  
  if (cJSON_IsNumber(latitude) && cJSON_IsNumber(longitude)) {
    coord.latitude = latitude->valuedouble;
    coord.longitude = longitude->valuedouble;
    snprintf(coord.name, sizeof(coord.name), "%s", name->valuestring);
    cJSON_Delete(json);
    return coord;
  }

  return coord;

}

