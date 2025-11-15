

#include "layout.h" 




void expand_all(GtkWidget* widget) {
  if(widget == NULL) return;
  gtk_widget_set_hexpand(widget, TRUE);
  gtk_widget_set_vexpand(widget, TRUE);
  gtk_widget_set_halign(widget, GTK_ALIGN_FILL);
  gtk_widget_set_valign(widget, GTK_ALIGN_FILL);
  return;

  
}
