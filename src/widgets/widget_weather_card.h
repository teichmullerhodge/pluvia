#ifndef WIDGET_WEATHER_CARD_H
#define WIDGET_WEATHER_CARD_H 



#include <gtk/gtk.h>

GtkWidget *widget_weather_card(const char *label, const char* value, const char *icon_or_path, bool use_icon);
GtkLabel *get_label_val_from_card(GtkWidget *weather_card);


#endif 
