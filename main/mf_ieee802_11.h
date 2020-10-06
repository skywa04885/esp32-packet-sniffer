/*
  Copyright Cybox internet & communicatie 2020-[End of humanity], all rights reserved
*/

#ifndef _MF_IEEE802_11_H
#define _MF_IEEE802_11_H

#include "fc_ieee802_11.h"

// ===================================================
// Management Frame
// ===================================================

typedef struct __attribute__ ((packed)) {
  wifi_cf_t frame_control;
  uint16_t  duration_id;
  uint8_t   destination[6];
  uint8_t   sender[6];
  uint8_t   bssid[6];
  uint16_t  seq_ctl;
} wifi_management_frame_t;

#endif
