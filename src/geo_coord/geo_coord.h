#ifndef GEO_COORD_H 
#define GEO_COORD_H 


#include "../nett/nett.h"
#include "../definitions/definitions.h"

typedef struct {


  f64 latitude;
  f64 longitude;

} GeoCoordinates;


GeoCoordinates get_city_latitude_longitude(const char *city_name, NettResponse *res);


#endif 
