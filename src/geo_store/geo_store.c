#include "geo_store.h"
#include <sqlite3.h>
#include "../logger/logger.h"
#include <gtk/gtk.h>

static sqlite3 *db = NULL;
static sqlite3_stmt *statement = NULL;

QueryStatus get_cities_from_query(const char *query, CityGeoData *cities_store, GCancellable *cancellable){  
  if(db == NULL){
    int op = sqlite3_open("../store/cities.db", &db);
    if(op != SQLITE_OK){
      LOGGER_ERROR("Error opening the cities database.");
      return QUERY_ERROR_DB_NOT_INITIALIZED;
    }
    
    sqlite3_exec(db, "PRAGMA synchronous = OFF;", NULL, NULL, NULL);
    sqlite3_exec(db, "PRAGMA journal_mode = DELETE;", NULL, NULL, NULL);
    sqlite3_exec(db, "PRAGMA cache_size = 10000;", NULL, NULL, NULL);
  }


   const char *sql =
    "SELECT c.name, s.name, c.country, c.lat, c.lon "
    "FROM cities c "
    "LEFT JOIN states s ON s.admin1_code = c.admin1_code "
    "WHERE c.name LIKE ? || '%' "
    "LIMIT 15;";
              


  if (sqlite3_prepare_v2(db, sql, -1, &statement, NULL) != SQLITE_OK) {
        char error_msg[512];
        snprintf(error_msg, sizeof(error_msg), "Error in prepare statement: %s", sqlite3_errmsg(db));
        LOGGER_ERROR(error_msg);
        sqlite3_close(db);
        return QUERY_ERROR_PREPARING_STATEMENT;
    }
  
  sqlite3_bind_text(statement, 1, query, -1, SQLITE_STATIC);
  memset(cities_store, 0, sizeof(CityGeoData) * CITIES_STORE_MAX_ELEMENTS);
  size_t index = 0;
  while (sqlite3_step(statement) == SQLITE_ROW) {

       if (g_cancellable_is_cancelled(cancellable)) {
            sqlite3_finalize(statement);
            return QUERY_OK_ABORTED;
        }
        const unsigned char *name  = sqlite3_column_text(statement, 0);
        const unsigned char *state = sqlite3_column_text(statement, 1);
        const unsigned char *country = sqlite3_column_text(statement, 2);
        double lat = sqlite3_column_double(statement, 3);
        double lon = sqlite3_column_double(statement, 4);
        snprintf(cities_store[index].name, sizeof(cities_store[index].name), "%s - %s | %s", name, state, country);
        cities_store[index].latitude = lat;
        cities_store[index].longitude = lon;
        index++;
  }
  if (!g_cancellable_is_cancelled(cancellable))
    LOGGER_SUCCESS("Query made. OK.");
  sqlite3_finalize(statement);
  return QUERY_OK;

}
