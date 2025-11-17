#ifndef SEARCH_STATE_H
#define SEARCH_STATE_H 


#include <gtk/gtk.h>
gboolean debounce_search(gpointer data);
void on_city_changed(GtkEntry *e, gpointer data);
#endif 
