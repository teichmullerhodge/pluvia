

#include "layout.h" 




void expand_all(GtkWidget* widget) {
  if(widget == NULL) return;
  gtk_widget_set_hexpand(widget, TRUE);
  gtk_widget_set_vexpand(widget, TRUE);
  gtk_widget_set_halign(widget, GTK_ALIGN_FILL);
  gtk_widget_set_valign(widget, GTK_ALIGN_FILL);
  return;

  
}

void on_toggle_theme_clicked(GtkButton *button, gpointer user_data){
  (void)button;
  (void)user_data;
  return;
  // toggle_dark_mode(); TODO in toggle_dark_mode match css colors.
}

void toggle_dark_mode(void){
    GtkSettings *settings = gtk_settings_get_default();
    gboolean dark = FALSE;
    g_object_get(settings, "gtk-application-prefer-dark-theme", &dark, NULL);
    g_object_set(settings, "gtk-application-prefer-dark-theme", !dark, NULL);
}
