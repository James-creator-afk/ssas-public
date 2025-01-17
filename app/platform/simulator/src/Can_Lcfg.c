/**
 * SSAS - Simple Smart Automotive Software
 * Copyright (C) 2021 Parai Wang <parai@foxmail.com>
 */
/* ================================ [ INCLUDES  ] ============================================== */
#include "Can_Lcfg.h"
#include "string.h"
/* ================================ [ MACROS    ] ============================================== */
/* ================================ [ TYPES     ] ============================================== */
/* ================================ [ DECLARES  ] ============================================== */
/* ================================ [ DATAS     ] ============================================== */
/* ================================ [ LOCALS    ] ============================================== */
/* ================================ [ FUNCTIONS ] ============================================== */
Can_ChannelConfigType Can_ChannelConfigs[] = {
  {
    "simulator",
    0,
    500000,
  },
  {
    "simulator",
    1,
    500000,
  },
  {
    "simulator",
    2,
    500000,
  },
  {
    "simulator",
    3,
    500000,
  },
};

Can_ConfigType Can_Config = {
  ".can.log",
  Can_ChannelConfigs,
  ARRAY_SIZE(Can_ChannelConfigs),
};

void Can_ReConfig(uint8_t Controller, const char *device, int port, uint32_t baudrate) {
  Can_ChannelConfigType *config;
  if (Controller < Can_Config.numOfChannels) {
    config = &Can_Config.channelConfigs[Controller];
    strncpy(config->device, device, sizeof(config->device));
    config->port = port;
    config->baudrate = baudrate;
  }
}
