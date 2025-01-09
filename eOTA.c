#include "eOTA.h"

volatile bool OTA_BOOL = false;

esp_err_t ota_post_handler(httpd_req_t *req) {
    ESP_LOGI("OTA", "OTA update started");
    OTA_BOOL = true;
    esp_ota_handle_t ota_handle;
    const esp_partition_t *ota_partition = esp_ota_get_next_update_partition(NULL);
    if (ota_partition == NULL) {
        ESP_LOGE("OTA", "Failed to find OTA partition");
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to find OTA partition");
        OTA_BOOL = false;
        return ESP_OK;
    }
    esp_err_t err = esp_ota_begin(ota_partition, OTA_SIZE_UNKNOWN, &ota_handle);
    if (err != ESP_OK) {
        ESP_LOGE("OTA", "OTA begin failed %i", err);
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "OTA begin failed");
        OTA_BOOL = false;
        return ESP_OK;
    }
    char buf[1024];
    int remaining = req->content_len;
    while (remaining > 0) {
        int recv_len = httpd_req_recv(req, buf, MIN(remaining, sizeof(buf)));
        if (recv_len <= 0) {
            ESP_LOGE("OTA", "OTA receive failed");
            esp_ota_end(ota_handle);
            httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "OTA receive failed");
            OTA_BOOL = false;
            return ESP_OK;
        }
        err = esp_ota_write(ota_handle, buf, recv_len);
        if (err != ESP_OK) {
            ESP_LOGE("OTA", "OTA write failed %i", err);
            esp_ota_end(ota_handle);
            httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "OTA write failed");
            OTA_BOOL = false;
            return ESP_OK;
        }
        remaining -= recv_len;
    }

    err = esp_ota_end(ota_handle);
    if (err != ESP_OK) {
        ESP_LOGE("OTA", "OTA end failed %i", err);
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "OTA end failed");
        OTA_BOOL = false;
        return ESP_OK;
    }

    err = esp_ota_set_boot_partition(ota_partition);
    if (err != ESP_OK) {
        ESP_LOGE("OTA", "OTA set boot partition failed %i", err);
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "OTA set boot partition failed");
        OTA_BOOL = false;
        return ESP_OK;
    }

    ESP_LOGI("OTA", "OTA update successful, rebooting...");
    httpd_resp_sendstr(req, "OTA update successful, rebooting...");
    OTA_BOOL = false;
    esp_restart();
    return ESP_OK;
}
