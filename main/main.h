// #define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
/*
  Copyright Cybox internet & communicatie 2020-[End of humanity], all rights reserved
*/

#ifndef _MAIN_H
#define _MAIN_H

// ===================================================
// C STD Library
// ===================================================

#include <stdio.h>

// ===================================================
// ESP32-IDF STD Library
// ===================================================

#include <string.h>
#include <sdkconfig.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
#include <esp_spi_flash.h>
#include <nvs_flash.h>
#include <esp_task_wdt.h>
#include <esp_int_wdt.h>
#include <esp_wifi.h>
#include <esp_log.h>
#include "esp_task_wdt.h"

// ===================================================
// Macros
// ===================================================

#define CHANNEL_SWITCH_DELAY_MS 10
#define SNIFFER_TAG "SNIFF_SNIFF"
#define APP_MAIN_TAG "APP_MAIN"
#define FREQ_TAG "SNIFF_FREQ"
#define SNIFFER_DEFAULT(V) V.filter.filter_mask = WIFI_PROMIS_FILTER_MASK_MGMT;    \
V.channel = 2
#define CHECK_ERROR_CODE(A, B, C) \
if (B != C) { \
  ESP_LOGE(A, "TWDT Error !"); \
  abort(); \
}

// ===================================================
// Structs / Enums
// ===================================================

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

typedef struct  {
  uint8_t mac[6];
} sniffer_ram_pkt_t;

typedef struct {
  wifi_promiscuous_filter_t filter;
  wifi_ant_gpio_config_t antenna;
  uint32_t channel;
} sniffer_t;

// ===================================================
// Prrototypes
// ===================================================

void app_main(void);

#endif
