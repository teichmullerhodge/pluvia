#include "nett.h"
#include <curl/curl.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <stdlib.h>
#include "../logger/logger.h"
#include <stdio.h>
#include <string.h>

static CURL *handler;
atomic_int initialized = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

CURL *get_handler(){

  (void)lock;
  if(initialized == 0){
    atomic_fetch_add(&initialized, 1);
    curl_global_init(CURL_GLOBAL_ALL);
  }

  handler = curl_easy_init();
  return handler;
  
}

bool nett_ok(HttpStatus status){
  return status >= HTTP_STATUS_OK && status < HTTP_STATUS_MULTIPLE_CHOICES;
}

void reset_response(NettResponse *res) {
  
  if(res){
   res->error = 0;
   if(res->contents != NULL) free(res->contents);
   res->contents = malloc(1);
   res->status_code = 0;
   res->contents_size = 0;
  
  }
}

NettResponse *response_init(){
  NettResponse *res = malloc(sizeof(NettResponse));
  if(res == NULL) {
    LOGGER_ERROR("Malloc failed at response_init.\n");
    return NULL;
  }

  res->contents = malloc(1);
  if(res->contents == NULL) {
    free(res);
    LOGGER_ERROR("Malloc failed at contents in response_init.\n");
    return NULL;
  }
  res->error = 0;
  res->status_code = 0;
  res->contents_size = 0;
  return res;
}

void destroy_response(NettResponse *res){

  if(res != NULL && res->contents != NULL){
    free(res->contents);
    free(res);
  }

}


size_t write_memory_callback(void *contents, size_t size, size_t nmemb, void *user_data){

  size_t real = size * nmemb;
  NettResponse * res = (NettResponse*)user_data;
  char *ptr = realloc(res->contents, res->contents_size + real + 1);
  if(ptr == NULL) {
    LOGGER_ERROR("Not enough memory (realloc returned NULL)\n");
    return 0;
  }

  res->contents = ptr;
  memcpy(&(res->contents[res->contents_size]), contents, real);
  res->contents_size += real;
  res->contents[res->contents_size] = '\0';
  return real;

}



void set_headers(NettHeaders *headers, struct curl_slist *h){
   if(headers == NULL) return;
   for(size_t k = 0; k < headers->len; k++){
     char full_header[1024];
     snprintf(full_header, sizeof(full_header), "%s: %s", headers->keys[k], headers->values[k]);
     h = curl_slist_append(h, full_header);
    }
   
}

void nett_get(const char *url, NettHeaders *headers, NettResponse *res){
  

  CURL *handler = get_handler();
  reset_response(res);
  if(handler == NULL) return;
  struct curl_slist *h = NULL;  
  set_headers(headers, h);
 
  curl_easy_setopt(handler, CURLOPT_HTTPHEADER, headers);
  curl_easy_setopt(handler, CURLOPT_URL, url);
  curl_easy_setopt(handler, CURLOPT_WRITEFUNCTION, write_memory_callback);
  curl_easy_setopt(handler, CURLOPT_WRITEDATA, (void*)res);
  CURLcode result = curl_easy_perform(handler);
  if(result != CURLE_OK){
      char error[512];
      snprintf(error, sizeof(error), "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
      LOGGER_ERROR(error);
      return;
  }
  long code = 0;
  curl_easy_getinfo(handler, CURLINFO_RESPONSE_CODE, &code);
  res->status_code = (HttpStatus)code;
  curl_easy_cleanup(handler);
  return; 

}









