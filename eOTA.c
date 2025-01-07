#include "eOTA.h"


extern const char ota_html_asm_start[] asm("_binary_ota_html_start");
extern const char ota_html_asm_end[] asm("_binary_ota_html_end");

esp_err_t ota_post_handler(httpd_req_t *req) {
    esp_ota_handle_t ota_handle;
    const esp_partition_t *ota_partition = esp_ota_get_next_update_partition(NULL);
    if (ota_partition == NULL) {
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to find OTA partition");
        return ESP_FAIL;
    }

    esp_err_t err = esp_ota_begin(ota_partition, OTA_SIZE_UNKNOWN, &ota_handle);
    if (err != ESP_OK) {
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "OTA begin failed");
        return ESP_FAIL;
    }

    char buf[1024];
    int remaining = req->content_len;
    while (remaining > 0) {
        int recv_len = httpd_req_recv(req, buf, MIN(remaining, sizeof(buf)));
        if (recv_len <= 0) {
            esp_ota_end(ota_handle);
            httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "OTA receive failed");
            return ESP_FAIL;
        }
        err = esp_ota_write(ota_handle, buf, recv_len);
        if (err != ESP_OK) {
            esp_ota_end(ota_handle);
            httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "OTA write failed");
            return ESP_FAIL;
        }
        remaining -= recv_len;
    }

    err = esp_ota_end(ota_handle);
    if (err != ESP_OK) {
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "OTA end failed");
        return ESP_FAIL;
    }

    err = esp_ota_set_boot_partition(ota_partition);
    if (err != ESP_OK) {
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "OTA set boot partition failed");
        return ESP_FAIL;
    }

    httpd_resp_sendstr(req, "OTA update successful, rebooting...");
    esp_restart();
    return ESP_OK;
}
