//
//  Agora FPA SDK
//
//  Copyright (c) 2021 Agora.io. All rights reserved.
//
#pragma once

#include <cstdlib>
#include <cstring>

#if defined(_WIN32)

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif  // !WIN32_LEAN_AND_MEAN

#include <Windows.h>

#if defined(AGORARTC_EXPORT)
#define AGORA_FPA_PROXY_API extern "C" __declspec(dllexport)
#else
#define AGORA_FPA_PROXY_API extern "C" __declspec(dllimport)
#endif  // AGORARTC_EXPORT

#define AGORA_FPA_PROXY_CALL __cdecl

#elif defined(__APPLE__)

#include <TargetConditionals.h>

#define AGORA_FPA_PROXY_API extern "C" __attribute__((visibility("default")))
#define AGORA_FPA_PROXY_CALL

#elif defined(__ANDROID__) || defined(__linux__)

#define AGORA_FPA_PROXY_API extern "C" __attribute__((visibility("default")))
#define AGORA_FPA_PROXY_CALL

#else  // !_WIN32 && !__APPLE__ && !(__ANDROID__ || __linux__)

#define AGORA_FPA_PROXY_API extern "C"
#define AGORA_FPA_PROXY_CALL

#endif  // _WIN32

namespace agora {
namespace fpa {
namespace service {

#define MAX_STR_LEN 512

enum FPA_ERROR_CODE {
  /**
   * Everything is OK, No error happen
   */
  FPA_ERR_NONE = 0,

  /**
   * Bad parameters when call function
   */
  FPA_ERR_INVALID_ARGUMENT = -1,

  /**
   * No memory to allocate object
   */
  FPA_ERR_NO_MEMORY = -2,

  /**
   * Not init
   */
  FPA_ERR_NOT_INITIALIZED = -3,

  /**
   * Initialize failed
   */
  FPA_ERR_CORE_INITIALIZE_FAILED = -4,

  /**
   * Unable to bind a socket port
   */
  FPA_ERR_UNABLE_BIND_SOCKET_PORT = -5,
};

/**
 * fpa fallback error reason code
 */
enum FPA_FAILED_REASON_CODE {
  /**
   * Query dns failed(convert request url to ip failed)
   */
  FPA_FAILED_REASON_DNS_QUERY = -101,

  /**
   * Create socket failed
   */
  FPA_FAILED_REASON_SOCKET_CREATE = -102,

  /**
   * Connect socket failed
   */
  FPA_FAILED_REASON_SOCKET_CONNECT = -103,

  /**
   * Connect remote server timeout(most use at NOT fallback)
   */
  FPA_FAILED_REASON_CONNECT_TIMEOUT = -104,

  /**
   * Not match a chain id(most use at http)
   */
  FPA_FAILED_REASON_NO_CHAIN_ID_MATCH = -105,

  /**
   * Failed to read data
   */
  FPA_FAILED_REASON_DATA_READ = -106,

  /**
   * Failed to write data
   */
  FPA_FAILED_REASON_DATA_WRITE = -107,

  /**
   * Call too frequently
   */
  FPA_FAILED_REASON_TOO_FREQUENTLY = -108,

  /**
   * Service core connect too many connections
   */
  FPA_FAILED_REASON_TOO_MANY_CONNECTIONS = -109,
};

struct FpaChainInfo {
  FpaChainInfo() : chain_id(0), enable_fallback(true), port(0),
  enable_crypto(false), user_cas(nullptr), user_cas_size(0),
  verify_host(nullptr) {
    memset(address, 0, sizeof(address));
  }

  /**
   * ip or domain
   */
  char address[MAX_STR_LEN];

  /**
   * port
   */
  int port;

  /**
   * fpa chain id
   */
  int chain_id;

  /**
   * Whether to fall back to the original link, if not, the link fails
   */
  bool enable_fallback;

  /**
   * Whether to enable crypto transport during fall back
   */
  bool enable_crypto;

  /**
   * Custom root CA when verify remote cert chain.
   * Should be in PEM file format with header and footer.
   */
  const char** user_cas;

  /**
   * Size of |user_cas| array.
   */

  int user_cas_size;

  /**
   * Host that will be verified when verify remote cert chain.
   * Ignore host verification if NULL.
   */
  const char* verify_host;
};

struct FpaHttpProxyChainConfig {
  /**
   * FpaChainInfo array
   */
  FpaChainInfo* chain_array;

  /**
   * FpaChainInfo array size
   */
  int chain_array_size;

  /**
   * When the http proxy cannot find the corresponding chain configuration, whether to fall back to
   * the original link, if not, the link fails
   */
  bool fallback_when_no_chain_available;

  FpaHttpProxyChainConfig()
      : chain_array(NULL), chain_array_size(0), fallback_when_no_chain_available(true) {}
};

struct FpaProxyServiceDiagnosisInfo {
  /**
   * Install id
   */
  char install_id[MAX_STR_LEN];

  /**
   * Instance id
   */
  char instance_id[MAX_STR_LEN];

