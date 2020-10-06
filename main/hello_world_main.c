/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "nvs_flash.h"
#include "esp_wifi.h"

typedef struct {
  uint16_t  frame_control;
  uint16_t  duration_id;
  uint8_t   address1[6];
  uint8_t   address2[6];
  uint8_t   address3[6];
  uint16_t  seqctl;
  uint8_t   address4[6];
  // The rest can be anything
} sniffer_802_frame_t;

typedef struct {
  wifi_promiscuous_filter_t filter;
  wifi_ant_gpio_config_t antenna;
  uint32_t channel;
} sniffer_t;

#define SNIFFER_DEFAULT(V) sniffer_t V; \
V.filter.filter_mask = WIFI_PROMIS_FILTER_MASK_MGMT; \
V.channel = 2

/**
 * This callback will be called from the driver, each time when a packet
 *  is received and filtered
 */
void sniffer_pkt_cb(void *buffer, wifi_promiscuous_pkt_type_t type) {
  wifi_promiscuous_pkt_t *packet = (wifi_promiscuous_pkt_t *) buffer;
  sniffer_802_frame_t *frame = (sniffer_802_frame_t *) packet->payload;

  printf("Packet of type [%d] {\n", type);
  printf("\tLen: %d\n", packet->rx_ctrl.sig_len);
  printf("\tChannel: %d\n", packet->rx_ctrl.channel);
  printf("\tSender: %02x:%02x:%02x:%02x:%02x:%02x\n", frame->address2[0], frame->address2[1],
    frame->address2[2], frame->address2[3], frame->address2[4], frame->address2[5]);
  printf("\tReceiver: %02x:%02x:%02x:%02x:%02x:%02x\n", frame->address1[0], frame->address1[1],
    frame->address1[2], frame->address1[3], frame->address1[4], frame->address1[5]);
  printf("}\n");
  fflush(stdout);
}

/**
 * Initializes an packet sniffer, and performs the configuration based
 *   uppon the variables in the sniffer_t structure
 */
void sniffer_init(sniffer_t *sniffer) {
  esp_wifi_set_promiscuous(true);                         // Enables promiscuous mode
  esp_wifi_set_promiscuous_filter(&sniffer->filter);      // Sets the sniffer filter ( management by default )
  esp_wifi_set_promiscuous_rx_cb(&sniffer_pkt_cb);        // Sets the sniffer's packet callback
  esp_wifi_set_channel(sniffer->channel, WIFI_SECOND_CHAN_NONE);

  esp_wifi_set_ant(WIFI_ANT_MODE_ANT0);                   // Select antenna 0 for RX/TX
}

static void sniffer_channel_switch(sniffer_t *sniffer) {
  if (sniffer->channel >= 14) sniffer->channel = 1;
  else ++sniffer->channel;
  esp_wifi_set_channel(sniffer->channel, WIFI_SECOND_CHAN_NONE);
}

/**
 * Stops the sniffer, and frees memory used by it, so we can
 *  perform any other task, without undefined behaviour
 */
void sniffer_stop(sniffer_t *sniffer) {
  esp_wifi_set_promiscuous(false);                        // Disables promiscous mode
}

/**
 * Initializes the NVS ( Non volatile storage ) this is requried
 *   for the WiFI/BT Module
 */
static void nvs_init() {
  esp_err_t err = nvs_flash_init();
  if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    err = nvs_flash_init();
  }

  ESP_ERROR_CHECK(err);
}

/**
 * Initializes the wifi chip for the packet sniffer, this will have to be called
 *  once at the startup of the device
 */
static void sniffer_wifi_init() {
  ESP_ERROR_CHECK(esp_event_loop_create_default());
  wifi_init_config_t config = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&config));                // Initializes the wifi chip with default config
  ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));// Sets the storage to ram, since nothing will go to disk
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_NULL));     // The WiFi module will not be as SoftAP or Client, but sniffer mode
}

void app_main(void) {
  nvs_init();
  sniffer_wifi_init();

  SNIFFER_DEFAULT(sniffer);
  sniffer_init(&sniffer);

  // xTaskCreate(
  //   &frequency_switcher_task, "FrequencySwitcher", 1024, 
  //   NULL, 2, &frequency_switcher_task_handle
  // );

  printf("Task scheduler started !");
  fflush(stdout);

  for (;;) {
    sniffer_channel_switch(&sniffer);
    vTaskDelay(4);
  }
}