#pragma once
#include "esp_http_server.h"
#include "esp_ota_ops.h"
#include "esp_log.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))


//TODO CHECK_HANDLER

#define EOTA_HANDLERS(concidional_funtion,handler_html,handler_statics) \
    {{"/ota.html", HTTP_GET , handler_html , NULL}, true, {ota_html_asm_start,ota_html_asm_end,"",NULL}}, \
    {{"/css/ota.css", HTTP_GET , handler_statics , NULL}, true, {ota_css_asm_start,ota_css_asm_end,"text/css",NULL}}, \
    {{"/js/ota.js", HTTP_GET , handler_statics , NULL}, true, {ota_js_asm_start,ota_js_asm_end,"text/javascript",NULL}}, \
    {{"/ota_update", HTTP_POST, concidional_funtion, NULL}, true, {.uri_handler_function = ota_post_handler}}, \
    // {{"/ota_status", HTTP_POST, concidional_funtion, NULL}, true, {.uri_handler_function = ota_status_post_handler}}


enum eota_status{
    NO_OTA,
    GET_PARTITION,
    BEGIN,
    RECIV,
    END,
    SET_PARTITION,
    OK_RESET,
};



extern const char ota_html_asm_start[] asm("_binary_ota_html_start");
extern const char ota_html_asm_end[] asm("_binary_ota_html_end");

extern const char ota_css_asm_start[] asm("_binary_ota_css_start");
extern const char ota_css_asm_end[] asm("_binary_ota_css_end");

extern const char ota_js_asm_start[] asm("_binary_ota_js_start");
extern const char ota_js_asm_end[] asm("_binary_ota_js_end");

extern volatile bool OTA_BOOL;
extern volatile enum eota_status OTA_STATUS;
extern volatile int status_value;

esp_err_t ota_status_post_handler(httpd_req_t *req);
esp_err_t ota_post_handler(httpd_req_t *req);