  FpaProxyServiceDiagnosisInfo() {
    memset(install_id, 0, sizeof(install_id));
    memset(instance_id, 0, sizeof(instance_id));
  }
};

struct FpaProxyServiceConfig {
  char app_id[MAX_STR_LEN];
  char token[MAX_STR_LEN];
  int log_level;
  int log_file_size_kb;
  char log_file_path[MAX_STR_LEN];

  FpaProxyServiceConfig() : log_level(0), log_file_size_kb(0) {
    memset(app_id, 0, sizeof(app_id));
    memset(token, 0, sizeof(token));
    memset(log_file_path, 0, sizeof(log_file_path));
  }
};

struct FpaProxyConnectionInfo {
  char dst_ip_or_domain[MAX_STR_LEN];
  char connection_id[MAX_STR_LEN];
  char proxy_type[MAX_STR_LEN];  // http/https/transport
  int dst_port;
  int local_port;  // local socket bind port

  FpaProxyConnectionInfo() : dst_port(0), local_port(0) {
    memset(dst_ip_or_domain, 0, sizeof(dst_ip_or_domain));
    memset(connection_id, 0, sizeof(connection_id));
    memset(proxy_type, 0, sizeof(proxy_type));
  }
};

class IAgoraFpaProxyServiceObserver {
 public:
  /**
   * Success of once FPA call(NOT include fallback)
   * @param info Information of FpaProxyConnectionInfo
   */
  virtual void onAccelerationSuccess(const FpaProxyConnectionInfo& info) = 0;

  /**
   * Connect to fpa success
   * @param info Information of FpaProxyConnectionInfo
   */
  virtual void onConnected(const FpaProxyConnectionInfo& info) = 0;

  /**
   * Error happen and fallback when connect(MEAN: will try fallback)
   * @param info
   * @param reason
   */
  virtual void onDisconnectedAndFallback(const FpaProxyConnectionInfo& info,
                                         FPA_FAILED_REASON_CODE reason) = 0;

  /**
   * Error happen and not fallback when connect(MEAN: not fallback, end of this request)
   * @param info Information of FpaProxyConnectionInfo
   * @param reason Reason code of this failed
   */
  virtual void onConnectionFailed(const FpaProxyConnectionInfo& info,
                                  FPA_FAILED_REASON_CODE reason) = 0;

  virtual ~IAgoraFpaProxyServiceObserver() = default;
};

class IAgoraFpaProxyService {
 public:
  /**
   * @brief Start fpa proxy service
   *
   * @param config See FpaProxyServiceConfig
   * @return int 0: Success. < 0: Failure.
   */
  virtual int Start(const FpaProxyServiceConfig& config) = 0;

  /**
   * @brief Stop fpa proxy service
   *
   * @return int 0: Success. < 0: Failure.
   */
  virtual int Stop() = 0;

  /**
   * @brief Set IAgoraFpaProxyServiceObserver Observer
   *
   * @param observer
   * @return int
   */
  virtual int SetObserver(IAgoraFpaProxyServiceObserver* observer) = 0;

  /**
   * @brief Get the local http proxy port, Used for http proxy usage scenarios
   *
   * @param port [OUT] The local proxy port.
   * @return int 0: Success. < 0: Failure.
   */
  virtual int GetHttpProxyPort(unsigned short& port) = 0;

  /**
   * @brief Obtain the local transparent proxy port according to the chain information, Used in
   * transparent proxy scenarios
   * @param proxy_port the result of transparent proxy port
   * @param info the basic info of fpa
   * @return
   */
  virtual int GetTransparentProxyPort(unsigned short& proxy_port, const FpaChainInfo& info) = 0;

  /**
   * @brief Set the Parameters
   *
   * @param param
   * @return int 0: Success. < 0: Failure.
   */
  virtual int SetParameters(const char* param) = 0;

  /**
   * @brief Set or update the chain configuration of the http proxy
   *
   * @param config See HttpProxyChainConfig
   * @return int 0: Success. < 0: Failure.
   */
  virtual int SetOrUpdateHttpProxyChainConfig(const FpaHttpProxyChainConfig& config) = 0;

  /**
   * @brief Get information about diagnosis
   *
   * @param info Diagnosis information structure
   * @return int 0: Success. < 0: Failure.
   */
  virtual int GetDiagnosisInfo(FpaProxyServiceDiagnosisInfo& info) = 0;
};

}  // namespace service
}  // namespace fpa
}  // namespace agora

/**
 * @brief Get Agora Fpa Proxy Service object, The object is globally unique
 *
 * @return IAgoraFpaProxyService*
 */
AGORA_FPA_PROXY_API agora::fpa::service::IAgoraFpaProxyService* AGORA_FPA_PROXY_CALL GetAgoraFpaProxyService();

/**
 * @brief Get the Agora Fpa Proxy Service Sdk Version
 *
 * @return const char*
 */
AGORA_FPA_PROXY_API const char* AGORA_FPA_PROXY_CALL GetAgoraFpaProxyServiceSdkVersion();

/**
 * @brief Get the Agora Fpa Proxy Service Sdk build information
 *
 * @return const char*
 */
AGORA_FPA_PROXY_API const char* AGORA_FPA_PROXY_CALL GetAgoraFpaProxyServiceSdkBuildInfo();
