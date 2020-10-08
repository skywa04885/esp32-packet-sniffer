#ifndef _DEFAULT_H
#define _DEFAULT_H

// ==== C STD ====
#include <stdio.h>
#include <stdint.h>
#include <string.h>

// ==== FreeRTOS ====
#include <freertos/FreeRTOS.h>

// ==== ESP32 ====
#include <sdkconfig.h>
#include <esp_system.h>
#include <esp_spi_flash.h>
#include <esp_wifi.h>
#include <nvs_flash.h>
#include <esp_task_wdt.h>
#include <esp_log.h>

// ==== Global definitions ====
#ifdef DEBUG
#define DEBUG_ONLY(A) A
#else
#define DEBUG_ONLY(A)
#endif

#define MAIN_TAG "Main"
#define CHANNEL_SWITCHER_TAG "ChannelSwitcher"
#define SNIFFER_TAG "Sniffer"

#endif
