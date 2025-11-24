#include "weather_assets.h"
#include <cjson/cJSON.h>

static char *assets_contents = NULL;
static cJSON *assets_icons_json = NULL;

void update_widgets_from_wmo_code(WMOWeatherCode code, bool is_day, GtkImage *img_widget, GtkLabel *description_widget){
    if (assets_contents == NULL){
        GError *error = NULL;
        GBytes *json_bytes =
            g_resources_lookup_data("/com/pluvia/assets/wmo/wmo_record.json",
                                    G_RESOURCE_LOOKUP_FLAGS_NONE,
                                    &error);
        if (json_bytes == NULL){
            g_warning("Failed to load wmo_record.json: %s", error->message);
            g_clear_error(&error);
            return;
        }

        gsize size;
        const char *data = g_bytes_get_data(json_bytes, &size);
        assets_contents = g_strndup(data, size);
        g_bytes_unref(json_bytes);
    }

    if (assets_icons_json == NULL){
        assets_icons_json = cJSON_Parse(assets_contents);
        if (!assets_icons_json){
            g_warning("Failed to parse wmo_record.json");
            return;
        }
    }

    char key[12];
    snprintf(key, sizeof(key), "%d", (int)code);
    cJSON *asset = cJSON_GetObjectItem(assets_icons_json, key);
    if (!asset)
        return;
    cJSON *inner = cJSON_GetObjectItem(asset, is_day ? "day" : "night");
    if (!inner)
        return;
    cJSON *img = cJSON_GetObjectItem(inner, "image");
    cJSON *desc = cJSON_GetObjectItem(inner, "description");

    if (!img || !desc)
        return;

    char resource_path[256];
    snprintf(resource_path, sizeof(resource_path),
             "%s", img->valuestring);

    gtk_image_set_from_resource(img_widget, resource_path);

    if (description_widget != NULL){
        gtk_label_set_text(description_widget, desc->valuestring);
    }
}
