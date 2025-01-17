#pragma once
#include "esp_http_server.h"
#include "esp_ota_ops.h"
#include "esp_log.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))


//TODO CHECK_HANDLER

#define EOTA_HANDLERS(concidional_funtion,handler_html,handler_statics) \
    {{"/ota.min.html", HTTP_GET , handler_html , NULL}, true, {ota_min_html_asm_start,ota_min_html_asm_end,"",NULL,NULL}}, \
    {{"/js/ota.min.js", HTTP_GET , handler_statics , NULL}, true, {ota_min_js_asm_start,ota_min_js_asm_end,"text/javascript",NULL,NULL}}, \
    {{"/ota_update", HTTP_POST, concidional_funtion, NULL}, true, {.uri_execution_function = ota_post_handler}} \

extern const char ota_min_html_asm_start[] asm("_binary_ota_min_html_start");
extern const char ota_min_html_asm_end[] asm("_binary_ota_min_html_end");

extern const char ota_min_css_asm_start[] asm("_binary_ota_min_css_start");
extern const char ota_min_css_asm_end[] asm("_binary_ota_min_css_end");

extern const char ota_min_js_asm_start[] asm("_binary_ota_min_js_start");
extern const char ota_min_js_asm_end[] asm("_binary_ota_min_js_end");

extern volatile bool OTA_BOOL;
esp_err_t ota_post_handler(httpd_req_t *req);
