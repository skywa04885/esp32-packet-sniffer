/*
  Copyright Cybox internet & communicatie 2020.
  Software door Luke A.C.A. Rieff
*/

#ifndef _IEEE80211_H
#define _IEEE80211_H

#include "default.h"

typedef enum {
  WIFI_CF_MGMT,
  WIFI_CF_CONTROL,
  WIFI_CF_DATA,
  WIFI_CF_EXT
} ieee80211_control_frame_type_t;

typedef enum {
  WIFI_ASSOC_REQ,
  WIFI_ASSOC_RES,
  WIFI_REASSOC_REQ,
  WIFI_REASSOC_RES,
  WIFI_PROBE_REQ,
  WIFI_PROBE_RES,
  __WIFI_RESERVED0,
  __WIFI_RESERVED1,
  WIFI_BEACON,
  WIFI_ATIM,
  WIFI_DISASSOC,
  WIFI_AUTH,
  WIFI_DEAUTH,
  WIFI_ACTION,
  WIFI_ACTION_NO_ACK
} ieee80211_control_mgmt_subtype_t;

typedef enum {
  __WIFI_RESERVED2,
  __WIFI_RESERVED3,
  WIFI_TRIGGER,
  WIFI_REPORT_POLL,
  WIFI_NDP_ANNOUNCE,
  WIFI_CONTROL_FRAME_EXTENSION,
  WIFI_CONTROL_WRAPPER,
  WIFI_BLOCK_ACK_REQUEST,
  WIFI_BLOCK_ACK,
  WIFI_PS_POLL,
  WIFI_RTS,
  WIFI_CTS,
  WIFI_ACK,
  WIFI_CF_END,
  WIFI_CF_END__CF_ACK
} ieee80211_control_ctr_subtype_t;

typedef struct __attribute__ ((packed)) {
  unsigned protocol : 2;
  unsigned type : 2;
  unsigned subtype : 4;
  unsigned to_ds : 1;
  unsigned from_ds : 1;
  unsigned more_frag : 1;
  unsigned retry : 1;
  unsigned pwr_mgmt : 1;
  unsigned more_data : 1;
  unsigned protected_frame : 1;
  unsigned order : 1;
} ieee80211_control_frame_t;

typedef struct __attribute__ ((packed)) {
  ieee80211_control_frame_t cf;
  uint16_t duration;
  uint8_t address1[6];
  uint8_t address2[6];
  uint8_t address3[6];
  uint16_t seq_ctl;
  uint8_t address4[6];
} ieee80211_data_mac_header_t;

typedef struct __attribute__ ((packed)) {
  ieee80211_data_mac_header_t hdr;
  char payload[0];
} ieee80211_data_packet_t;

typedef struct __attribute__ ((packed)) {
  ieee80211_control_frame_t cf;
  uint16_t duration;
  uint8_t destination[6];
  uint8_t transmitter[6];
  uint8_t bssid[6];
  uint16_t seq_ctl;
} ieee80211_management_mac_header_t;

typedef struct __attribute__ ((packed)) {
  ieee80211_management_mac_header_t hdr;
  char payload[0];
} ieee80211_management_packet_t;

typedef struct __attribute__ ((packed)) {
  uint8_t timestamp[8];
  uint16_t interval;
  uint16_t capability;
  uint8_t tag_number;
  uint8_t tag_length;
  char ssid[0];
} ieee80211_management_beacon_var_t;

typedef struct __attribute__ ((packed)) {
  ieee80211_control_frame_t cf;
  uint16_t duration;
  uint8_t destination[6];
  uint8_t transmitter[6];
} ieee80211_control_mac_header_t;

/**
 * Pretty-prints an packet to the log of the ESP32 chip,
 *  this will be used for debug purposes.
 */
void ieee80211_log_packet(void *buffer, wifi_promiscuous_pkt_type_t type);

/**
 * Formats an MAC address into a string, this will be used
 *  for pretty-printing it to the console
 */
void ieee80211_mac_to_string(char *out, uint8_t *in);

/**
 * Turns an pair of numbers into human readable text which will
 *  contain the type and subtype of an type/subtype pair
 */
const char *ieee80211_get_type_string(wifi_promiscuous_pkt_type_t type);
const char *ieee80211_get_mgmt_subtype_string(ieee80211_control_mgmt_subtype_t type);
const char *ieee80211_get_ctrl_subtype_string(ieee80211_control_ctr_subtype_t type);

#endif
