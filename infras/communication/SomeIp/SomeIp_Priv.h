/**
 * SSAS - Simple Smart Automotive Software
 * Copyright (C) 2021 Parai Wang <parai@foxmail.com>
 */
#ifndef _SOMEIP_PRIV_H
#define _SOMEIP_PRIV_H
/* ================================ [ INCLUDES  ] ============================================== */
#include "ComStack_Types.h"
#include "TcpIp.h"
#include "sys/queue.h"
/* ================================ [ MACROS    ] ============================================== */
/* ================================ [ TYPES     ] ============================================== */
/* API for service */
typedef void (*SomeIp_OnAvailabilityFncType)(boolean isAvailable);

/* for events */
typedef void (*SomeIp_OnSubscribeFncType)(boolean isSubscribe);

/* API for method */
typedef Std_ReturnType (*SomeIp_OnRequestFncType)(uint16_t conId, SomeIp_MessageType *req,
                                                  SomeIp_MessageType *res);
typedef Std_ReturnType (*SomeIp_OnFireForgotFncType)(uint16_t conId, SomeIp_MessageType *req);
typedef Std_ReturnType (*SomeIp_OnAsyncRequestFncType)(uint16_t conId, SomeIp_MessageType *res);

/* For the LF, set the msg->data as beginning of the buffer */
typedef Std_ReturnType (*SomeIp_OnTpCopyRxDataFncType)(uint16_t conId, SomeIp_TpMessageType *msg);

typedef Std_ReturnType (*SomeIp_OnTpCopyTxDataFncType)(uint16_t conId, SomeIp_TpMessageType *msg);

typedef Std_ReturnType (*SomeIp_OnResponseFncType)(SomeIp_MessageType *res);

/* API for events */
typedef Std_ReturnType (*SomeIp_OnNotificationFncType)(SomeIp_MessageType *evt);

typedef struct {
  uint16_t methodId;
  uint8_t interfaceVersion;
  SomeIp_OnRequestFncType onRequest;
  SomeIp_OnFireForgotFncType onFireForgot;
  SomeIp_OnAsyncRequestFncType onAsyncRequest;
  SomeIp_OnTpCopyRxDataFncType onTpCopyRxData;
  SomeIp_OnTpCopyTxDataFncType onTpCopyTxData;
  uint32_t resMaxLen;
} SomeIp_ServerMethodType;

typedef struct {
  uint16_t sdHandleID;
  uint16_t eventId;
  uint8_t interfaceVersion;
} SomeIp_ServerEventType;

typedef struct {
  uint16_t methodId;
  uint8_t interfaceVersion;
  SomeIp_OnResponseFncType onResponse;
  SomeIp_OnTpCopyRxDataFncType onTpCopyRxData;
  SomeIp_OnTpCopyTxDataFncType onTpCopyTxData;
} SomeIp_ClientMethodType;

typedef struct {
  uint16_t eventId;
  uint8_t interfaceVersion;
  SomeIp_OnNotificationFncType onNotification;
} SomeIp_ClientEventType;

/* @SWS_SomeIpXf_00152 */
typedef struct {
  uint16_t serviceId;
  uint16_t methodId;
  uint32_t length;
  uint16_t clientId;
  uint16_t sessionId;
  uint8_t interfaceVersion;
  uint8_t messageType;
  uint8_t returnCode;
  boolean isTpFlag;
} SomeIp_HeaderType;

typedef struct {
  uint32_t offset;
  boolean moreSegmentsFlag;
} SomeIp_TpHeaderType;

typedef struct {
  SomeIp_HeaderType header;
  SomeIp_TpHeaderType tpHeader;
  TcpIp_SockAddrType RemoteAddr;
  SomeIp_MessageType req;
} SomeIp_MsgType;

typedef struct SomeIp_AsyncReqMsg_s {
  STAILQ_ENTRY(SomeIp_AsyncReqMsg_s) entry;
  TcpIp_SockAddrType RemoteAddr;
  uint16_t clientId;
  uint16_t sessionId;
  uint16_t methodId;
} SomeIp_AsyncReqMsgType;

