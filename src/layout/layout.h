#ifndef LAYOUT_H
#define LAYOUT_H


#include <gtk/gtk.h> 


void expand_all(GtkWidget *widget);
void toggle_dark_mode(void);
void on_toggle_theme_clicked(GtkButton *button, gpointer user_data);


#endif 


