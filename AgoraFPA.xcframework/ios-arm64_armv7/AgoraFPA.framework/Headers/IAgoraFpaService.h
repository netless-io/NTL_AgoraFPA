//
//  Agora FPA SDK
//
//  Copyright (c) 2021 Agora IO. All rights reserved.
//
#pragma once

#include "IAgoraLog.h"

#if defined(_WIN32)

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif  // !WIN32_LEAN_AND_MEAN

#include <Windows.h>

#if defined(AGORARTC_EXPORT)
#define AGORA_FPA_API extern "C" __declspec(dllexport)
#else
#define AGORA_FPA_API extern "C" __declspec(dllimport)
#endif  // AGORARTC_EXPORT

#define AGORA_FPA_CALL __cdecl

#elif defined(__APPLE__)

#include <TargetConditionals.h>

#define AGORA_FPA_API extern "C" __attribute__((visibility("default")))
#define AGORA_FPA_CALL

#elif defined(__ANDROID__) || defined(__linux__)

#define AGORA_FPA_API extern "C" __attribute__((visibility("default")))
#define AGORA_FPA_CALL

#else  // !_WIN32 && !__APPLE__ && !(__ANDROID__ || __linux__)

#define AGORA_FPA_API extern "C"
#define AGORA_FPA_CALL

#endif  // _WIN32

namespace agora {
namespace fpa {

/** Error code.
 */
enum FpaErrorType : uint16_t {
  /** 0: No error occurs.
   */
  FPA_ERROR_TYPE_OK = 0,
  /** 1: Resource temporarily unavailable.
   * This is a temporary condition and later calls to the same routine may complete normally.*
   */
  FPA_ERROR_TYPE_AGAIN = 11,
  /** 32: Broken pipe.
   * A write on a pipe, socket or FIFO for which there is no process to read the data.*
   */
  FPA_ERROR_TYPE_BROKEN_PIPE = 32,

  /** 200: No permission exists.
   * Check if the user has correct token to access fpa service.*
   */
  FPA_ERROR_TYPE_PERMISSION_DENIED = 200,

  /** 201: The connection failed since token is expired.
   * Please destroy the old fpa sevice and create a new one if see this error.*
   */
  FPA_ERROR_TYPE_TOKEN_EXPIRED = 201,

  /** 202: Service temporarily unavailable
   * This is a temporary condition and later calls to the same routine may complete normally.*
   */
  FPA_ERROR_TYPE_IN_PROGRESS = 202,

  /** 203: Socket not connect or socket already closed
   * Please close the old socket and create a new socket if see this error.*
   */
  FPA_ERROR_TYPE_NOT_CONNECTED = 203,

  /** 204: Service internal error
   * Please close the old socket and create a new socket if see this error.*
   */
  FPA_ERROR_TYPE_INTERNAL_ERROR = 204,

  /** 205: The length of sending buffer exceed limitation
   */
  FPA_ERROR_TYPE_MSG_TOO_BIG = 205,

  /** 206: The socket instance with specify chain id has alread been created
   * Do not connect twice.*
   */
  FPA_ERROR_TYPE_SOCKET_ALREADY_CONNECTED = 206,

  /** 207: The connection has been reset and service is reconneting
   * This is a temporary condition and later calls to the same routine may complete normally.*
   */
  FPA_ERROR_TYPE_RECONNECTING_CLIENT = 207,

  /** 208: The argument is invalid
   * Please check the input arguments if see this error.*
   */
  FPA_ERROR_TYPE_INVALID_ARGUMENT = 208,

  /** 209: Unknown error
   * Please close the old socket and create a new socket if see this error.*
   */
  FPA_ERROR_TYPE_UNKNOWN = 209,

  /** 210: AppID not set or is invalid
   * Please check the appid if see this error.*
   */
  FPA_ERROR_TYPE_INVALID_APP_ID = 210,

  /** 211: Service is not initialized or initialize failed
   */
  FPA_ERROR_TYPE_NOT_INITIALIZED = 211,

  /** 212: Socket can not send because send side is closed.
   * Read side is still available if see this error.
   */
  FPA_ERROR_TYPE_SEND_SIDE_CLOSED = 212,

  /** 213: Socket can not read because read side is closed.
   * Send side is still available if see this error.
   */
  FPA_ERROR_TYPE_READ_SIDE_CLOSED = 213,

