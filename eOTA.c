#include "eOTA.h"

volatile bool OTA_BOOL = false;

void _ota_delayed_restart_task(void *pvParameter) {
    vTaskDelay(pdMS_TO_TICKS(1000));

    esp_restart();

    vTaskDelete(NULL);
}

esp_err_t ota_post_handler(httpd_req_t *req) {
    OTA_BOOL = true;
    esp_ota_handle_t ota_handle;
    const esp_partition_t *ota_partition = esp_ota_get_next_update_partition(NULL);
    if (ota_partition == NULL) {
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to find OTA partition");
        OTA_BOOL = false;
        return ESP_FAIL;
    }

    esp_err_t err = esp_ota_begin(ota_partition, OTA_SIZE_UNKNOWN, &ota_handle);
    if (err != ESP_OK) {
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "OTA begin failed");
        OTA_BOOL = false;
        return err;
    }
    
    char buf[1024];
    int remaining = req->content_len;
    while (remaining > 0) {
        int recv_len = httpd_req_recv(req, buf, MIN(remaining, sizeof(buf)));
        if (recv_len <= 0) {
            esp_ota_end(ota_handle);
            httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "OTA receive failed");
            OTA_BOOL = false;
            return err;
        }
        err = esp_ota_write(ota_handle, buf, recv_len);
        if (err != ESP_OK) {
            esp_ota_end(ota_handle);
            httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "OTA write failed");
            OTA_BOOL = false;
            return err;
        }
        remaining -= recv_len;
    }

    err = esp_ota_end(ota_handle);
    if (err != ESP_OK) {
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "OTA end failed");
        OTA_BOOL = false;
        return err;
    }

    err = esp_ota_set_boot_partition(ota_partition);
    if (err != ESP_OK) {
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "OTA set boot partition failed");
        OTA_BOOL = false;
        return err;
    }

    httpd_resp_sendstr(req, "OTA update successful. Rebooting...");
    xTaskCreate(&_ota_delayed_restart_task, "delayed_restart", 2048, NULL, 5, NULL);
    return ESP_OK;
}
