#include "search_state.h"
#include "../nett/nett.h"
#include "../geo_coord/geo_coord.h"
#include "../weather_info/weather_info.h"
#include "../weather_info/weather_assets.h"
#include "../layout/layout.h"
#include "../nett/nett_utils.h"
#include "../geo_store/geo_store.h"
#include "../logger/logger.h"

static CityGeoData cities_store[CITIES_STORE_MAX_ELEMENTS];
GCancellable *current_city_cancellable = NULL;


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
    gtk_widget_add_css_class(GTK_WIDGET(ui->weather_image), "shimmer");
    for(size_t k = 0; k < FORECAST_HOURS_DAY; k++){
      gtk_widget_add_css_class(GTK_WIDGET(ui->forecast_box_hour_label[k]), "shimmer");
      gtk_widget_add_css_class(GTK_WIDGET(ui->forecast_box_images[k]), "shimmer");
      gtk_image_clear(ui->forecast_box_images[k]);
    }
    gtk_image_clear(ui->weather_image);

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
    gtk_widget_remove_css_class(GTK_WIDGET(ui->weather_image), "shimmer");
    for(size_t k = 0; k < FORECAST_HOURS_DAY; k++){
      gtk_widget_remove_css_class(GTK_WIDGET(ui->forecast_box_hour_label[k]), "shimmer");
      gtk_widget_remove_css_class(GTK_WIDGET(ui->forecast_box_images[k]), "shimmer");
    }
 

}


void debounce_search_task(GTask *task, gpointer source_object, gpointer task_data, GCancellable *cancellable){
    (void)source_object;
    (void)cancellable;
    WeatherUI *ui = task_data;

    const char *city = gtk_editable_get_text(GTK_EDITABLE(ui->search_entry));
    get_cities_from_query(city, cities_store, current_city_cancellable);
    NettResponse *res = response_init();
    CityGeoData first = cities_store[0];
    
    if (first.latitude == 0.0f || first.longitude == 0.0f) {
        g_task_return_pointer(task, NULL, NULL);
        show_error_page(ui, LAYOUT_LOCATION_DONT_EXIST); 
        return;
    }
    
    WeatherInfo *info = g_new0(WeatherInfo, 1);
    *info = get_city_weather(first, res);

    if(!nett_ok(res->status_code)){
      destroy_response(res);
      g_task_return_pointer(task, NULL, NULL);
      show_error_page(ui, LAYOUT_REQUEST_ERROR);
      return;
 
    }

    if(info == NULL){
      destroy_response(res);
      g_task_return_pointer(task, NULL, NULL);
      show_error_page(ui, LAYOUT_INFO_LOCATION_NOT_FOUND);
      return;

    } 

    destroy_response(res);
    g_task_return_pointer(task, info, g_free);
}


void debounce_search_done(GObject *source, GAsyncResult *result, gpointer user_data){
    (void)source;
    WeatherUI *ui = user_data;

    GTask *task = G_TASK(result);
    WeatherInfo *info = g_task_propagate_pointer(task, NULL);

    if (!info) {
        return;
    }

    char temperature_c[50];
    snprintf(temperature_c, sizeof(temperature_c), "%d°C", (int)info->temperature);

    gtk_label_set_text(ui->current_temperature_label, temperature_c);
    
    gtk_label_set_text(ui->city_label, info->city_name);
    
    char chance_of_rain[16];
    snprintf(chance_of_rain, sizeof(chance_of_rain), "%d%%", info->chance_of_rain);
    gtk_label_set_text(ui->chance_of_rain_label, chance_of_rain);
    
    char feels_like_c[50];
    snprintf(feels_like_c, sizeof(feels_like_c), "%d°C", (int)info->feels_like);
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



    for(size_t k = 0; k < FORECAST_HOURS_DAY; k++) {
      char forecast_label_c[50];
      snprintf(forecast_label_c, sizeof(forecast_label_c), "%d°C", (int)info->forecast_temperatures[k]);
      gtk_label_set_text(ui->forecast_box_hour_label[k], forecast_label_c);
      update_widgets_from_wmo_code(info->forecast_wcodes[k], k >= 6 && k <= 18, ui->forecast_box_images[k], NULL); 

    }

    gtk_label_set_text(ui->sunrise_label, info->sunrise);
    gtk_label_set_text(ui->sunset_label, info->sunset);

 
    set_loading_ui(ui, false);
    g_free(info);
}



gboolean debounce_search(gpointer data){
    WeatherUI *ui = data;
    set_loading_ui(ui, true); 
    show_main_page(ui);
   
    GTask *task = g_task_new(NULL, NULL, debounce_search_done, ui);
    g_task_set_task_data(task, ui, NULL);
    g_task_run_in_thread(task, debounce_search_task);
    g_object_unref(task);
    

    return G_SOURCE_REMOVE;
}




void on_city_changed(GtkEntry *e, gpointer data) {
    (void)e;
    WeatherUI *ui = data;
    if(!has_internet()){
      show_error_page(ui, LAYOUT_NO_INTERNET);
      return;
    
  }
    debounce_search(ui);
}




static void run_cities_query_in_parallel(GTask *task, gpointer source_object, gpointer task_data, GCancellable *cancellable) {
    (void)source_object;
    (void)task_data;
    (void)cancellable;
    const char *query = task_data;
    
    if (g_cancellable_is_cancelled(cancellable)) {
        LOGGER_WARNING("Task cancel OK.");
        g_task_return_boolean(task, FALSE);
        return;
    }

    get_cities_from_query(query, cities_store, current_city_cancellable);
    
    if (g_cancellable_is_cancelled(cancellable)) {
        LOGGER_WARNING("Task cancel OK.");
        g_task_return_boolean(task, FALSE);
        return;
    }

    g_task_return_boolean(task, TRUE);
}


void run_cities_query_async(const char *query){


    if (current_city_cancellable != NULL)
        g_cancellable_cancel(current_city_cancellable);

    current_city_cancellable = g_cancellable_new();
    GTask *task = g_task_new(NULL, current_city_cancellable, NULL, NULL);
    g_task_set_task_data(task,
                         g_strdup(query),
                         g_free);
    g_task_run_in_thread(task, run_cities_query_in_parallel);
    g_object_unref(task);
}

void on_city_entry_changed(GtkEditable *editable, gpointer data) {
    WeatherUI *ui = data;
    (void)ui; 
 
    LOGGER_DEBUG("Entering in cities query."); 
    const char *city_text = gtk_editable_get_text(editable);
    
    if(strlen(city_text) <= 2) {
        gtk_popover_popdown(GTK_POPOVER(ui->popover));
        return;
    }
      
    run_cities_query_async(city_text);
    return;
}