  /** 214: Too frequent invocation
   * This error raise when user invoke API too frequently in a short interval
   */
  FPA_ERROR_TYPE_TOO_FREQUENTLY = 214,

  /** 214: Too many connections, exceed limitation
   */
  FPA_ERROR_TYPE_TOO_MANY_CONNECTIONS = 215,
};

enum ConnectionStateType {
  /**
   * 1: The SDK is disconnected from the server.
   */
  CONNECTION_STATE_TYPE_DISCONNECTED = 1,
  /**
   * 2: The SDK is connecting to the server.
   */
  CONNECTION_STATE_TYPE_CONNECTING = 2,
  /**
   * 3: The SDK is connected to the server and now you can use fpa service to
   * accelerate your data
   */
  CONNECTION_STATE_TYPE_CONNECTED = 3,
  /**
   * 4: The SDK keeps reconnecting to the server, probably because of
   * network issues.
   */
  CONNECTION_STATE_TYPE_RECONNECTING = 4,
  /**
   * 5: The SDK fails to connect to the server.
   */
  CONNECTION_STATE_TYPE_FAILED = 5,
};

enum ConnectionStateChangedReasonType {
  /**
   * 1: The SDK is connecting to the server.
   */
  CONNECTION_STATE_CHANGED_REASON_TYPE_CONNECTING = 1,
  /**
   * 2: The SDK is connected to the server.
   */
  CONNECTION_STATE_CHANGED_REASON_TYPE_CONNECTED = 2,
  /**
   * 3: The SDK fails to connect server due to invalid chain id or not authorized.
   */
  CONNECTION_STATE_CHANGED_REASON_TYPE_NOT_AUTHORIZED = 3,
  /**
   * 4: The connection is rejected by the server.
   */
  CONNECTION_STATE_CHANGED_REASON_TYPE_REJECTED_BY_SERVER = 4,
  /**
   * 5: The connection state changed due to network changed.
   */
  CONNECTION_STATE_CHANGED_REASON_TYPE_NETWORK_CHANGED = 5,
  /**
   * 6: The connection state changed due to no valid server address.
   */
  CONNECTION_STATE_CHANGED_REASON_TYPE_NO_ADDRESSES = 6,
  /**
   * 7: The connection state changed due to socket error.
   */
  CONNECTION_STATE_CHANGED_REASON_TYPE_SOCKET_ERROR = 7,
  /**
   * 8: The connection state changed due to internal error.
   */
  CONNECTION_STATE_CHANGED_REASON_TYPE_UNRECOVERABLE_ERROR = 8,
  /**
   * 9: The connection state changed due to expried ticket.
   */
  CONNECTION_STATE_CHANGED_REASON_TYPE_TICKET_EXPIRED = 9,
};

enum AccelerationStopReasonType {
  /**
   * 1: The acceleration service stopped normally.
   */
  ACCELERATION_STOP_REASON_TYPE_NORMAL_CLOSE = 1,
  /**
   * 2: The acceleration service stopped due to too frequent invocation.
   */
  ACCELERATION_STOP_REASON_TYPE_TOO_FREQUENTLY = 2,
  /**
   * 3: The acceleration service stopped due to too many connections.
   */
  ACCELERATION_STOP_REASON_TYPE_TOO_MANY_CONNECTIONS = 3,
  /**
   * 4: The acceleration service stopped due to connection already exist.
   */
  ACCELERATION_STOP_REASON_TYPE_ALREADY_CONNECTED = 4,
  /**
   * 5: The acceleration service stopped due to chain id not found.
   */
  ACCELERATION_STOP_REASON_TYPE_CHAIN_ID_NOT_FOUND = 5,
  /**
   * 6: The acceleration service stopped due to server reject request.
   */
  ACCELERATION_STOP_REASON_TYPE_REJECTED_BY_SERVER = 6,
  /**
   * 7: The acceleration service stopped due to socket error.
   */
  ACCELERATION_STOP_REASON_TYPE_SOCKET_ERROR = 7,
  /**
   * 8: The acceleration service stopped due to stream closed by remote server.
   */
  ACCELERATION_STOP_REASON_TYPE_STREAM_CLOSED = 8,
};

/**
 * Definition of fpa socket type.
*/
enum FpaSocketType : uint8_t {
  kSocketTcp = 0,
  kSocketUdp = 1,
};

enum FpaAreaCodeType {
  /**
   * Mainland China.
   */
  FPA_AREA_CODE_TYPE_CN = 0x00000001,
  /**
   * North America.
   */
  FPA_AREA_CODE_TYPE_NA = 0x00000002,
  /**
   * Europe.
   */
  FPA_AREA_CODE_TYPE_EU = 0x00000004,
  /**
   * Asia, excluding Mainland China.
   */
  FPA_AREA_CODE_TYPE_AS = 0x00000008,
  /**
   * Japan.
   */
  FPA_AREA_CODE_TYPE_JP = 0x00000010,
  /**
   * India.
   */
  FPA_AREA_CODE_TYPE_IN = 0x00000020,
  /**
   * (Default) Global.
   */
  FPA_AREA_CODE_TYPE_GLOB = 0xFFFFFFFF
};

typedef void (*general_callback_type)(void* user);
typedef void (*event_engine_callback_type)(int fd, short what, void* arg);
typedef void (*event_query_dns_callback_type)(int err, int size, const char** addresses, void* arg);

/**
 * Definition of fpa socket options.
*/
struct FpaSocketOptions {
  const char* ip = nullptr;
  const char* domain = nullptr;
  uint16_t port = 0;
};

class FpaSocketInterface {
 protected:
  virtual ~FpaSocketInterface() {}

