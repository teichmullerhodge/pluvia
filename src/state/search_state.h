#ifndef SEARCH_STATE_H
#define SEARCH_STATE_H 
#include <gtk/gtk.h>
#include "../weather_info/weather_info.h"


typedef struct {

    char query[100];
    WeatherUI *ui;
    bool cancelled;
    CityGeoData cities_result[CITIES_STORE_MAX_ELEMENTS];

} QueryData;

gboolean debounce_search(gpointer data);
void on_city_changed(GtkEntry *e, gpointer data);
void on_city_entry_changed(GtkEditable *editable, gpointer data);


#endif 
