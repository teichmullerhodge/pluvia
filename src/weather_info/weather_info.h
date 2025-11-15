#ifndef WEATHER_INFO_H 
#define WEATHER_INFO_H 

#include "../definitions/definitions.h"
#include "../geo_coord/geo_coord.h"
#include "../nett/nett.h"
#include <gtk/gtk.h>

typedef struct {
    GtkEntry *search_entry;
    GtkLabel *city_label;
    GtkLabel *current_temperature_label;
    GtkImage *weather_image;

    GtkLabel *sunrise_label;
    GtkLabel *sunset_label;
    GtkLabel *chance_of_rain_label;
    GtkLabel *pressure_label;
    GtkLabel *wind_label;
    GtkLabel *severe_alert_label;
    GtkLabel *feels_like_label;
    GtkLabel *visibility_label;

    GtkBox   *forecast_box;
} WeatherUI;


typedef struct {

  f64 latitude;
  f64 longitude;
  f64 generationtime_ms;
  u64 utc_offset_seconds;
  u64 elevation;
  f64 temperature;
  f64 wind_speed;
  char time[32];

} WeatherInfo;

WeatherInfo get_city_weather(GeoCoordinates coord, NettResponse *res);



#endif 