typedef struct SomeIp_RxTpMsg_s {
  STAILQ_ENTRY(SomeIp_RxTpMsg_s) entry;
  TcpIp_SockAddrType RemoteAddr;
  uint32_t offset;
  uint16_t methodId; /* this is the key */
  uint16_t clientId;
  uint16_t sessionId;
  uint16_t timer;
} SomeIp_RxTpMsgType;

typedef struct SomeIp_TxTpMsg_s {
  STAILQ_ENTRY(SomeIp_TxTpMsg_s) entry;
  TcpIp_SockAddrType RemoteAddr;
  uint32_t offset;
  uint32_t length;
  uint16_t methodId; /* this is the key */
  uint16_t clientId;
  uint16_t sessionId;
#ifndef DISABLE_SOMEIP_TX_NOK_RETRY
  uint8_t retryCounter;
#endif
} SomeIp_TxTpMsgType;

/* For TCP large messages */
typedef struct {
  uint8_t *data;
  uint32_t length;
  uint32_t offset;
  uint8_t header[16];
  uint8_t lenOfHd;
} SomeIp_TcpBufferType;

typedef STAILQ_HEAD(rxTpMsgHead, SomeIp_RxTpMsg_s) SomeIp_RxTpMsgList;
typedef STAILQ_HEAD(txTpMsgHead, SomeIp_TxTpMsg_s) SomeIp_TxTpMsgList;

typedef struct {
  STAILQ_HEAD(reqMsgHead, SomeIp_AsyncReqMsg_s) pendingAsyncReqMsgs;
  SomeIp_RxTpMsgList pendingRxTpMsgs;
  SomeIp_TxTpMsgList pendingTxTpMsgs;
  bool online;
} SomeIp_ServerServiceContextType;

typedef struct {
  SomeIp_RxTpMsgList pendingRxTpMsgs;
  SomeIp_TxTpMsgList pendingTxTpMsgs;
  uint16_t sessionId;
  bool online;
} SomeIp_ClientServiceContextType;

typedef struct {
  SomeIp_ServerServiceContextType *context;
  PduIdType TxPduId;
  SoAd_SoConIdType SoConId;
  SomeIp_TcpBufferType *tcpBuf;
} SomeIp_ServerConnectionType;

typedef struct {
  uint16_t serviceId;
  uint16_t clientId;
  const SomeIp_ServerMethodType *methods;
  uint16_t numOfMethods;
  const SomeIp_ServerEventType *events;
  uint16_t numOfEvents;
  const SomeIp_ServerConnectionType *connections;
  uint8_t numOfConnections;
} SomeIp_ServerServiceType;

typedef struct {
  uint16_t serviceId;
  uint16_t clientId;
  uint16_t sdHandleID;
  const SomeIp_ClientMethodType *methods;
  uint16_t numOfMethods;
  const SomeIp_ClientEventType *events;
  uint16_t numOfEvents;
  SomeIp_ClientServiceContextType *context;
  PduIdType TxPduId;
  SomeIp_OnAvailabilityFncType onAvailability;
  SomeIp_TcpBufferType *tcpBuf;
} SomeIp_ClientServiceType;

typedef struct {
  boolean isServer;
  SoAd_SoConIdType SoConId;
  const void *service;
} SomeIp_ServiceType;

struct SomeIp_Config_s {
  /* @ECUC_SomeIpTp_00023 */
  uint16_t TpRxTimeoutTime;
  const SomeIp_ServiceType *services;
  uint16_t numOfService;
  const uint16_t *PID2ServiceMap;
  const uint16_t *PID2ServiceConnectionMap;
  uint16_t numOfPIDs;
  const uint16_t *TxMethod2ServiceMap;
  const uint16_t *TxMethod2PerServiceMap;
  uint16_t numOfTxMethods;
  const uint16_t *TxEvent2ServiceMap;
  const uint16_t *TxEvent2PerServiceMap;
  uint16_t numOfTxEvents;
};
/* ================================ [ DECLARES  ] ============================================== */
/* ================================ [ DATAS     ] ============================================== */
/* ================================ [ LOCALS    ] ============================================== */
/* ================================ [ FUNCTIONS ] ============================================== */

#endif /* _SOMEIP_PRIV_H */
