#pragma once
#include "esp_http_server.h"
#include "esp_ota_ops.h"

esp_err_t ota_post_handler(httpd_req_t *req);

