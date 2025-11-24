#ifndef WEATHER_INFO_H 
#define WEATHER_INFO_H 

#include "../definitions/definitions.h"
#include "../geo_store/geo_store.h"
#include "../nett/nett.h"
#include <gtk/gtk.h>


typedef enum {
    // 0
    WMO_CLEAR_SKY = 0,

    // 1–3
    WMO_MAINLY_CLEAR      = 1,
    WMO_PARTLY_CLOUDY     = 2,
    WMO_OVERCAST          = 3,

    // 45–48
    WMO_FOG               = 45,
    WMO_RIME_FOG          = 48,

    // 51–55 (drizzle)
    WMO_DRIZZLE_LIGHT     = 51,
    WMO_DRIZZLE_MODERATE  = 53,
    WMO_DRIZZLE_DENSE     = 55,

    // 56–57 (freezing drizzle)
    WMO_FREEZING_DRIZZLE_LIGHT = 56,
    WMO_FREEZING_DRIZZLE_DENSE = 57,

    // 61–65 (rain)
    WMO_RAIN_SLIGHT       = 61,
    WMO_RAIN_MODERATE     = 63,
    WMO_RAIN_HEAVY        = 65,

    // 66–67 (freezing rain)
    WMO_FREEZING_RAIN_LIGHT = 66,
    WMO_FREEZING_RAIN_HEAVY = 67,

    // 71–75 (snowfall)
    WMO_SNOWFALL_SLIGHT   = 71,
    WMO_SNOWFALL_MODERATE = 73,
    WMO_SNOWFALL_HEAVY    = 75,

    // 77
    WMO_SNOW_GRAINS       = 77,

    // 80–82 (rain showers)
    WMO_RAIN_SHOWERS_SLIGHT   = 80,
    WMO_RAIN_SHOWERS_MODERATE = 81,
    WMO_RAIN_SHOWERS_VIOLENT  = 82,

    // 85–86 (snow showers)
    WMO_SNOW_SHOWERS_SLIGHT = 85,
    WMO_SNOW_SHOWERS_HEAVY  = 86,

    // 95 (thunderstorm)
    WMO_THUNDERSTORM_MODERATE = 95,

    // 96–99 (thunderstorm with hail)
    WMO_THUNDERSTORM_HAIL_SLIGHT = 96,
    WMO_THUNDERSTORM_HAIL_HEAVY  = 99

} WMOWeatherCode;

#define FORECAST_HOURS_DAY 24


typedef struct {
    
    GtkEntry *search_entry;
    GtkLabel *city_label;
    GtkLabel *current_temperature_label;
    GtkLabel *weather_description_label;
    GtkImage *weather_image;
    GtkLabel *sunrise_label;
    GtkLabel *sunset_label;
    GtkLabel *chance_of_rain_label;
    GtkLabel *pressure_label;
    GtkLabel *wind_label;
    GtkLabel *severe_alert_label;
    GtkLabel *feels_like_label;
    GtkLabel *visibility_label;
    GtkLabel *forecast_box_hour_label[FORECAST_HOURS_DAY];
    GtkImage *forecast_box_images[FORECAST_HOURS_DAY];

    GtkStack *stack;

    GtkLabel *error_label;
    GtkImage *error_image; 
    GtkLabel *error_code;

    GtkWidget *popover;
    GtkWidget *pop_over_list_box;

} WeatherUI;



typedef struct {

  char city_name[512];
  char time[32];
  f64 latitude;
  f64 longitude;
  u64 elevation;
  f64 temperature;
  char sunrise[6];
  char sunset[6]; 
  u8 chance_of_rain;
  f64 pressure;
  f64 wind_speed;
  bool severe_alert;
  f64 feels_like;
  u64 visibility;
  bool is_day;
  WMOWeatherCode wcode;
  f64 forecast_temperatures[FORECAST_HOURS_DAY];
  WMOWeatherCode forecast_wcodes[FORECAST_HOURS_DAY];

   
} WeatherInfo;

WeatherInfo get_city_weather(CityGeoData coord, NettResponse *res);



#endif 
