/*
  Copyright Cybox internet & communicatie 2020.
  Software door Luke A.C.A. Rieff
*/

#ifndef _MAIN_H
#define _MAIN_H

#include "default.h"

/**
 * Basically the entry point for the application, will be called
 *  once FreeRTOS has been started.
 */
void app_main(void);

/**
 * Handles an packet received by the ESP32 chip in promisc mode
 *  this will print or store it.
 */
void promisc_packet_cb(void *buffer, wifi_promiscuous_pkt_type_t type);

#endif
