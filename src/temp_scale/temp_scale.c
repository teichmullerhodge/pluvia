#include "temp_scale.h"

#define FAHR_MULT_FACTOR 1.8
#define FAHR_ADD_FACTOR 32

f64 temp_scale_convert(TemperatureScales from, f64 value){
  return from == FAHRENHEIT ? 
  fahrenheit_to_celsius(value) 
  : celsius_to_fahrenheit(value);
}
f64 fahrenheit_to_celsius(f64 fahr){
  return (fahr - FAHR_ADD_FACTOR) / FAHR_MULT_FACTOR;
}

f64 celsius_to_fahrenheit(f64 celsius){
  return (celsius * FAHR_MULT_FACTOR) + FAHR_ADD_FACTOR; 
}






