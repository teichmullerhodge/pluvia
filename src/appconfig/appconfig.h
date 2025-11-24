#ifndef APPCONFIG_H
#define APPCONFIG_H 

#define APP_CONFIG_WIN_TITLE "Pluvia"
#define APP_CONFIG_APP_ID "com.pluvia.app"
#define APP_CONFIG_APP_FLAGS G_APPLICATION_DEFAULT_FLAGS
#define APP_CONFIG_WIN_WIDTH 800
#define APP_CONFIG_WIN_HEIGHT 600

#define APP_CONFIG_DEBUG_MODE 1

#if APP_CONFIG_DEBUG_MODE
    #define LOAD_CSS(provider, path) gtk_css_provider_load_from_path(provider, path)
    #define CSS_PATH_LIGHT "../resources/css/index-light.css"
    #define CSS_PATH_DARK "../resources/css/index-dark.css"
#else
    #define LOAD_CSS(provider, path) gtk_css_provider_load_from_resource(provider, path)
    #define CSS_PATH_LIGHT "/com/pluvia/css/index-light.css"
    #define CSS_PATH_DARK "/com/pluvia/css/index-dark.css"
#endif


#endif
