/*
  Copyright Cybox internet & communicatie 2020-[End of humanity], all rights reserved
*/

#ifndef _DF_IEEE802_11_H
#define _DF_IEEE802_11_H

#include <stdint.h>

#include "fc_ieee802_11.h"

// ===================================================
// WiFi Data frame
// ===================================================

typedef struct __attribute__ ((packed)) {
  wifi_cf_t frame_control;
  uint16_t  duration_id;
  uint8_t   address1[6];    // -> Receiver
  uint8_t   address2[6];    // -> Sender
  uint8_t   address3[6];    // -> Filtering
  uint16_t  seg_ctl;
  uint8_t   address4[6];    // -> Optional
} wifi_dataframe_t;

// ===================================================
// WiFi RTS frame ( Request to send )
// ===================================================

typedef struct __attribute__ ((packed)) {
  wifi_cf_t frame_control;
  uint16_t  duration_id;
  uint8_t   receiver[6];
  uint8_t   transmitter[6];
} wifi_rts_frame_t;

// ===================================================
// WiFi CTS frame ( Clear to send )
// ===================================================

typedef struct __attribute__ ((packed)) {
  wifi_cf_t frame_contro;
  uint16_t  duration_id;
  uint8_t   receiver[6];
} wifi_cts_frame_t;

// ===================================================
// WiFi ACK frame ( Acknowledgement )
// ===================================================

typedef struct __attribute__ ((packed)) {
  wifi_cf_t frame_control;
  uint16_t  duration_id;
  uint8_t   receiver[6];
} wifi_ack_frame_t;

// ===================================================
// WiFi PS-Poll ( Power-Save Poll )
// ===================================================

typedef struct __attribute__ ((packed)) {
  wifi_cf_t frame_control;
  uint16_t  duration_id;
  uint8_t   bssid[6];
  uint8_t   transmitter[6];
} wifi_ps_poll_frame_t;

#endif
