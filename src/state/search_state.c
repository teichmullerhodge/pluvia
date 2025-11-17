#include "search_state.h"
#include "../nett/nett.h"
#include "../geo_coord/geo_coord.h"
#include "../weather_info/weather_info.h"
#include "../weather_info/weather_assets.h"




void set_loading_ui(WeatherUI *ui, bool loading) {

  if(loading){

    gtk_widget_add_css_class(GTK_WIDGET(ui->city_label), "shimmer");
    gtk_widget_add_css_class(GTK_WIDGET(ui->current_temperature_label), "shimmer");
    gtk_widget_add_css_class(GTK_WIDGET(ui->sunrise_label), "shimmer");
    gtk_widget_add_css_class(GTK_WIDGET(ui->sunset_label), "shimmer");
    gtk_widget_add_css_class(GTK_WIDGET(ui->chance_of_rain_label), "shimmer");
    gtk_widget_add_css_class(GTK_WIDGET(ui->pressure_label), "shimmer");
    gtk_widget_add_css_class(GTK_WIDGET(ui->wind_label), "shimmer");
    gtk_widget_add_css_class(GTK_WIDGET(ui->feels_like_label), "shimmer");
    gtk_widget_add_css_class(GTK_WIDGET(ui->visibility_label), "shimmer");
    gtk_widget_add_css_class(GTK_WIDGET(ui->weather_description_label), "shimmer");
    gtk_widget_add_css_class(GTK_WIDGET(ui->severe_alert_label), "shimmer");
    return;

  }

    gtk_widget_remove_css_class(GTK_WIDGET(ui->city_label), "shimmer");
    gtk_widget_remove_css_class(GTK_WIDGET(ui->current_temperature_label), "shimmer");
    gtk_widget_remove_css_class(GTK_WIDGET(ui->sunrise_label), "shimmer");
    gtk_widget_remove_css_class(GTK_WIDGET(ui->sunset_label), "shimmer");
    gtk_widget_remove_css_class(GTK_WIDGET(ui->chance_of_rain_label), "shimmer");
    gtk_widget_remove_css_class(GTK_WIDGET(ui->pressure_label), "shimmer");
    gtk_widget_remove_css_class(GTK_WIDGET(ui->wind_label), "shimmer");
    gtk_widget_remove_css_class(GTK_WIDGET(ui->feels_like_label), "shimmer");
    gtk_widget_remove_css_class(GTK_WIDGET(ui->visibility_label), "shimmer");
    gtk_widget_remove_css_class(GTK_WIDGET(ui->weather_description_label), "shimmer"); 
    gtk_widget_remove_css_class(GTK_WIDGET(ui->severe_alert_label), "shimmer");
}


void debounce_search_task(GTask *task, gpointer source_object, gpointer task_data, GCancellable *cancellable){
    (void)source_object;
    (void)cancellable;
    WeatherUI *ui = task_data;

    const char *city = gtk_editable_get_text(GTK_EDITABLE(ui->search_entry));

    NettResponse *res = response_init();
    GeoCoordinates geo = get_city_latitude_longitude(city, res);

    if (geo.latitude == 0.0f || geo.longitude == 0.0f) {
        destroy_response(res);
        g_task_return_pointer(task, NULL, NULL);
        set_loading_ui(ui, false);
        return;
    }

    WeatherInfo *info = g_new0(WeatherInfo, 1);
    *info = get_city_weather(geo, res);

    destroy_response(res);
    g_task_return_pointer(task, info, g_free);
}


void debounce_search_done(GObject *source, GAsyncResult *result, gpointer user_data){
    (void)source;
    WeatherUI *ui = user_data;

    GTask *task = G_TASK(result);
    WeatherInfo *info = g_task_propagate_pointer(task, NULL);

    if (!info) {
        set_loading_ui(ui, false);
        return;
    }

    char temperature_c[50];
    snprintf(temperature_c, sizeof(temperature_c), "%d°", (int)info->temperature);

    gtk_label_set_text(ui->current_temperature_label, temperature_c);
    
    gtk_label_set_text(ui->city_label, info->city_name);
    
    char chance_of_rain[16];
    snprintf(chance_of_rain, sizeof(chance_of_rain), "%d%%", info->chance_of_rain);
    gtk_label_set_text(ui->chance_of_rain_label, chance_of_rain);
    
    char feels_like_c[50];
    snprintf(feels_like_c, sizeof(feels_like_c), "%d°", (int)info->feels_like);
    gtk_label_set_text(ui->feels_like_label, feels_like_c);

    char pressure_c[50];
    snprintf(pressure_c, sizeof(pressure_c), "%d mb", (int)info->pressure); 
    gtk_label_set_text(ui->pressure_label, pressure_c);
     
    char wind_c[50];
    snprintf(wind_c, sizeof(wind_c), "%d km/h", (int)info->wind_speed); 
    gtk_label_set_text(ui->wind_label, wind_c);
  
    char visibility_c[50];
    snprintf(visibility_c, sizeof(visibility_c), "%ld km", (u64)(info->visibility / 1000));
    gtk_label_set_text(ui->visibility_label, visibility_c);


    update_widgets_from_wmo_code(info->wcode, info->is_day, ui->weather_image, ui->weather_description_label); 


    gtk_label_set_text(ui->sunrise_label, info->sunrise);
    gtk_label_set_text(ui->sunset_label, info->sunset);

 
    set_loading_ui(ui, false);
    g_free(info);
}



gboolean debounce_search(gpointer data){
    WeatherUI *ui = data;
    set_loading_ui(ui, true); 

   
    GTask *task = g_task_new(NULL, NULL, debounce_search_done, ui);
    g_task_set_task_data(task, ui, NULL);
    g_task_run_in_thread(task, debounce_search_task);
    g_object_unref(task);
    

    return G_SOURCE_REMOVE;
}




void on_city_changed(GtkEntry *e, gpointer data) {
    (void)e;
    debounce_search(data);
}


