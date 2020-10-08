/*
  Copyright Cybox internet & communicatie 2020.
  Software door Luke A.C.A. Rieff
*/

#ifndef _WIFI_H
#define _WIFI_H

#include "default.h"

/**
 * Initializes the ESP32-WiFi module, so that we will be capable
 *  of using it for a packet sniffer.
 */
void wifi_init(wifi_mode_t mode);

/**
 * Configures the WiFi module to use promiscuois mode, after which
 *  we set the filter for the packets.
 */
void wifi_promisc_init(wifi_promiscuous_filter_t *filter, wifi_promiscuous_cb_t cb);

#endif