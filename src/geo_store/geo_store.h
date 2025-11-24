#ifndef GEO_STORE_H
#define GEO_STORE_H 

#include "../definitions/definitions.h"
#include <gtk/gtk.h>



typedef struct {

  char name[512];
  f64 latitude;
  f64 longitude;


} CityGeoData;


typedef enum {

  QUERY_OK,
  QUERY_OK_ABORTED,
  QUERY_ERROR_GENERIC,
  QUERY_ERROR_PREPARING_STATEMENT,
  QUERY_ERROR_DB_NOT_INITIALIZED,
} QueryStatus;

#define CITIES_STORE_MAX_ELEMENTS 20

QueryStatus get_cities_from_query(const char *query, CityGeoData *cities_store, GCancellable *cancellable); 

#endif 
