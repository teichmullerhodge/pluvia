#include <gtk/gtk.h>
#include "appconfig/appconfig.h"
#include "layout/layout.h"
#include "logger/logger.h"
#include "temp_scale/temp_scale.h"
#include <assert.h>
#include "nett/nett.h"
#include "geo_coord/geo_coord.h"
#include "weather_info/weather_info.h"
#include "widgets/widget_time_weather.h"
#include "widgets/widget_weather_card.h"

static void on_activate(GtkApplication *app) {
  
  GtkCssProvider *provider = gtk_css_provider_new();
  gtk_css_provider_load_from_path(provider, "../resources/css/index.css");
  gtk_style_context_add_provider_for_display(gdk_display_get_default(),
                                             GTK_STYLE_PROVIDER(provider),
                                             GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

  g_object_unref(provider);

  GtkWidget *window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), APP_CONFIG_WIN_TITLE);
  gtk_window_set_default_size(GTK_WINDOW(window), APP_CONFIG_WIN_WIDTH, APP_CONFIG_WIN_HEIGHT);
  gtk_widget_add_css_class(window, "window-main");
 
  GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_widget_add_css_class(main_box, "main-box");


  GtkWidget *header_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);

  GtkWidget *app_name_label = gtk_label_new("Pluvia");
  gtk_widget_add_css_class(app_name_label, "app-title");
  GtkWidget *logo_image = gtk_image_new_from_file("../assets/pluvia-icon.png");
  GtkWidget *city_entry = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(city_entry), "Search for cities");
  
  gtk_box_append(GTK_BOX(header_box), logo_image);
  gtk_box_append(GTK_BOX(header_box), app_name_label);
  gtk_box_append(GTK_BOX(header_box), city_entry);
  gtk_widget_set_hexpand(city_entry, true);

  gtk_box_append(GTK_BOX(main_box), header_box);

  GtkWidget *weather_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  GtkWidget *weather_labels_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);


  GtkWidget *city_label = gtk_label_new("Madrid");
  gtk_widget_add_css_class(city_label, "city-label");
  GtkWidget *current_temperature_label = gtk_label_new("16°");
  gtk_widget_add_css_class(current_temperature_label, "current-temperature-label");


  gtk_box_append(GTK_BOX(weather_labels_box), city_label);
  gtk_box_append(GTK_BOX(weather_labels_box), current_temperature_label);

  GtkWidget *weather_image = gtk_image_new_from_file("../assets/sun-cloud.png");
  gtk_widget_add_css_class(weather_image, "current-temperature-image");
  gtk_widget_set_size_request(weather_image, 60, 60);

  gtk_box_append(GTK_BOX(weather_box), weather_labels_box);
  gtk_box_append(GTK_BOX(weather_box), weather_image);

  
  gtk_box_append(GTK_BOX(main_box), weather_box);

  i64 temperatures[] = {10, 11, 12, 12, 13, 14, 15, 16, 16, 16, 17, 17, 18, 19, 19, 20, 21, 20, 22, 20, 19, 18, 17, 15};


  GtkWidget *scrolled_forecast_win = gtk_scrolled_window_new();
  GtkWidget *forecast_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_forecast_win), forecast_box);
  GtkWidget *forecast_label = gtk_label_new("TODAY'S FORECAST"); 
  gtk_widget_add_css_class(forecast_label, "section-label");

  GtkWidget *tm_weathers_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 15);
  gtk_widget_set_halign(forecast_label, GTK_ALIGN_START);
  gtk_box_append(GTK_BOX(forecast_box), forecast_label);

  size_t temperatures_size = sizeof(temperatures) / sizeof(temperatures[0]);
  for(size_t k = 0; k < temperatures_size; k++){
    GtkWidget *tm_weather = widget_time_weather(k, temperatures[k]);
    gtk_box_append(GTK_BOX(tm_weathers_box), tm_weather);
  }

  gtk_box_append(GTK_BOX(forecast_box), tm_weathers_box);


  gtk_scrolled_window_set_policy(
    GTK_SCROLLED_WINDOW(scrolled_forecast_win),
    GTK_POLICY_AUTOMATIC,  
    GTK_POLICY_NEVER      
);

  gtk_box_append(GTK_BOX(main_box), scrolled_forecast_win);
  gtk_widget_add_css_class(forecast_box, "section-box");

  GtkWidget *weather_details_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  GtkWidget *weather_details_label = gtk_label_new("WEATHER DETAILS");
  gtk_widget_set_halign(weather_details_label, GTK_ALIGN_START);
 
  gtk_widget_add_css_class(weather_details_label, "section-label");
  gtk_widget_add_css_class(weather_details_box, "section-box");



  GtkWidget *details_grid = gtk_grid_new();
  
  GtkWidget *sunrise_card = widget_weather_card("Sunrise", "8:18", "../assets/sunrise.png", false);
  GtkWidget *sunset_card = widget_weather_card("Sunset", "18:40", "../assets/sunset.png", false);
  GtkWidget *chance_of_rain_card = widget_weather_card("Chance of rain", "21%", "weather-showers-scattered", true);
  GtkWidget *pressure_card = widget_weather_card("Pressure", "1023 mb", "view-restore-symbolic", true);
  GtkWidget *wind_card = widget_weather_card("Wind", "8 km/h", "weather-windy-symbolic", true);
  GtkWidget *uv_idx_card = widget_weather_card("Severe alert?", "No", "weather-severe-alert-symbolic", true);
  GtkWidget *feels_like_card = widget_weather_card("Feels like", "12°", "../assets/termometer.png", false);
  GtkWidget *visibility_card = widget_weather_card("Visibility", "14 km", "../assets/eye-visible.png", false);

  gtk_grid_attach(GTK_GRID(details_grid), sunrise_card, 0, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(details_grid), sunset_card, 1, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(details_grid), chance_of_rain_card, 2, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(details_grid), pressure_card, 3, 0, 1, 1);

  gtk_grid_attach(GTK_GRID(details_grid), wind_card, 0, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(details_grid), uv_idx_card, 1, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(details_grid), feels_like_card, 2, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(details_grid), visibility_card, 3, 1, 1, 1);


  gtk_grid_set_column_spacing(GTK_GRID(details_grid), 10);
  gtk_grid_set_row_spacing(GTK_GRID(details_grid), 10);
  gtk_grid_set_row_homogeneous(GTK_GRID(details_grid), true);
  gtk_grid_set_column_homogeneous(GTK_GRID(details_grid), true);



  gtk_widget_set_hexpand(details_grid, true);
  gtk_widget_set_halign(details_grid, GTK_ALIGN_CENTER);



  gtk_box_append(GTK_BOX(weather_details_box), weather_details_label);
  gtk_box_append(GTK_BOX(weather_details_box), details_grid);



  gtk_box_append(GTK_BOX(main_box), weather_details_box);
 

  expand_all(main_box);

  gtk_window_set_child(GTK_WINDOW(window), main_box);   
  gtk_window_present(GTK_WINDOW(window));

  

}

int main(int argc, char **argv) {


  f64 degree = 32.0f;
  assert(fahrenheit_to_celsius(degree) == 0);
  assert(celsius_to_fahrenheit(degree) >= 89.0f && celsius_to_fahrenheit(degree) <= 90.0f);
  assert(temp_scale_convert(FAHRENHEIT, degree) == fahrenheit_to_celsius(degree));
  assert(temp_scale_convert(CELSIUS, degree) == celsius_to_fahrenheit(degree));

/*
  NettResponse *res = response_init();
  if(res == NULL) {
    return -1;
  }

  GeoCoordinates geo = get_city_latitude_longitude("Londrina", res);
  WeatherInfo info = get_city_weather(geo, res);

  g_print("Latitude: %f - Longitude: %f | Weather: temperature: %f, wind-speed: %f, time: %s\n", 
          geo.latitude, geo.longitude, info.temperature, info.wind_speed, info.time);

*/


  GtkApplication *app =
      gtk_application_new(APP_CONFIG_APP_ID, APP_CONFIG_APP_FLAGS);

  g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);
  int status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);
  return status;
}
