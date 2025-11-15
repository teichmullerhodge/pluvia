#include "widget_weather_card.h"

GtkWidget *widget_weather_card(const char *label, const char* value, const char *icon_or_path, bool use_icon){
  
  GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  GtkWidget *info_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

  GtkWidget *label_widget = gtk_label_new(label);
  GtkWidget *label_val = gtk_label_new(value);
  GtkWidget *icon_widget = use_icon ? gtk_image_new_from_icon_name(icon_or_path) : gtk_image_new_from_file(icon_or_path);

  gtk_box_append(GTK_BOX(info_box), label_widget);
  gtk_box_append(GTK_BOX(info_box), label_val);

  gtk_box_append(GTK_BOX(main_box), info_box);
  gtk_box_append(GTK_BOX(main_box), icon_widget);

  gtk_widget_add_css_class(label_widget, "wc-label");
  gtk_widget_add_css_class(label_widget, "wc-value");
  gtk_widget_add_css_class(label_widget, "wc-icon");
  gtk_widget_add_css_class(main_box, "wc");




  return main_box;
}

GtkLabel *get_label_val_from_card(GtkWidget *weather_card){

 return GTK_LABEL(gtk_widget_get_last_child(gtk_widget_get_first_child(weather_card)));


}

