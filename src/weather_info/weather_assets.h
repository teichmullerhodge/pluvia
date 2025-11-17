#ifndef WEATHER_ASSETS_H
#define WEATHER_ASSETS_H 

#include "weather_info.h"
#include <gtk/gtk.h>

void update_widgets_from_wmo_code(WMOWeatherCode code, bool is_day, GtkImage *img_widget, GtkLabel *description_widget);


#endif 
