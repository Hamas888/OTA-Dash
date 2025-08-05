 /*
  =====================================================================================================
  * File:    OTADash.h
  * Author:  Hamas Saeed
  * Version: Rev_1.0.0
  * Date:    December 12 2024
  * Brief:   This File Provide Wireless Intrective Features For IoT Devices Based On ESP32
  =====================================================================================================
  * Attention:
  *                         COPYRIGHT 2024 AMS-IOT Pvt Ltd.
  *
  * Licensed under ************* License Agreement V2, (the "License");
  * Third Party may not use this file except in compliance with the License.
  * Third Party may obtain a copy of the License at:
  *
  *
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  =====================================================================================================
  */  

#ifndef OTADASH_H
#define OTADASH_H

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <Update.h>
#include <DNSServer.h>
#include <EEPROM.h>
#include "WebPages.h"
#include "ArduinoJson.h"

enum class NetworkMode {
    ACCESS_POINT,
    STATION,
    DUAL
};

class OTADash {
public:
    OTADash(const char* ssid, const char* password, const char* custom_domain, const char* portal_title);
    ~OTADash();

    OTADash(const OTADash&) = delete;                                                                               // Prevent copy constructor and assignment
    OTADash& operator=(const OTADash&) = delete;
     
    OTADash(OTADash&&) noexcept = default;                                                                          // Allow move constructor and assignment
    OTADash& operator=(OTADash&&) noexcept = default;
    
    void stop();
    void printDebug(const String& message);    
    String encryptionTypeToString(int encryptionType);
    void begin(NetworkMode mode = NetworkMode::ACCESS_POINT);
    
    void onPaired(std::function<void(JsonDocument&)> callback);
    void onWifiSaved(std::function<void(const String&, const String&)> callback);
    
    void setEEPROMSize(size_t size)         { eepromSize            = size;    }
    void setEEPROMAddress(int address)      { eepromAddress         = address; }
    void setContentSize(int size)           { contentSize           = size;    }
    void setDebugLogMax(int logs)           { debugLogsMax          = logs;    }
    void setDebugPageStatus(bool status)    { isOnDebugPage         = status;  }
    void setReconnectAttempts(int attempts) { maxReconnectAttempts  = attempts;}
    void setReconnectDelay(uint32_t delay)  { reconnectDelay        = delay;   }
    void setPairRequest(bool request)       { pairRequest           = request; }
    void setPairResult(bool result)         { pairResult            = result;  }

    int getEEPROMAddress()    const         { return eepromAddress;            }
    int getContentSize()      const         { return eepromAddress;            }
    int getDebugLogsCounter() const         { return debugLogsCounter;         }
    int getDebugLogsMax()     const         { return debugLogsMax;             }
    bool isConnected()        const         { return isWifiConnected;         }
    size_t getEEPROMSize()    const         { return eepromSize;               }
    String getSSID()          const         { return WiFi.SSID();              }
    IPAddress getLocalIP()    const         { return WiFi.localIP();           }
    IPAddress getAPIP()       const         { return WiFi.softAPIP();          }

private:      
    int                                                 eepromAddress           = 0; 
    int                                                 contentSize             = 0; 
    int                                                 debugLogsCounter        = 0;
    int                                                 debugLogsMax            = 200;
    int                                                 maxReconnectAttempts    = 3;
    bool                                                isWifiConnected         = false;
    bool                                                serverStarted           = false;
    bool                                                isOnDebugPage           = false;
    bool                                                autoReconnect           = true;
    bool                                                pairRequest             = false;
    bool                                                pairResult              = false;
    size_t                                              eepromSize              = 0; 
    String                                              debugLogs;
    String                                              customDomain;
    uint32_t                                            reconnectDelay          = 5000;   
    const char*                                         ssid;
    const char*                                         password;
    const char*                                         portal_title;
    NetworkMode                                         currentMode             = NetworkMode::ACCESS_POINT;
    static OTADash*                                     instance;
    static const byte                                   DNS_PORT                = 53;
    std::unique_ptr<DNSServer>                          dnsServer;
    std::unique_ptr<AsyncWebServer>                     server;
    std::unique_ptr<AsyncWebSocket>                     ws;
    std::function<void(JsonDocument&)>                  pairingCallback;                                                  // User-defined callback
    std::function<void(const String&, const String&)>   wifiSavedCallback;                                                // User-defined callback

    void setupServer();
    void handleClient();   
    bool startStation();
    void reconnectWifi();
    bool startDualMode(); 
    void disconnectWifi();
    bool startAccessPoint();
    void handlePairingResult(); 
    void handleNetworkFailure();
    static void otaDashTask(void *parameter);
    void handleWifiScanResult(int scanResult);  
    static void handleUpdate(AsyncWebServerRequest *request);
    void handleWebSocketMessage(void *arg, uint8_t *data, size_t len);
    bool connectToWifi(const char* ssid, const char* password, uint32_t timeout_ms = 20000);
    static void handleUpload(AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final);
};

#endif // OTASERVER_H