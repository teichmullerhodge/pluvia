#include "search_state.h"
#include "../nett/nett.h"
#include "../geo_coord/geo_coord.h"
#include "../weather_info/weather_info.h"


gboolean debounce_search(gpointer data) {
    WeatherUI *ui = data;
    
    const char *city = gtk_editable_get_text(GTK_EDITABLE(ui->search_entry));
    
    NettResponse *res = response_init();
    GeoCoordinates geo = get_city_latitude_longitude(city, res);
    if(geo.latitude == 0.0f || geo.longitude == 0.0f) return false;
    WeatherInfo info = get_city_weather(geo, res);

    char temperature_c[50];
    snprintf(temperature_c, sizeof(temperature_c), "%d°", (i32)info.temperature);

    gtk_label_set_text(ui->current_temperature_label, temperature_c);
    gtk_label_set_text(ui->city_label, geo.name);

    destroy_response(res);

    return G_SOURCE_REMOVE;
}

void on_city_changed(GtkEditable *e, gpointer data) {
    (void)e;
    static guint timeout_id = 0;

    if (timeout_id != 0)
        g_source_remove(timeout_id);

    timeout_id = g_timeout_add(500, debounce_search, data);
}