 public:
  virtual int Connect(char* connection_id, uint32_t chain_id, const FpaSocketOptions& options) = 0;
  virtual int Close() = 0;
  virtual int CloseWrite() = 0;
  virtual int SendBuffer(const char* data, uint32_t length) = 0;
  virtual int ReadBuffer(char *data, uint32_t length) = 0;
  virtual int RegisterOnWriteCallback(general_callback_type callback = nullptr, void* user = nullptr) = 0;
  virtual int RegisterOnReadCallback(general_callback_type callback = nullptr, void* user = nullptr) = 0;
};

class IFpaServiceEventHandler {
 public:
  virtual ~IFpaServiceEventHandler() {}
  virtual void OnAccelerationStart(int chain_id, const char* connection_id) = 0;
  virtual void OnAccelerationStop(
      int chain_id, const char* connection_id,
      AccelerationStopReasonType reason) = 0;
  virtual void OnConnectionStateChanged(ConnectionStateType state,
      ConnectionStateChangedReasonType reason) = 0;
  virtual void OnTokenPrivilegeWillExpire(const char* token) = 0;
};

struct FpaServiceContext {
  const char* appId;
  const char* token;
  commons::LogConfig logConfig;
  IFpaServiceEventHandler *eventHandler;
  unsigned int areaCode;
  FpaServiceContext() : appId(nullptr),
    token(nullptr),
    eventHandler(nullptr),
    areaCode(FPA_AREA_CODE_TYPE_GLOB) {}
};

class IFpaService {
 protected:
  virtual ~IFpaService() {}

 public:
  virtual const char* GetInstallId() = 0;
  virtual const char* GetInstanceId() = 0;
  virtual ConnectionStateType GetConnectionState() = 0;
  virtual void* GetEventEngine() = 0;
  virtual int Initialize(const FpaServiceContext& ctx) = 0;
  virtual int Release() = 0;
  virtual int RenewToken(const char* token) = 0;
  virtual FpaSocketInterface *CreateSocket(FpaSocketType type) = 0;
  virtual int SetParameters(const char* parameters) = 0;

  virtual int OpenSocketPair(int fd[2], int socket_type) = 0;
  virtual void MakeSocketNonblocking(int fd) = 0;
  virtual void* EventNew(int fd, int type, event_engine_callback_type callback, void* arg) = 0;
  virtual void EventFree(void* ev) = 0;
  virtual void EventAdd(void* ev) = 0;
  virtual void* TimerEventNew(general_callback_type callback, uint64_t ms, bool persist = true, void* arg = nullptr) = 0;
  virtual int TimerEventFree(void* timer) = 0;
  virtual void SyncCall(general_callback_type callback, void* arg = nullptr) = 0;
  virtual void AsyncCall(general_callback_type callback, void* arg = nullptr) = 0;
  virtual void QueryDns(const char* fallback_property, event_query_dns_callback_type callback, void* arg) = 0;
  virtual void CloseSocketFd(int fd) = 0;

  virtual int SetLogFile(const char *filePath, unsigned int fileSize) = 0;
  virtual int SetLogFilter(unsigned int filter) = 0;
};

} /* fpa */
} /* agora */

AGORA_FPA_API agora::fpa::IFpaService* AGORA_FPA_CALL CreateAgoraFpaService();
