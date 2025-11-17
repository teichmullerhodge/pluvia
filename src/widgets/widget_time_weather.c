#include "widget_time_weather.h"

GtkWidget *widget_time_weather(u8 hour, i64 temp){

  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  char time_hour[8];
  char *format = hour >= 10 ? "%d:00" : "0%d:00";
  snprintf(time_hour, sizeof(time_hour), format, hour);
  GtkWidget *time_label = gtk_label_new(time_hour);
  GtkWidget *image = gtk_image_new_from_file("../assets/wmo/icons/01n@2x.png");
  char temp_format[8];
  snprintf(temp_format, sizeof(temp_format), "%ld°", temp);
  GtkWidget *temp_label = gtk_label_new(temp_format);
  gtk_box_append(GTK_BOX(box), time_label);
  gtk_box_append(GTK_BOX(box), image);
  gtk_box_append(GTK_BOX(box), temp_label);

  gtk_widget_add_css_class(time_label, "tw-time-label");
  gtk_widget_add_css_class(image, "tw-time-image");
  gtk_widget_add_css_class(temp_label, "tw-temp-label");
  gtk_widget_add_css_class(box, "tw");



  return box;

}

