   /*
 ====================================================================================================
 * File:        OTADash.h
 * Author:      Hamas Saeed
 * Version:     Rev_1.0.0
 * Date:        Feb 10 2025
 * Brief:       This Package Provide Wireless Intrective Features For IoT Devices Based On ESP32
 * 
 ====================================================================================================
 * License: 
 * MIT License
 * 
 * Copyright (c) 2025 Hamas Saeed
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 * For any inquiries, contact Hamas Saeed at hamasaeed@gmail.com
 *
 ====================================================================================================
 */

#ifndef OTADASH_H
#define OTADASH_H

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <Update.h>
#include <DNSServer.h>
#include <EEPROM.h>
#include <ESPmDNS.h>
#include "WebPages.h"
#include "styles.h"
#include "ArduinoJson.h"

enum NetworkMode {
    ACCESS_POINT,
    STATION,
    AUTO,
    DUAL
};

struct NetworkCredentials {
    char ssid[20];
    char password[20];
    char setuped[10];
};

class OTADash {
public:
    OTADash(const char* ssid, const char* password, const char* custom_domain, const char* portal_title);
    ~OTADash();

    OTADash(const OTADash&) = delete;                                                                               // Prevent copy constructor and assignment
    OTADash& operator=(const OTADash&) = delete;
     
    OTADash(OTADash&&) noexcept = default;                                                                          // Allow move constructor and assignment
    OTADash& operator=(OTADash&&) noexcept = default;
    
    void printDebug(const String& message);   
    void begin(NetworkMode mode = NetworkMode::AUTO); 
    
    void onPaired(std::function<void(JsonDocument&)> callback);
    void onWifiSaved(std::function<void(const String&, const String&)> callback);
    
    void setDebugLogMax(int logs)           { debugLogsMax          = logs;    }
    void setEEPROMSize(size_t size)         { eepromSize            = size;    }
    void setPairResult(bool result)         { pairResult            = result;  }
    void setProductName(String name)        { productName           = name;    }
    void setPairRequest(bool request)       { pairRequest           = request; }
    void setEEPROMAddress(int address)      { eepromAddress         = address; }
    void setReconnectDelay(uint32_t delay)  { reconnectDelay        = delay;   }
    void setReconnectAttempts(int attempts) { maxReconnectAttempts  = attempts;}
    void setFirmwareVersion(String version) { firmwareVersion       = version; }

    int getEEPROMAddress()    const         { return eepromAddress;            }
    int getDebugLogsCounter() const         { return debugLogsCounter;         }
    int getDebugLogsMax()     const         { return debugLogsMax;             }
    bool isConnected()        const         { return isWifiConnected;          }
    size_t getEEPROMSize()    const         { return eepromSize;               }
    String getSSID()          const         { return WiFi.SSID();              }
    IPAddress getLocalIP()    const         { return WiFi.localIP();           }
    IPAddress getAPIP()       const         { return WiFi.softAPIP();          }
    String getCustomDomain()  const         { return customDomain;             }

    NetworkCredentials getNetworkCredentials() const {
        return networkCredentials;
    }

private:      
    int                                                 eepromAddress           = 0; 
    int                                                 debugLogsCounter        = 0;
    int                                                 debugLogsMax            = 200;
    int                                                 maxReconnectAttempts    = 3;
    bool                                                isWifiConnected         = false;
    bool                                                serverStarted           = false;
    bool                                                isOnDebugPage           = false;
    bool                                                autoReconnect           = true;
    bool                                                pairRequest             = false;
    bool                                                pairResult              = false;
    bool                                                mdnsStarted             = false;
    size_t                                              eepromSize              = 50; 
    String                                              debugLogs;
    String                                              customDomain;
    String                                              firmwareVersion         = "Not Configured";
    String                                              productName             = "ESP32 Device";
    uint32_t                                            reconnectDelay          = 5000;   
    const char*                                         ssid;
    const char*                                         password;
    const char*                                         portal_title;
    NetworkMode                                         currentMode             = NetworkMode::AUTO;
    static OTADash*                                     instance;
    static const byte                                   DNS_PORT                = 53;
    NetworkCredentials                                  networkCredentials;
    std::unique_ptr<DNSServer>                          dnsServer;
    std::unique_ptr<AsyncWebServer>                     server;
    std::unique_ptr<AsyncWebSocket>                     ws;
    std::function<void(JsonDocument&)>                  pairingCallback;                                                  // User-defined callback
    std::function<void(const String&, const String&)>   wifiSavedCallback;                                                // User-defined callback

    void stop();
    bool readEEPROM();
    bool writeEEPROM();
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
    String encryptionTypeToString(int encryptionType); 
    static void handleUpdate(AsyncWebServerRequest *request);
    void handleWebSocketMessage(void *arg, uint8_t *data, size_t len);
    bool connectToWifi(const char* ssid, const char* password, uint32_t timeout_ms = 20000);
    static void handleUpload(AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final);
};

#endif // OTASERVER_H