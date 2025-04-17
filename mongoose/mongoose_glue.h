// SPDX-FileCopyrightText: 2024 Cesanta Software Limited
// SPDX-License-Identifier: GPL-2.0-only or commercial
// Generated by Mongoose Wizard, https://mongoose.ws/wizard/

#ifndef MONGOOSE_GLUE_H
#define MONGOOSE_GLUE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mongoose.h"

#define WIZARD_ENABLE_HTTP 1
#define WIZARD_ENABLE_HTTPS 1
#define WIZARD_ENABLE_HTTP_UI 1
#define WIZARD_ENABLE_HTTP_UI_LOGIN 0

#define WIZARD_ENABLE_WEBSOCKET 0

#define WIZARD_ENABLE_MQTT 0
#define WIZARD_MQTT_URL ""

#define WIZARD_ENABLE_SNTP 0  // Enable time sync.
#define WIZARD_SNTP_TYPE 0    // 0: default Google, 1: DHCP, 2: custom
#define WIZARD_SNTP_URL "udp://time.google.com:123"  // Custom SNTP server URL
#define WIZARD_SNTP_INTERVAL_SECONDS 3600            // Frequency of SNTP syncs

#define WIZARD_DNS_TYPE 0  // 0: default Google, 1: DHCP, 2: custom
#define WIZARD_DNS_URL "udp://8.8.8.8:53"  // Custom DNS server URL
#define WIZARD_CAPTIVE_PORTAL 0
#define WIZARD_ENABLE_MDNS 0
#define WIZARD_MDNS_NAME ""

#define WIZARD_ENABLE_MODBUS 0
#define WIZARD_MODBUS_PORT 502

#ifndef WIZARD_REBOOT_TIMEOUT_MS
#define WIZARD_REBOOT_TIMEOUT_MS 500
#endif

void mongoose_init(void);    // Initialise Mongoose
void mongoose_poll(void);    // Poll Mongoose
extern struct mg_mgr g_mgr;  // Mongoose event manager

void mongoose_set_http_handlers(const char *name, ...);
void mongoose_add_ws_handler(unsigned ms, void (*)(struct mg_connection *));

struct mongoose_mqtt_handlers {
  struct mg_connection *(*connect_fn)(mg_event_handler_t);
  void (*tls_init_fn)(struct mg_connection *);
  void (*on_connect_fn)(struct mg_connection *, int);
  void (*on_message_fn)(struct mg_connection *, struct mg_str, struct mg_str);
  void (*on_cmd_fn)(struct mg_connection *, struct mg_mqtt_message *);
};
void mongoose_set_mqtt_handlers(struct mongoose_mqtt_handlers *);

struct mongoose_modbus_handlers {
  bool (*read_reg_fn)(uint16_t address, uint16_t *value);
  bool (*write_reg_fn)(uint16_t address, uint16_t value);
};
void mongoose_set_modbus_handlers(struct mongoose_modbus_handlers *);

#define run_mongoose() \
  do {                 \
    mongoose_init();   \
    for (;;) {         \
      mongoose_poll(); \
    }                  \
  } while (0)

#if WIZARD_ENABLE_MQTT
void glue_lock_init(void);  // Initialise global Mongoose mutex
void glue_lock(void);       // Lock global Mongoose mutex
void glue_unlock(void);     // Unlock global Mongoose mutex
#else
#define glue_lock_init()
#define glue_lock()
#define glue_unlock()
#endif

// Increment device change state counter - trigger UI refresh
void glue_update_state(void);

// Firmware Glue

struct leds {
  bool led1;
};
void glue_get_leds(struct leds *);
void glue_set_leds(struct leds *);


#ifdef __cplusplus
}
#endif
#endif  // MONGOOSE_GLUE_H
