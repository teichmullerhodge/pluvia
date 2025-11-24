

#include "layout.h"
#include "../appconfig/appconfig.h"
#include "../logger/logger.h"

static GSettings *settings = NULL;
static LayoutTheme current_system_theme = LAYOUT_NONE;
static LayoutTheme current_app_theme = LAYOUT_NONE;


void expand_all(GtkWidget *widget) {
  if (widget == NULL)
    return;
  gtk_widget_set_hexpand(widget, TRUE);
  gtk_widget_set_vexpand(widget, TRUE);
  gtk_widget_set_halign(widget, GTK_ALIGN_FILL);
  gtk_widget_set_valign(widget, GTK_ALIGN_FILL);
  return;
}

void on_toggle_theme_clicked(GtkButton *button, gpointer user_data){
  (void)button;
  (void)user_data;
  toggle_dark_mode(user_data); 
}

void toggle_dark_mode(gpointer user_data) {
    static bool dark = false;
  
    dark = current_app_theme == LAYOUT_DARK_MODE;
    dark = !dark;
    current_app_theme = dark ? LAYOUT_DARK_MODE : LAYOUT_LIGHT_MODE;
    
    GtkCssProvider *provider = (GtkCssProvider*)user_data;
    const char *css_path = dark ? CSS_PATH_DARK : CSS_PATH_LIGHT;
    LOAD_CSS(provider, css_path);
}

void toggle_system_mode(GtkCssProvider *provider, bool dark) {
    const char *css_path = dark ? CSS_PATH_DARK : CSS_PATH_LIGHT;
    LOAD_CSS(provider, css_path);
}


void error_from_layout_errors(char *buff, size_t buff_size, LayoutErrors error){
  switch (error) {
  case LAYOUT_INFO_LOCATION_NOT_FOUND:
    snprintf(buff, buff_size, "%s", LAYOUT_INFO_LOCATION_NOT_FOUND_MSG);
    break;
  case LAYOUT_REQUEST_ERROR:
    snprintf(buff, buff_size, "%s", LAYOUT_REQUEST_ERROR_MSG);
    break;
  case LAYOUT_LOCATION_DONT_EXIST:
    snprintf(buff, buff_size, "%s", LAYOUT_LOCATION_DONT_EXIST_MSG);
    break;
  case LAYOUT_REQUEST_TIMEOUT:
    snprintf(buff, buff_size, "%s", LAYOUT_REQUEST_TIMEOUT_MSG);
    break;
  case LAYOUT_NO_INTERNET:
    snprintf(buff, buff_size, "%s", LAYOUT_NO_INTERNET_MSG);
    break;
  case LAYOUT_GENERIC_ERROR:
    snprintf(buff, buff_size, "%s", LAYOUT_GENERIC_ERROR_MSG);
    break;
  default:
    snprintf(buff, buff_size, "%s", LAYOUT_GENERIC_ERROR_MSG);
    break;
  }
}
void error_code_from_layout_errors(char *buff, size_t buff_size, LayoutErrors error){
  snprintf(buff, buff_size, "PL_EC %d", error);
}

void image_path_from_layout_errors(char *buff, size_t buff_size, LayoutErrors error){
  switch (error){
  case LAYOUT_INFO_LOCATION_NOT_FOUND:
    snprintf(buff, buff_size, "%s", LAYOUT_INFO_LOCATION_NOT_FOUND_IMAGE_PATH);
    break;
  case LAYOUT_REQUEST_ERROR:
    snprintf(buff, buff_size, "%s", LAYOUT_REQUEST_ERROR_IMAGE_PATH);
    break;
  case LAYOUT_LOCATION_DONT_EXIST:
    snprintf(buff, buff_size, "%s", LAYOUT_LOCATION_DONT_EXIST_IMAGE_PATH);
    break;
  case LAYOUT_REQUEST_TIMEOUT:
    snprintf(buff, buff_size, "%s", LAYOUT_REQUEST_TIMEOUT_IMAGE_PATH);
    break;
  case LAYOUT_NO_INTERNET:
    snprintf(buff, buff_size, "%s", LAYOUT_NO_INTERNET_IMAGE_PATH);
    break;
  case LAYOUT_GENERIC_ERROR:
    snprintf(buff, buff_size, "%s", LAYOUT_GENERIC_ERROR_IMAGE_PATH);
    break;
  default:
    snprintf(buff, buff_size, "%s", LAYOUT_GENERIC_ERROR_IMAGE_PATH);
    break;
  }
}

GtkWidget *layout_page_error(WeatherUI *ui){

  char generic_error_code[12];
  error_code_from_layout_errors(generic_error_code, sizeof(generic_error_code), LAYOUT_GENERIC_ERROR);
  GtkWidget *page_error = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
  gtk_widget_add_css_class(page_error, "error-page");
  gtk_widget_set_halign(page_error, GTK_ALIGN_CENTER);
  gtk_widget_set_valign(page_error, GTK_ALIGN_CENTER);
  GtkWidget *error_label = gtk_label_new(LAYOUT_GENERIC_ERROR_MSG);
  GtkWidget *error_image = gtk_image_new_from_resource("/com/pluvia/assets/generic-error.png");
  GtkWidget *error_code = gtk_label_new(generic_error_code);
  gtk_widget_add_css_class(error_code, "error-code");
  gtk_widget_set_size_request(GTK_WIDGET(error_image), 250, 250);
  ui->error_label = GTK_LABEL(error_label);
  ui->error_image = GTK_IMAGE(error_image);
  ui->error_code = GTK_LABEL(error_code);
  gtk_widget_add_css_class(error_label, "error-label");
  gtk_box_append(GTK_BOX(page_error), error_label);
  gtk_box_append(GTK_BOX(page_error), error_image);
  gtk_box_append(GTK_BOX(page_error), error_code);
  return page_error;
}

void show_error_page(WeatherUI *ui, LayoutErrors error){
  char msg[512];
  char code[12];
  char image_path[512];

  error_from_layout_errors(msg, sizeof(msg), error);
  error_code_from_layout_errors(code, sizeof(code), error);
  image_path_from_layout_errors(image_path, sizeof(image_path), error);

  gtk_label_set_text(ui->error_label, msg);
  gtk_label_set_text(ui->error_code, code);
  gtk_image_clear(ui->error_image);
  gtk_image_set_from_resource(ui->error_image, image_path);
  gtk_stack_set_visible_child_name(GTK_STACK(ui->stack), "page-error");
}

void show_main_page(WeatherUI *ui){
  gtk_stack_set_visible_child_name(ui->stack, "page-main");
}

LayoutTheme get_system_theme(){
    
    if(settings == NULL){
      settings = g_settings_new("org.gnome.desktop.interface");
    }
    bool is_dark = strstr(g_settings_get_string(settings, "color-scheme"), "dark") != NULL;
    return is_dark ? LAYOUT_DARK_MODE : LAYOUT_LIGHT_MODE;
}


gboolean check_theme_change(gpointer user_data){
 
    LayoutTheme theme = get_system_theme();
    if(current_system_theme == LAYOUT_NONE || current_system_theme != theme){

      GtkCssProvider *provider = (GtkCssProvider*)user_data;
      LOGGER_DEBUG("Changing theme.");
      toggle_system_mode(provider, theme == LAYOUT_DARK_MODE);
      current_app_theme = theme;
      current_system_theme = theme;
    
    }

    return G_SOURCE_CONTINUE;
}



