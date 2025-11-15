#ifndef LOGGER_H
#define LOGGER_H 

#include <stdlib.h> 


#define RESET       "\033[0m"
#define BOLD        "\033[1m"

#define FG_BLACK    "\033[30m"
#define FG_RED      "\033[31m"
#define FG_GREEN    "\033[32m"
#define FG_YELLOW   "\033[33m"
#define FG_BLUE     "\033[34m"
#define FG_MAGENTA  "\033[35m"
#define FG_CYAN     "\033[36m"
#define FG_WHITE    "\033[37m"

#define BG_RED          "\033[41m"
#define BG_GREEN        "\033[42m"
#define BG_YELLOW       "\033[43m"
#define BG_BLUE         "\033[44m"
#define BG_MAGENTA      "\033[45m"
#define BG_CYAN         "\033[46m"
#define BG_WHITE        "\033[47m"
#define BG_BRIGHT_RED   "\033[101m"
#define BG_BRIGHT_GREEN "\033[102m"
#define BG_BRIGHT_YELLOW "\033[103m"
#define BG_BRIGHT_CYAN  "\033[106m"
#define BG_BRIGHT_WHITE "\033[107m"


#define IS_DEBUG 1 

#if IS_DEBUG

  #define LOGGER_DEBUG(message) logger_debug(message)
  #define LOGGER_ERROR(message) logger_error(message)
  #define LOGGER_WARNING(message) logger_warning(message)
  #define LOGGER_SUCCESS(message) logger_success(message)
  #define LOGGER_FATAL(message) logger_fatal(message)
  

#else 

  #define LOGGER_DEBUG(message) ((void)0)
  #define LOGGER_ERROR(message) logger_error(message)
  #define LOGGER_WARNING(message) ((void)0)
  #define LOGGER_SUCCESS(message) ((void)0)
  #define LOGGER_FATAL(message) logger_fatal(message)


#endif 

void logger_debug(const char *message);
void logger_error(const char *message);
void logger_warning(const char *message);
void logger_success(const char *message);
void logger_fatal(const char *message);








#endif 
