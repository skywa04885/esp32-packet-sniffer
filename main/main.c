/*
  Copyright Cybox internet & communicatie 2020-[End of humanity], all rights reserved
*/

#include "main.h"

static sniffer_ram_pkt_t *__packetBuffer = NULL;
static uint32_t __packetBufferSize = 0, __packetBufferAllocSize = 40;
static sniffer_t sniffer;

/**
 * This callback will be called from the driver, each time when a packet
 *  is received and filtered
 */
static void sniffer_pkt_cb(void *buffer, wifi_promiscuous_pkt_type_t type) {
  wifi_promiscuous_pkt_t *packet = (wifi_promiscuous_pkt_t *) buffer;
  sniffer_802_frame_t *frame = (sniffer_802_frame_t *) packet->payload;

  // Checks the size of the buffer, if it is at its limit or over it
  //  we want to clear it, and send the data over lora to the server
  if (__packetBufferSize >= __packetBufferAllocSize) {
    ESP_LOGD(SNIFFER_TAG, "Buffer of packets is full, transmitting and clearing ..");
    __packetBufferSize = 0;
  }

  // Loggs the packet information to the console, although this only happens
  //  when we're compiling in debug mode
  ESP_LOGD(SNIFFER_TAG, "%d -> Packet of type [%d] {", packet->rx_ctrl.timestamp, type);
  ESP_LOGD(SNIFFER_TAG, "\tLen: %d", packet->rx_ctrl.sig_len);
  ESP_LOGD(SNIFFER_TAG, "\tChannel: %d", packet->rx_ctrl.channel);
  ESP_LOGD(SNIFFER_TAG, "\tSender: %02x:%02x:%02x:%02x:%02x:%02x", frame->address2[0], frame->address2[1],
    frame->address2[2], frame->address2[3], frame->address2[4], frame->address2[5]);
  ESP_LOGD(SNIFFER_TAG, "\tReceiver: %02x:%02x:%02x:%02x:%02x:%02x", frame->address1[0], frame->address1[1],
    frame->address1[2], frame->address1[3], frame->address1[4], frame->address1[5]);
  ESP_LOGD(SNIFFER_TAG, "}");

  // Loops over the array of packets, and checks if the currently specified mac address
  //  is already in there
  bool found = false;
  for (uint32_t i = 0; i < __packetBufferSize; ++i) {
    // Compares the packet segments, and if it does not match
    //  just continue with the next one
    sniffer_ram_pkt_t pkt_i = __packetBuffer[i];
    if (pkt_i.mac[0] != frame->address2[0]) continue;
    if (pkt_i.mac[1] != frame->address2[1]) continue;
    if (pkt_i.mac[2] != frame->address2[2]) continue;
    if (pkt_i.mac[3] != frame->address2[3]) continue;
    if (pkt_i.mac[4] != frame->address2[4]) continue;
    if (pkt_i.mac[5] != frame->address2[5]) continue;

    // Since the mac was found, set found to true, so that we know
    //  we do not have to insert the address again
    found = true;
    break;
  }

  // Checks if the element was found, if not add it to the buffer
  //  and increment the count
  if (!found) {
    sniffer_ram_pkt_t ram_pkt;
    memcpy(ram_pkt.mac, frame->address2, 6);
    __packetBuffer[__packetBufferSize++] = ram_pkt;

    // Prints the debug message with the total size to the console
    ESP_LOGD(SNIFFER_TAG, "MAC address not found, added to buffer ..");
  }
}

/**
 * Initializes an packet sniffer, and performs the configuration based
 *   uppon the variables in the sniffer_t structure
 */
static void sniffer_init(sniffer_t *sniffer) {
  ESP_ERROR_CHECK(esp_wifi_set_promiscuous(true));                                  // Enables promiscuous mode
  ESP_ERROR_CHECK(esp_wifi_set_promiscuous_filter(&sniffer->filter));               // Sets the sniffer filter ( management by default )
  ESP_ERROR_CHECK(esp_wifi_set_promiscuous_rx_cb(&sniffer_pkt_cb));                 // Sets the sniffer's packet callback
  esp_wifi_set_channel(sniffer->channel, WIFI_SECOND_CHAN_NONE);                    // Sets the channel to default
}

/**
 * Performs an WiFI channel switch, this will stay between the range of 1
 *  and 14
 */
static void sniffer_channel_switch(sniffer_t *sniffer) {
  if (sniffer->channel >= 14) sniffer->channel = 1;
  else ++sniffer->channel;
  esp_wifi_set_channel(sniffer->channel, WIFI_SECOND_CHAN_NONE);
}

/**
 * Initializes the NVS ( Non volatile storage ) this is requried
 *   for the WiFI/BT Module
 */
static void nvs_init() {
  // Checks if the non volatile storage could be initialized, if not
  //  check if the error can be fixed, else just panic
  esp_err_t err = nvs_flash_init();
  if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    err = nvs_flash_init();
  }

  // Performs the final error check
  ESP_ERROR_CHECK(err);
}

static TaskHandle_t channel_switcher_handle;

/**
 * This task will switch the channel between 1 and 14, this will make sure
 *  that we capture all of the packets
 */
static void channel_switcher_task(void *u) {
  // Subscribe the current task to the watchdog, so we can later
  //  reset it
  CHECK_ERROR_CODE(APP_MAIN_TAG,esp_task_wdt_add(NULL), ESP_OK);
  CHECK_ERROR_CODE(APP_MAIN_TAG, esp_task_wdt_status(NULL), ESP_OK);

  // Starts the switching loop, each iteration we reset the watchdog
  //  and switch the channel
  while (true) {
    CHECK_ERROR_CODE(APP_MAIN_TAG, esp_task_wdt_reset(), ESP_OK);
    sniffer_channel_switch(&sniffer);
    fflush(stdout);
    vTaskDelay(CHANNEL_SWITCH_DELAY_MS / portTICK_PERIOD_MS);
  }
  
  vTaskDelete(NULL);
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
  ESP_LOGI(APP_MAIN_TAG, "Sniffer starting ..");

  // Performs memory allocation for the packet buffer, after which we check if it failed
  //  if it did throw an error, else we will proceed and print the success message
  __packetBuffer = malloc(__packetBufferAllocSize * sizeof (sniffer_ram_pkt_t));
  if (!__packetBuffer) {
    ESP_LOGE(APP_MAIN_TAG, "Allocation of %d ram packets of a total size %d bytes failed ..", __packetBufferAllocSize, 
      __packetBufferAllocSize * sizeof (sniffer_ram_pkt_t));
  }

  ESP_LOGI(APP_MAIN_TAG, "Allocation of %d ram packets of a total size %d bytes succeeded ..", __packetBufferAllocSize, 
    __packetBufferAllocSize * sizeof (sniffer_ram_pkt_t));

  // Initializes the non volatile storage, after which we initialize
  //  the WiFi module
  nvs_init();
  sniffer_wifi_init();
  ESP_LOGI(APP_MAIN_TAG, "NVS & WiFi initialized ..");

  // Creates the sniffer with the default config, after which we initialize
  //  the sniffer, which will start promiscuous mode
  SNIFFER_DEFAULT(sniffer);
  sniffer_init(&sniffer);
  ESP_LOGI(APP_MAIN_TAG, "Sniffer started ..");

  // Creates the channel switching task, this will have the first priority
  //  since we do not want to miss it
  xTaskCreate(&channel_switcher_task, "FreqTask", 1024, NULL, 0, &channel_switcher_handle);
}