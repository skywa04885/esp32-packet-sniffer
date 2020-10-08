/*
  Copyright Cybox internet & communicatie 2020.
  Software door Luke A.C.A. Rieff
*/

#include "nvs.h"

void nvs_init(void) {
  // Initializes the non volatile flash memory, after which we check if there is an error,
  //  if so if it can be fixed, else perform final check and panic.
  esp_err_t err = nvs_flash_init();
  if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    err = nvs_flash_init();
  } 

  ESP_ERROR_CHECK(err);
}