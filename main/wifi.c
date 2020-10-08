/*
  Copyright Cybox internet & communicatie 2020.
  Software door Luke A.C.A. Rieff
*/

#include "wifi.h"

void wifi_init(wifi_mode_t mode) {
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();      // Gets the default WiFi configuration

  ESP_ERROR_CHECK(esp_event_loop_create_default());         // Creates the default event loop ( for driver or something )
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));                     // Initializes the WiFi driver
  ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));  // Sets the WiFi storage mode to RAM
  ESP_ERROR_CHECK(esp_wifi_set_mode(mode));                 // Sets the WiFi mode to the one specified
}

void wifi_promisc_init(wifi_promiscuous_filter_t *filter, wifi_promiscuous_cb_t cb) {
  ESP_ERROR_CHECK(esp_wifi_set_promiscuous(true));
  ESP_ERROR_CHECK(esp_wifi_set_promiscuous_filter(filter));
  ESP_ERROR_CHECK(esp_wifi_set_promiscuous_rx_cb(cb));
}