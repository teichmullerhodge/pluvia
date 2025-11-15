#ifndef TEMP_SCALE_H
#define TEMP_SCALE_H 


#include "../definitions/definitions.h"

typedef enum {

  FAHRENHEIT,
  CELSIUS,

} TemperatureScales;



f64 temp_scale_convert(TemperatureScales from, f64 value);
f64 fahrenheit_to_celsius(f64 fahr);
f64 celsius_to_fahrenheit(f64 celsius);




#endif 
