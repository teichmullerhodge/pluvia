<p align="center">
  <img src="assets/pluvia-icon.png" alt="Pluvia Icon" width="110">
</p>

# Pluvia

A lightweight weather application built with GTK4.

![Pluvia](https://img.shields.io/badge/pluvia-weather_app-blue)
![C Language](https://img.shields.io/badge/Language-C-orange)
![GTK4](https://img.shields.io/badge/desktop-GTK4-green)



## Features 

Pluvia currently supports:

- Temperature (°C)
- Feels like (°C)
- Pressure (mb)
- Wind speed (km/h)
- Visibility
- Chance of rain (%)
- Hour-by-hour forecast from 00:00 to 23:00
- Sunrise time
- Sunset time
- Quick visual representation for each hour (icons, temperature, conditions)
- Error handling showing errors (location not found, missing internet connection, request-error)

---

## Screenshots

<p align="center">
  <img src="assets/pluvia-screen.png" alt="Pluvia screenshot" width="650">
</p>

<p align="center">
  <img src="assets/pluvia-screen-snow.png" alt="Pluvia screenshot snow" width="650">
</p>

---

## Technical Notes

- Built entirely with GTK4.
- Weather data fetched from the Open-Meteo API.
- All requests are performed asynchronously using GTask, ensuring the UI remains smooth and responsive.
- Designed to run efficiently even on low-resource machines.

## Build

- Ensure you have GTK4 installed on your machine.
- Run the `cc.sh` script.
- Execute the binary inside the `build/` directory.

## Releases 

- Soon.

