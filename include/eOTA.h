#pragma once
#include "esp_http_server.h"
#include "esp_ota_ops.h"
#include "esp_log.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define OTA_HANDLERS(handler) \
    {{"/ota.html", HTTP_GET , handler , NULL}, true, {ota_html_asm_start,ota_html_asm_end,""}}, \
    {{"/ota", HTTP_POST, ota_post_handler, NULL}, false, {}}

extern const char ota_html_asm_start[] asm("_binary_ota_html_start");
extern const char ota_html_asm_end[] asm("_binary_ota_html_end");
extern volatile bool OTA_BOOL;

esp_err_t ota_post_handler(httpd_req_t *req);
