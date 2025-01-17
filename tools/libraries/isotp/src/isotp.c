/**
 * SSAS - Simple Smart Automotive Software
 * Copyright (C) 2021 Parai Wang <parai@foxmail.com>
 */
/* ================================ [ INCLUDES  ] ============================================== */
#include "isotp.h"
#include "isotp_types.h"
#include <pthread.h>
/* ================================ [ MACROS    ] ============================================== */
#define AS_LOG_ISOTP 0
#define AS_LOG_ISOTPI 2
#define AS_LOG_ISOTPE 3
/* ================================ [ TYPES     ] ============================================== */

/* ================================ [ DECLARES  ] ============================================== */
extern isotp_t *isotp_can_create(isotp_parameter_t *params);
extern int isotp_can_transmit(isotp_t *isotp, const uint8_t *txBuffer, size_t txSize,
                              uint8_t *rxBuffer, size_t rxSize);
int isotp_can_receive(isotp_t *isotp, uint8_t *rxBuffer, size_t rxSize);
extern void isotp_can_destory(isotp_t *isotp);

extern isotp_t *isotp_lin_create(isotp_parameter_t *params);
extern int isotp_lin_transmit(isotp_t *isotp, const uint8_t *txBuffer, size_t txSize,
                              uint8_t *rxBuffer, size_t rxSize);
int isotp_lin_receive(isotp_t *isotp, uint8_t *rxBuffer, size_t rxSize);
extern void isotp_lin_destory(isotp_t *isotp);

/* ================================ [ DATAS     ] ============================================== */
static pthread_mutex_t lMutex = PTHREAD_MUTEX_INITIALIZER;
/* ================================ [ LOCALS    ] ============================================== */
/* ================================ [ FUNCTIONS ] ============================================== */
isotp_t *isotp_create(isotp_parameter_t *params) {
  isotp_t *isotp = NULL;

  pthread_mutex_lock(&lMutex);
  switch (params->protocol) {
  case ISOTP_OVER_CAN:
    isotp = isotp_can_create(params);
    break;
#ifdef USE_LINTP
  case ISOTP_OVER_LIN:
    isotp = isotp_lin_create(params);
    break;
#endif
  default:
    break;
  }
  pthread_mutex_unlock(&lMutex);

  return isotp;
}

int isotp_transmit(isotp_t *isotp, const uint8_t *txBuffer, size_t txSize, uint8_t *rxBuffer,
                   size_t rxSize) {
  int r = -1;
  pthread_mutex_lock(&lMutex);
  switch (isotp->params->protocol) {
  case ISOTP_OVER_CAN:
    r = isotp_can_transmit(isotp, txBuffer, txSize, rxBuffer, rxSize);
    break;
#ifdef USE_LINTP
  case ISOTP_OVER_LIN:
    r = isotp_lin_transmit(isotp, txBuffer, txSize, rxBuffer, rxSize);
    break;
#endif
  default:
    break;
  }
  pthread_mutex_unlock(&lMutex);
  return r;
}

int isotp_receive(isotp_t *isotp, uint8_t *rxBuffer, size_t rxSize) {
  int r = -1;
  pthread_mutex_lock(&lMutex);
  switch (isotp->params->protocol) {
  case ISOTP_OVER_CAN:
    r = isotp_can_receive(isotp, rxBuffer, rxSize);
    break;
#ifdef USE_LINTP
  case ISOTP_OVER_LIN:
    r = isotp_lin_receive(isotp, rxBuffer, rxSize);
    break;
#endif
  default:
    break;
  }
  pthread_mutex_unlock(&lMutex);
  return r;
}

void isotp_destory(isotp_t *isotp) {
  pthread_mutex_lock(&lMutex);
  switch (isotp->params->protocol) {
  case ISOTP_OVER_CAN:
    isotp_can_destory(isotp);
    break;
#ifdef USE_LINTP
  case ISOTP_OVER_LIN:
    isotp_lin_destory(isotp);
    break;
#endif
  default:
    break;
  }
  pthread_mutex_unlock(&lMutex);
}
