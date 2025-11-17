#include "weather_assets.h"
#include <cjson/cJSON.h>
#include "../helpers/file_helpers.h"

static char *assets_contents = NULL;
static cJSON *assets_icons_json = NULL;


void update_widgets_from_wmo_code(WMOWeatherCode code, bool is_day, GtkImage *img_widget, GtkLabel *description_widget){  
  if(assets_contents == NULL){
   
   assets_contents = read_file_contents("../assets/wmo/wmo_record.json");
  }
  if(assets_icons_json == NULL){
   assets_icons_json = cJSON_Parse(assets_contents);
  }
  
  char key[12];
  snprintf(key, sizeof(key), "%d", (int)code);
  cJSON *asset = cJSON_GetObjectItem(assets_icons_json, key);
  if(asset != NULL){
   cJSON *inner = is_day ? cJSON_GetObjectItem(asset, "day") : cJSON_GetObjectItem(asset, "night");
  if(inner != NULL){
      cJSON *img = cJSON_GetObjectItem(inner, "image");
      cJSON *desc = cJSON_GetObjectItem(inner, "description");
      gtk_image_set_from_file(img_widget, img->valuestring);
      gtk_label_set_text(description_widget, desc->valuestring);
      return;
    }
  }

}
