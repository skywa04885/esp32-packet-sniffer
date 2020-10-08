/*
  Copyright Cybox internet & communicatie 2020.
  Software door Luke A.C.A. Rieff
*/

#include "main.h"
#include "ieee80211.h"
#include "wifi.h"
#include "nvs.h"

static wifi_promiscuous_filter_t g_WifiPromiscuousFilter = {
  .filter_mask = WIFI_PROMIS_FILTER_MASK_ALL
};

void promisc_packet_cb(void *buffer, wifi_promiscuous_pkt_type_t type) {
  ieee80211_log_packet(buffer, type);
}

void promisc_channel_switch_task(void *u) {
  uint8_t s_nFirstChannel = 1;

  if (esp_task_wdt_add(NULL) != ESP_OK) {
    ESP_LOGE(CHANNEL_SWITCHER_TAG, "esp_task_wdt_add() failed");
    abort();
  } else if (esp_task_wdt_status(NULL) != ESP_OK) {
    ESP_LOGE(CHANNEL_SWITCHER_TAG, "esp_task_wdt_status() failed");
    abort();
  }

  while (true) {
    // Switches the first channel between the most common frequency's
    //  to not miss anything
    if (s_nFirstChannel == 1) s_nFirstChannel = 6;
    else if (s_nFirstChannel == 6) s_nFirstChannel = 11;
    else s_nFirstChannel = 1;

    // Updates the channel of the first and second antenna
    //  and checks for errors
    ESP_ERROR_CHECK(esp_wifi_set_channel(s_nFirstChannel, WIFI_SECOND_CHAN_NONE));

    // Resets the RTOS watchdog and adds some delay until 
    //  we switch the channel again
    vTaskDelay(15 / portTICK_PERIOD_MS);
    if (esp_task_wdt_reset() != ESP_OK) {
      ESP_LOGE(CHANNEL_SWITCHER_TAG, "esp_task_wdt_reset() failed");
      abort();
    }
  }

  vTaskDelete(NULL);
}

void app_main(void) {
  TaskHandle_t channel_switch_task_handle;

  nvs_init();
  wifi_init(WIFI_MODE_NULL);
  wifi_promisc_init(&g_WifiPromiscuousFilter, &promisc_packet_cb);

  xTaskCreate(&promisc_channel_switch_task, "ChannelSwitcher", 2048, NULL, 0, &channel_switch_task_handle);
}
