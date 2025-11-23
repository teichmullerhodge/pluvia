#ifndef LAYOUT_H
#define LAYOUT_H


#include <gtk/gtk.h> 
#include "../weather_info/weather_info.h"

#define LAYOUT_INFO_LOCATION_NOT_FOUND_MSG "Information about this location not found. Please try again later."
#define LAYOUT_LOCATION_DONT_EXIST_MSG "This location doesn't exist. Please try again with a different name."
#define LAYOUT_REQUEST_ERROR_MSG "Error in the request. Please try again later."
#define LAYOUT_REQUEST_TIMEOUT_MSG "Request timed out. Please try again later."
#define LAYOUT_NO_INTERNET_MSG "No access to internet. Please connect and try again later."
#define LAYOUT_GENERIC_ERROR_MSG "An error ocurred. Please try again later."

#define LAYOUT_INFO_LOCATION_NOT_FOUND_IMAGE_PATH "/com/pluvia/assets/info-error.png"
#define LAYOUT_LOCATION_DONT_EXIST_IMAGE_PATH "/com/pluvia/assets/location-not-found.png"
#define LAYOUT_REQUEST_ERROR_IMAGE_PATH "/com/pluvia/assets/generic-error.png"
#define LAYOUT_REQUEST_TIMEOUT_IMAGE_PATH "/com/pluvia/assets/request-timeout.png"
#define LAYOUT_NO_INTERNET_IMAGE_PATH "/com/pluvia/assets/no-internet.png"
#define LAYOUT_GENERIC_ERROR_IMAGE_PATH "/com/pluvia/assets/generic-error.png"



typedef enum {

  LAYOUT_INFO_LOCATION_NOT_FOUND,
  LAYOUT_LOCATION_DONT_EXIST,
  LAYOUT_REQUEST_TIMEOUT,
  LAYOUT_REQUEST_ERROR,
  LAYOUT_NO_INTERNET,
  LAYOUT_GENERIC_ERROR

} LayoutErrors;  


typedef enum {

  LAYOUT_NONE,
  LAYOUT_DARK_MODE,
  LAYOUT_LIGHT_MODE

} LayoutTheme;


GtkWidget *layout_page_error(WeatherUI *ui);
void expand_all(GtkWidget *widget);
void toggle_dark_mode(gpointer user_data);
void on_toggle_theme_clicked(GtkButton *button, gpointer user_data);
void show_error_page(WeatherUI *ui, LayoutErrors error);
void show_main_page(WeatherUI *ui);
void on_system_theme_changed(GObject *settings, GParamSpec *pspec, gpointer user_data);

LayoutTheme get_system_theme();
gboolean check_theme_change(gpointer user_data);



#endif 


