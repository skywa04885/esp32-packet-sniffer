/*
  Copyright Cybox internet & communicatie 2020-[End of humanity], all rights reserved
*/

#ifndef _FC_IEEE802_11_H
#define _FC_IEEE802_11_H

#define WIFI_FRAME_FCS_SIZE 4

#include <stdint.h>

typedef enum {
  WIFI_CF_DATA,
  WIFI_CF_CONTROL,
  WIFI_CF_MGMT,
  WIFI_CF_EXTENSION
} wifi_cf_type_t;

#define WIFI_CF_TYPE_CONTROL_BITS 0b01
#define WIFI_CF_TYPE_MGMT_BITS 0b00
#define WIFI_CF_TYPE_DATA_BITS 0b10
#define WIFI_CF_TYPE_EXTS_BITS 0b11

typedef enum {
  WIFI_CF_SUBTYPE_CTS,
  WIFI_CF_SUBTYPE_RTS,
  WIFI_CF_SUBTYPE_DATA,
  WIFI_CF_SUBTYPE_ACK,
  WIFI_CF_SUBTYPE_PSPOLL
} wifi_cf_subtype_t;

typedef uint16_t wifi_cf_t;

#define WIFI_CF_TO_DS_BIT             _BV(8)
#define WIFI_CF_FROM_DS_BIT           _BV(9)
#define WIFI_CF_MORE_FRAG_BIT         _BV(10)
#define WIFI_CF_RETRY_BIT             _BV(11)
#define WIFI_CF_POWER_MANAGEMENT_BIT  _BV(12)
#define WIFI_CF_MORE_DATA_BIT         _BV(13)
#define WIFI_CF_PROTECTED_BIT         _BV(14)
#define WIFI_CF_ORDER_BIT             _BV(15)

#define WIFI_SUBTYPE_PSPOLL_BITS      0b0101
#define WIFI_SUBTYPE_ACK_BITS         0b1011
#define WIFI_SUBTYPE_DATA_BITS        0b0000
#define WIFI_SUBTYPE_RTS_BITS         0b1101
#define WIFI_SUBTYPE_CTS_BITS         0b0011

#endif
