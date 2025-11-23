#include <gtk/gtk.h>
#include "appconfig/appconfig.h"
#include "layout/layout.h"
#include "state/search_state.h"
#include "temp_scale/temp_scale.h"
#include <assert.h>
#include "weather_info/weather_info.h"
#include "widgets/widget_time_weather.h"
#include "widgets/widget_touchable.h"
#include "widgets/widget_weather_card.h"
#include <locale.h>

static void on_shutdown(GtkApplication *app, WeatherUI *ui){
  (void)app;
  g_free(ui);
}

static void on_activate(GtkApplication *app){

   WeatherUI *ui = g_new0(WeatherUI, 1);
   GSettings *settings = g_settings_new("org.gnome.desktop.interface");
   bool is_dark = strstr(g_settings_get_string(settings, "color-scheme"), "dark") != NULL;
  
   g_object_unref(settings);

   GtkCssProvider *provider = gtk_css_provider_new();
   LOAD_CSS(provider, is_dark ? CSS_PATH_DARK : CSS_PATH_LIGHT);
   gtk_style_context_add_provider_for_display(gdk_display_get_default(),
                                             GTK_STYLE_PROVIDER(provider),
                                             GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);



  g_timeout_add(500, check_theme_change, provider);

  GtkWidget *window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), APP_CONFIG_WIN_TITLE);
  gtk_window_set_default_size(GTK_WINDOW(window), APP_CONFIG_WIN_WIDTH, APP_CONFIG_WIN_HEIGHT);
  gtk_widget_add_css_class(window, "window-main");

  GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_widget_add_css_class(main_box, "main-box");

  GtkWidget *header_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);

  GtkWidget *app_name_label = gtk_label_new("Pluvia");
  gtk_widget_add_css_class(app_name_label, "app-title");
  GtkWidget *logo_image = gtk_image_new_from_resource("/com/pluvia/assets/pluvia-icon.png");
  GtkWidget *city_entry = gtk_entry_new();
  gtk_widget_add_css_class(city_entry, "city-entry");
  gtk_entry_set_placeholder_text(GTK_ENTRY(city_entry), "Search for cities");
  GtkWidget *toggle_theme_btn = touchable(NULL, "night-light-symbolic", NULL);
  gtk_widget_add_css_class(toggle_theme_btn, "toggle-theme");
  gtk_box_append(GTK_BOX(header_box), logo_image);
  gtk_box_append(GTK_BOX(header_box), app_name_label);
  gtk_box_append(GTK_BOX(header_box), city_entry);
  gtk_box_append(GTK_BOX(header_box), toggle_theme_btn);
  g_signal_connect(city_entry, "activate", G_CALLBACK(on_city_changed), ui);
  gtk_widget_set_hexpand(city_entry, true);

  g_signal_connect(toggle_theme_btn, "clicked", G_CALLBACK(on_toggle_theme_clicked), provider);

  gtk_box_append(GTK_BOX(main_box), header_box);

  GtkWidget *page_main = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

  GtkWidget *weather_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  gtk_widget_set_halign(weather_box, GTK_ALIGN_CENTER);
  gtk_widget_add_css_class(weather_box, "weather-box");
  GtkWidget *weather_labels_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

  GtkWidget *city_label = gtk_label_new("Search a city");
  gtk_widget_add_css_class(city_label, "city-label");
  GtkWidget *current_temperature_label = gtk_label_new("0°C");
  gtk_widget_add_css_class(current_temperature_label, "current-temperature-label");

  gtk_box_append(GTK_BOX(weather_labels_box), city_label);
  gtk_box_append(GTK_BOX(weather_labels_box), current_temperature_label);

  GtkWidget *weather_image = gtk_image_new_from_resource("/com/pluvia/assets/wmo/icons/01d@2x.png");
  gtk_widget_add_css_class(weather_image, "current-temperature-image");
  gtk_widget_set_size_request(weather_image, 60, 60);

  GtkWidget *weather_description = gtk_label_new("");

  gtk_box_append(GTK_BOX(weather_box), weather_labels_box);
  gtk_box_append(GTK_BOX(weather_box), weather_image);
  gtk_box_append(GTK_BOX(weather_box), weather_description);

  gtk_box_append(GTK_BOX(page_main), weather_box);

  GtkWidget *scrolled_forecast_win = gtk_scrolled_window_new();
  GtkWidget *forecast_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_forecast_win), forecast_box);
  GtkWidget *forecast_label = gtk_label_new("TODAY'S FORECAST");
  gtk_widget_add_css_class(forecast_label, "section-label");

  GtkWidget *tm_weathers_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 15);
  gtk_widget_set_halign(forecast_label, GTK_ALIGN_START);
  gtk_box_append(GTK_BOX(forecast_box), forecast_label);

  for (size_t k = 0; k < FORECAST_HOURS_DAY; k++)
  {
    GtkWidget *tm_weather = widget_time_weather(k, 0);
    gtk_box_append(GTK_BOX(tm_weathers_box), tm_weather);
    ui->forecast_box_hour_label[k] = GTK_LABEL(gtk_widget_get_last_child(tm_weather));
    ui->forecast_box_images[k] = GTK_IMAGE(gtk_widget_get_next_sibling(gtk_widget_get_first_child(tm_weather)));
  }

  gtk_box_append(GTK_BOX(forecast_box), tm_weathers_box);

  gtk_scrolled_window_set_policy(
      GTK_SCROLLED_WINDOW(scrolled_forecast_win),
      GTK_POLICY_AUTOMATIC,
      GTK_POLICY_NEVER);

  gtk_box_append(GTK_BOX(page_main), scrolled_forecast_win);
  gtk_widget_add_css_class(forecast_box, "section-box");

  GtkWidget *weather_details_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  GtkWidget *weather_details_label = gtk_label_new("WEATHER DETAILS");
  gtk_widget_set_halign(weather_details_label, GTK_ALIGN_START);

  gtk_widget_add_css_class(weather_details_label, "section-label");
  gtk_widget_add_css_class(weather_details_box, "section-box");

  GtkWidget *details_grid = gtk_grid_new();

  GtkWidget *sunrise_card = widget_weather_card("Sunrise", "Sunrise hour", "daytime-sunrise-symbolic", true);
  GtkWidget *sunset_card = widget_weather_card("Sunset", "Sunset hour", "daytime-sunset-symbolic", true);
  GtkWidget *chance_of_rain_card = widget_weather_card("Chance of rain", "%", "weather-showers-scattered", true);
  GtkWidget *pressure_card = widget_weather_card("Pressure", "mb", "view-restore-symbolic", true);
  GtkWidget *wind_card = widget_weather_card("Wind", "km/h", "weather-windy-symbolic", true);
  GtkWidget *severe_alert_card = widget_weather_card("Severe alert?", "No", "weather-severe-alert-symbolic", true);
  GtkWidget *feels_like_card = widget_weather_card("Feels like", "°C", "dialog-question-symbolic", true);
  GtkWidget *visibility_card = widget_weather_card("Visibility", "km", "weather-fog-symbolic", true);

  gtk_grid_attach(GTK_GRID(details_grid), sunrise_card, 0, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(details_grid), sunset_card, 1, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(details_grid), chance_of_rain_card, 2, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(details_grid), pressure_card, 3, 0, 1, 1);

  gtk_grid_attach(GTK_GRID(details_grid), wind_card, 0, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(details_grid), severe_alert_card, 1, 1, 1, 1);
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
  gtk_box_append(GTK_BOX(page_main), weather_details_box);

  expand_all(page_main);

  GtkWidget *stack = gtk_stack_new();
  gtk_stack_set_transition_type(GTK_STACK(stack), GTK_STACK_TRANSITION_TYPE_CROSSFADE);
  gtk_stack_set_transition_duration(GTK_STACK(stack), 250);
  ui->stack = GTK_STACK(stack);

  gtk_stack_add_named(GTK_STACK(ui->stack), page_main, "page-main");

  GtkWidget *page_error = layout_page_error(ui);
  gtk_stack_add_named(GTK_STACK(ui->stack), page_error, "page-error");

  ui->search_entry = GTK_ENTRY(city_entry);
  ui->city_label = GTK_LABEL(city_label);
  ui->current_temperature_label = GTK_LABEL(current_temperature_label);
  ui->weather_image = GTK_IMAGE(weather_image);
  ui->weather_description_label = GTK_LABEL(weather_description);
  ui->sunrise_label = get_label_val_from_card(sunrise_card);
  ui->sunset_label = get_label_val_from_card(sunset_card);
  ui->chance_of_rain_label = get_label_val_from_card(chance_of_rain_card);
  ui->pressure_label = get_label_val_from_card(pressure_card);
  ui->wind_label = get_label_val_from_card(wind_card);
  ui->severe_alert_label = get_label_val_from_card(severe_alert_card);
  ui->feels_like_label = get_label_val_from_card(feels_like_card);
  ui->visibility_label = get_label_val_from_card(visibility_card);

  g_signal_connect(app, "shutdown", G_CALLBACK(on_shutdown), ui);

  gtk_widget_set_hexpand(window, FALSE);
  gtk_widget_set_vexpand(window, FALSE);

  gtk_box_append(GTK_BOX(main_box), stack);

  gtk_window_set_child(GTK_WINDOW(window), main_box);
  gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv)
{

  setlocale(LC_NUMERIC, "C");
  f64 degree = 32.0f;
  assert(fahrenheit_to_celsius(degree) == 0);
  assert(celsius_to_fahrenheit(degree) >= 89.0f && celsius_to_fahrenheit(degree) <= 90.0f);
  assert(temp_scale_convert(FAHRENHEIT, degree) == fahrenheit_to_celsius(degree));
  assert(temp_scale_convert(CELSIUS, degree) == celsius_to_fahrenheit(degree));

  GtkApplication *app =
      gtk_application_new(APP_CONFIG_APP_ID, APP_CONFIG_APP_FLAGS);

  g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);
  int status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);
  return status;
}
