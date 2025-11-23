#include "geo_coord.h"
#include <cjson/cJSON.h>
#include "../apimeteo/apimeteo.h"
#include "../logger/logger.h"
#include <stdio.h>
#include <curl/curl.h>

GeoCoordinates get_city_latitude_longitude(const char *city_name, NettResponse *res){
  char *encoded = curl_easy_escape(NULL, city_name, 0);
  GeoCoordinates coord = { "", 0.0f, 0.0f };
  char location_url[1024];
  build_city_query_uri(encoded, location_url, sizeof(location_url));
  nett_get(location_url, NULL, res);
  if(res->contents == NULL || !nett_ok(res->status_code)) return coord;
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
  cJSON *state = cJSON_GetObjectItem(item, "admin1");
  cJSON *country_code = cJSON_GetObjectItem(item, "country_code");
  cJSON *latitude =  cJSON_GetObjectItem(item, "latitude");
  cJSON *longitude =  cJSON_GetObjectItem(item, "longitude");
  
  if (cJSON_IsNumber(latitude) && cJSON_IsNumber(longitude)) {
    coord.latitude = latitude->valuedouble;
    coord.longitude = longitude->valuedouble;
    snprintf(coord.name, sizeof(coord.name), "%s - %s | %s", name->valuestring, state->valuestring, country_code->valuestring);
    cJSON_Delete(json);
    return coord;
  }

  return coord;

}

