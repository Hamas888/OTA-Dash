   /*
 ====================================================================================================
 * File:        OTADash.cpp
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

#include "OTADash.h"
#include "WebPages.h"
#include "WebPagesStyles.h"

#if OTADASH_DEBUG_ENABLED
   ChronoLogger *otaDashLogger = nullptr;
#endif

OTADash* OTADash::instance = nullptr;

OTADash::OTADash(const char* ssid, const char* password, const char* custom_domain, const char* portal_title) : 
    ssid            (ssid), 
    password        (password), 
    pairingCallback (nullptr),
    wifiSavedCallback(nullptr),
    portal_title    (portal_title),
    dnsServer       (std::make_unique<DNSServer>()),
    customDomain    (String(custom_domain) + ".local"), 
    server          (std::make_unique<AsyncWebServer>(80)),
    ws              (std::make_unique<AsyncWebSocket>("/ws")) {
    instance = this;

    #if OTADASH_DEBUG_ENABLED
        otaDashLogger = new ChronoLogger("OTADash", OTADASH_DEBUG_LEVEL);
    #endif
    OTADASH_LOGGER(debug, "OTADash Instance Created");
}

OTADash::~OTADash() {
    stop();
    OTADASH_LOGGER(debug, "OTADash Instance Destroyed");
    #if OTADASH_DEBUG_ENABLED
        delete otaDashLogger;
        otaDashLogger = nullptr;
    #endif
}

void OTADash::stop() {
    if (serverStarted) {
        ws->closeAll();
        server->end();
        dnsServer->stop();
        if (mdnsStarted) {
            MDNS.end();
            mdnsStarted = false;
        }
        WiFi.softAPdisconnect(true);
        serverStarted = false;
        OTADASH_LOGGER(info, "Server stopped");
        
    }
}

bool OTADash::readEEPROM() {
    EEPROM.begin(eepromSize);
    EEPROM.get(eepromAddress, networkCredentials);

    if(strcmp(networkCredentials.setuped, "true") != 0) {
        EEPROM.end();
        return false;
    }

    EEPROM.end();
    return true;
}

bool OTADash::writeEEPROM() {
    EEPROM.begin(eepromSize);
    EEPROM.put(eepromAddress, networkCredentials);
    bool success = EEPROM.commit();
    EEPROM.end();
    return success;
}

void OTADash::handleClient() {
    if(serverStarted) {
        if (currentMode == NetworkMode::ACCESS_POINT || currentMode == NetworkMode::DUAL) {
            dnsServer->processNextRequest();
        }
        
        if ((currentMode == NetworkMode::STATION || currentMode == NetworkMode::DUAL) 
            && WiFi.status() != WL_CONNECTED && autoReconnect) {
            handleNetworkFailure();
        }

        if(pairRequest) {
            handlePairingResult();
        }
    }
    
    if(scanWiFi) {
        int scanComplete = WiFi.scanComplete();
        if(scanComplete >= 0) {
            handleWifiScanResult(scanComplete);
            visibleNetworksCount = scanComplete;
        }
    }
}

void OTADash::disconnectWifi() {
    if (currentMode == NetworkMode::STATION || currentMode == NetworkMode::DUAL) {
        WiFi.disconnect();
    }
}

void OTADash::begin(NetworkMode mode) {
    currentMode = mode;
    server->reset();
    
    OTADASH_LOGGER(info, "Starting server....");

    if(mode == NetworkMode::STATION || mode == NetworkMode::AUTO || mode == NetworkMode::DUAL) {
        if(!readEEPROM()) {
            OTADASH_LOGGER(warn, "WiFi credentials not found in EEPROM. Switching to AP mode.");
            currentMode = NetworkMode::ACCESS_POINT;                                                                    // Default to Access Point mode if no credentials are found
        } else if(mode == NetworkMode::AUTO) {
            OTADASH_LOGGER(info, "Auto mode detected. Switching to STATION mode");
            currentMode = NetworkMode::STATION;                                                                         // Switch to Station mode if AUTO is selected
        }
    }
  
    switch (currentMode) {
        case NetworkMode::ACCESS_POINT:
            OTADASH_LOGGER(info, "Access Point mode");            
            if (!startAccessPoint()) {
                OTADASH_LOGGER(error, "Failed to start AP mode");
                return;
            }
            break;
            
        case NetworkMode::STATION:
            OTADASH_LOGGER(info, "Station mode");
            if (!startStation()) {
                OTADASH_LOGGER(error, "Failed to start STA mode");
                return;
            }
            break;

        case NetworkMode::DUAL:
            OTADASH_LOGGER(info, "Dual AP/STA mode");
            if(!startDualMode()) {
                OTADASH_LOGGER(error, "Failed to start Dual mode");
                return;
            }
            break;
    }

    if (currentMode == NetworkMode::ACCESS_POINT || currentMode == NetworkMode::DUAL) {
        dnsServer->start(DNS_PORT, "*", WiFi.softAPIP());
    }

    ws->onEvent([this](
        AsyncWebSocket *server, 
        AsyncWebSocketClient *client,
        AwsEventType type, 
        void *arg, 
        uint8_t *data, 
        size_t len) {

        if (type == WS_EVT_DATA) {
            AwsFrameInfo *info = (AwsFrameInfo *)arg;
            if (info->opcode == WS_TEXT) {
                data[len] = 0;
                handleWebSocketMessage(arg, data, len);
            }
        } else if (type == WS_EVT_CONNECT) {
            OTADASH_LOGGER(info, "WebSocket client connected: ID=%u", client->id());
        } else if (type == WS_EVT_DISCONNECT) {
            OTADASH_LOGGER(info, "WebSocket client disconnected: ID=%u", client->id());
        }
    });

    server->addHandler(ws.get());
    setupServer();
    server->begin();
    OTADASH_LOGGER(info, "Server started");

    if (currentMode == NetworkMode::STATION || currentMode == NetworkMode::DUAL) {
        OTADASH_LOGGER(info, "Station IP: %s", WiFi.localIP().toString().c_str());
        OTADASH_LOGGER(info, "Access in the browser by: http://%s", WiFi.localIP().toString().c_str());
        OTADASH_LOGGER(info, "Waiting for mDNS initialization...");
        OTADASH_LOGGER(info, "Once ready, access via: http://%s", customDomain.c_str());
    }
    if (currentMode == NetworkMode::ACCESS_POINT || currentMode == NetworkMode::DUAL) {
        OTADASH_LOGGER(info, "Access point IP: %s", WiFi.softAPIP().toString().c_str());
        OTADASH_LOGGER(info, "Access in the browser by: http://%s", WiFi.softAPIP().toString().c_str());
        OTADASH_LOGGER(info, "Access in the browser by: http://%s", customDomain.c_str());
    }
    serverStarted = true;

    xTaskCreatePinnedToCore(otaDashTask, "otaDashTask", 4096, instance, 1, NULL, 0);
}

void OTADash::printDebug(const String& message) {
    if (serverStarted) {                                                                                           
        
        String formattedMessage = message;
        formattedMessage.replace("\n", "<br/>");
        formattedMessage.replace("\r", "");
        formattedMessage.replace("\t", "&emsp;");

        debugLogs += formattedMessage + "<br/>";
        debugLogsCounter++;
        
        if (isOnDebugPage) {                                                                                            // Send to WebSocket clients if debug page is open
            ws->textAll(formattedMessage);                                                                              // Send the formatted message to all WebSocket clients
        }

        if (debugLogsCounter >= debugLogsMax) {                                                                         // Clear buffer if it gets too large
            debugLogs = "";
            debugLogsCounter = 0;
        }
    }
}

String OTADash::encryptionTypeToString(int encryptionType) {                                                            // Helper function to convert encryption type to a string
    switch (encryptionType) {
        case WIFI_AUTH_OPEN: return "Open";
        case WIFI_AUTH_WEP: return "WEP";
        case WIFI_AUTH_WPA_PSK: return "WPA";
        case WIFI_AUTH_WPA2_PSK: return "WPA2";
        case WIFI_AUTH_WPA_WPA2_PSK: return "WPA/WPA2";
        case WIFI_AUTH_WPA3_PSK: return "WPA3";
        default: return "Unknown";
    }
}

bool OTADash::connectToWifi(const char* ssid, const char* password, uint32_t timeout_ms) {
    if (currentMode == NetworkMode::ACCESS_POINT) {
        OTADASH_LOGGER(info, "Cannot connect to WiFi in AP-only mode");
        
        return false;
    }

    WiFi.begin(ssid, password);
    
    uint32_t startTime = millis();
    OTADASH_LOGGER(info, "Connecting to WiFi %s", ssid);
    while (WiFi.status() != WL_CONNECTED) {
        if (millis() - startTime >= timeout_ms) {
            OTADASH_LOGGER(info, "WiFi connection timeout");
            return false;
        }
        vTaskDelay(500 / portTICK_PERIOD_MS);
        OTADASH_LOGGER(debug, "Trying to connect...");
    }
    
    OTADASH_LOGGER(info, "Connected to WiFi");
    return true;
}

void OTADash::setupServer() {
    server->onNotFound([](AsyncWebServerRequest *request) {
        if (request->method() == HTTP_OPTIONS) {
            AsyncWebServerResponse *response = request->beginResponse(204);
            response->addHeader("Access-Control-Allow-Origin", "*");
            response->addHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
            response->addHeader("Access-Control-Allow-Headers", "Content-Type");
            request->send(response);
            return;
        }
        request->send(404, "text/plain", "Not Found");
    });

    server->on("/", HTTP_GET, [this](AsyncWebServerRequest *request){
        isOnDebugPage = false;
        String html = index_html;
        html.replace("%PORTAL_HEADING%", portal_title);
        html.replace("%CUSTOM_DOMAIN%", customDomain);
        if(customPages.size() > 0) {
            String rawName = customPages[0].path;

            if (rawName.startsWith("/")) {
                rawName = rawName.substring(1);
            }

            if (rawName.length() > 0) {
                rawName.setCharAt(0, toupper(rawName.charAt(0)));
            }

            String displayName = rawName;

            html.replace(
                "%CUSTOM_CONTENT%",
                "<a href=\"" + customPages[0].path + "\" class=\"button\">" + displayName + "</a>"
            );

        } else {
            html.replace("%CUSTOM_CONTENT%", "");
        }
        request->send(200, "text/html", html.c_str());
    });

    server->on("/styles.css", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/css", otadash_css);
    });

    server->on("/info", HTTP_GET, [this](AsyncWebServerRequest *request){
        isOnDebugPage = false;
        String infoHtml = device_info_html;
        String deviceInfo;
        deviceInfo =  "<tr><td>Product Name</td><td>"               + productName                                       + "</td></tr>";
        deviceInfo += "<tr><td>Firmware Version</td><td>"           + firmwareVersion                                   + "</td></tr>";
        deviceInfo +=  "<tr><td>Chip Model</td><td>"                + String(ESP.getChipModel())                        + "</td></tr>";
        deviceInfo += "<tr><td>Chip Cores</td><td>"                 + String(ESP.getChipCores())                        + "</td></tr>";
        deviceInfo += "<tr><td>Chip Revision</td><td>"              + String(ESP.getChipRevision())                     + "</td></tr>";
        deviceInfo += "<tr><td>CPU Frequency</td><td>"              + String(ESP.getCpuFreqMHz())                       + " MHz</td></tr>";
        deviceInfo += "<tr><td>Chip Temperature</td><td>"           + String(temperatureRead())                         + " °C</td></tr>";

        deviceInfo += "<tr><td>Access Point SSID</td><td>"          + WiFi.softAPSSID()                                 + "</td></tr>";
        deviceInfo += "<tr><td>Access Point IP Address</td><td>"    + WiFi.softAPIP().toString()                        + "</td></tr>";
        deviceInfo += "<tr><td>Connected Clients</td><td>"          + String(WiFi.softAPgetStationNum())                + "</td></tr>";

        deviceInfo += "<tr><td>Flash Size</td><td>"                 + String(ESP.getFlashChipSize() / (1024 * 1024))    + " MB</td></tr>";
        deviceInfo += "<tr><td>Flash Speed</td><td>"                + String(ESP.getFlashChipSpeed() / 1000000)         + " MHz</td></tr>";
        deviceInfo += "<tr><td>Sketch Size</td><td>"                + String(ESP.getSketchSize())                       + " bytes</td></tr>";
        deviceInfo += "<tr><td>Free Sketch Space</td><td>"          + String(ESP.getFreeSketchSpace() / (1024 * 1024))  + " MB</td></tr>";
        deviceInfo += "<tr><td>Heap Size</td><td>"                  + String(ESP.getHeapSize() / (1024 * 1024))         + " MB</td></tr>";
        deviceInfo += "<tr><td>Free Heap</td><td>"                  + String(ESP.getFreeHeap() / (1024 * 1024))         + " MB</td></tr>";
        deviceInfo += "<tr><td>PSRAM Size</td><td>"                 + String(ESP.getPsramSize() / (1024 * 1024))        + " MB</td></tr>";
        deviceInfo += "<tr><td>Free PSRAM</td><td>"                 + String(ESP.getFreePsram() / (1024 * 1024))        + " MB</td></tr>";

        deviceInfo += "<tr><td>Uptime</td><td>"                     + String(millis() / 1000)                           + " seconds</td></tr>";

        infoHtml.replace("%DEVICE_INFO%", deviceInfo);
        request->send(200, "text/html", infoHtml.c_str());
    });

    server->on("/about", HTTP_GET, [this](AsyncWebServerRequest *request){
        isOnDebugPage = false;
        request->send(200, "text/html", about_html);
    });

    server->on("/wifimanage", HTTP_GET, [this](AsyncWebServerRequest *request) {
        String html = wifi_manage_html;
        request->send(200, "text/html", html.c_str());
        if(currentMode != NetworkMode::ACCESS_POINT && currentMode != NetworkMode::DUAL) {
            OTADASH_LOGGER(warn, "Wi-Fi scan Skipped (Not in AP or Dual mode)");
            vTaskDelay(100 / portTICK_PERIOD_MS);
            publishCachedScanResults();
        } else {
            OTADASH_LOGGER(info, "Wi-Fi scan requested");
            WiFi.scanNetworks(true);
            scanWiFi = true;
        }
    });

    server->on("/save-wifi", HTTP_POST, [this](AsyncWebServerRequest *request) {
        if (request->hasParam("ssid", true) && request->hasParam("password", true)) {
            String ssid = request->getParam("ssid", true)->value();
            String password = request->getParam("password", true)->value();

            if (ssid.isEmpty()) {
                request->send(400, "text/plain", "SSID cannot be empty");
                return;
            }
            if (password.length() < 8) {
                request->send(400, "text/plain", "Password must be at least 8 characters");
                return;
            }
            
            OTADASH_LOGGER(info, "WIFI Saved Request Received");

            if (wifiSavedCallback) {                                                                                    // Call the user-defined callback if set
                wifiSavedCallback(ssid, password);
                request->send(200, "text/plain", "WiFi credentials saved");
            } else {
                OTADASH_LOGGER(info, "SSID:     %s", ssid.c_str());
                OTADASH_LOGGER(info, "Password: %s", password.c_str());
                OTADASH_LOGGER(info, "Missing Saving Callback, Using Default Method");
                strcpy(networkCredentials.ssid, ssid.c_str());
                strcpy(networkCredentials.password, password.c_str());
                strcpy(networkCredentials.setuped, "true");
                writeEEPROM();
                request->send(200, "text/plain", "Missing Saving Callback");
                ESP.restart();
            }
            
        } else {
            request->send(400, "text/plain", "Missing SSID or password");
        }
    });

    server->on("/update", HTTP_GET, [this](AsyncWebServerRequest *request){
        String html = update_firmware_html;
        request->send(200, "text/html", html.c_str());
    });

    server->on("/update", HTTP_POST, [](AsyncWebServerRequest *request){
        handleUpdate(request);
    }, handleUpload);

    server->on("/erase", HTTP_GET, [this](AsyncWebServerRequest *request){
        String html = erase_settings_html;
        request->send(200, "text/html", html.c_str());
    });

    server->on("/erase", HTTP_POST, [this](AsyncWebServerRequest *request){
        strcpy(networkCredentials.ssid, "");
        strcpy(networkCredentials.password, "");
        strcpy(networkCredentials.setuped, "false");
        writeEEPROM();
        request->send(200, "text/html", "Settings erased.");
    });

    server->on("/debug", HTTP_GET, [this](AsyncWebServerRequest *request){
        String html = debug_html;
        isOnDebugPage = true;
        html.replace("%PORTAL_HEADING%", portal_title);
        request->send(200, "text/html", html.c_str());
        
        if (debugLogs.length() > 0) {
            vTaskDelay(100 / portTICK_PERIOD_MS);
            ws->textAll(debugLogs);
        }
    });

    server->on("/restart", HTTP_GET, [this](AsyncWebServerRequest *request){
        String html = restart_device_html;
        request->send(200, "text/html", html.c_str());
    });

    server->on("/restart", HTTP_POST, [this](AsyncWebServerRequest *request){
        request->send(200, "text/html", "Device is restarting...<br/>Please wait a moment.");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        ESP.restart();
    });

    server->on("/generate_204", HTTP_GET, [this](AsyncWebServerRequest *request){
        String html = index_html;
        html.replace("%PORTAL_HEADING%", portal_title);
        html.replace("%CUSTOM_DOMAIN%", customDomain);
        request->send(200, "text/html", html.c_str());
    });

    server->on("/fwlink", HTTP_GET, [this](AsyncWebServerRequest *request){
        String html = index_html;
        html.replace("%PORTAL_HEADING%", portal_title);
        html.replace("%CUSTOM_DOMAIN%", customDomain);
        request->send(200, "text/html", html.c_str());
    });

    server->on("/pair", HTTP_OPTIONS, [](AsyncWebServerRequest *request){
        AsyncWebServerResponse *response = request->beginResponse(204);
        response->addHeader("Access-Control-Allow-Origin", "*");
        response->addHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        response->addHeader("Access-Control-Allow-Headers", "Content-Type");
        response->addHeader("Access-Control-Allow-Credentials", "true");
        request->send(response);
    });

    server->on("/pair", HTTP_POST, [this](AsyncWebServerRequest *request) {
    }, nullptr, [this](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) { 
        OTADASH_LOGGER(info, "Pairing request received");      
        if (len == 0) {
            AsyncWebServerResponse *response = request->beginResponse(400, "application/json", "{\"status\":\"error\",\"message\":\"Empty request body\"}");
            response->addHeader("Access-Control-Allow-Origin", "*");
            request->send(response);
            return;
        }

        JsonDocument jsonDoc; 
        DeserializationError error = deserializeJson(jsonDoc, data, len);

        if (error) {
            AsyncWebServerResponse *response = request->beginResponse(400, "application/json", "{\"status\":\"error\",\"message\":\"Invalid JSON format\"}");
            response->addHeader("Access-Control-Allow-Origin", "*");
            request->send(response);
            return;
        }
       
        if (!jsonDoc["user_ids"].is<JsonArray>() || !jsonDoc["wifi_ssid"].is<String>() || 
            !jsonDoc["wifi_password"].is<String>() || !jsonDoc["master_pin"].is<String>()) {
            AsyncWebServerResponse *response = request->beginResponse(400, "application/json", "{\"status\":\"error\",\"message\":\"Missing or invalid keys\"}");
            response->addHeader("Access-Control-Allow-Origin", "*");
            request->send(response);
            return;
        }

        JsonArray user_ids      = jsonDoc["user_ids"];                                                                                                               // Extract the values from JSON
        String wifi_ssid        = jsonDoc["wifi_ssid"];
        String wifi_password    = jsonDoc["wifi_password"];
        String master_pin       = jsonDoc["master_pin"];

        if (user_ids.size() < 1 || wifi_ssid.isEmpty() || wifi_password.length() < 8 || master_pin.length() < 4) {
            AsyncWebServerResponse *response = request->beginResponse(400, "application/json", "{\"status\":\"error\",\"message\":\"Validation failed for one or more fields\"}");
            response->addHeader("Access-Control-Allow-Origin", "*");
            request->send(response);
            return;
        }
        
        OTADASH_LOGGER(info, "Received Pairing Data:");
        for (size_t i = 0; i < user_ids.size(); i++) {
            OTADASH_LOGGER(info, "User ID:       %s", (const char*)user_ids[i]);
        }
        OTADASH_LOGGER(info, "WiFi SSID:     %s", wifi_ssid.c_str());
        OTADASH_LOGGER(info, "WiFi Password: %s", wifi_password.c_str());
        OTADASH_LOGGER(info, "Master PIN:    %s", master_pin.c_str());
        AsyncWebServerResponse *response;

        if(pairingCallback) {
            pairingCallback(jsonDoc);
            response = request->beginResponse(202, "application/json", "{\"status\":\"success\",\"message\":\"Request Accepted: Listen On Websocket\"}");
        } else {
            OTADASH_LOGGER(info, "Missing Pairing Callback");
            response = request->beginResponse(500, "application/json", "{\"status\":\"error\",\"message\":\"Missing Pairing Functionality\"}");
        }
        response->addHeader("Access-Control-Allow-Origin", "*");
        request->send(response);
    });

    // Setup custom page routes
    setupCustomPageRoutes();
}

bool OTADash::startStation() {
    scanWiFi = true;
    WiFi.scanNetworks(true);                                                                                        // Start scanning
    while(visibleNetworksCount < 0) {
        handleClient();
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
    WiFi.mode(WIFI_STA);
    return connectToWifi(networkCredentials.ssid, networkCredentials.password);
}

void OTADash::reconnectWifi() {
    disconnectWifi();
    connectToWifi(ssid, password, 5000);
}

bool OTADash::startDualMode() {
    bool result;
    WiFi.mode(WIFI_AP_STA);
    result = WiFi.softAP(String(ssid + String("_AP")).c_str(), password);
    if (!connectToWifi(ssid, password)) {
        OTADASH_LOGGER(info, "Failed to connect in Dual mode");
        return false;
    }
    return true && result;
}

bool OTADash::startAccessPoint() {
    WiFi.mode(WIFI_AP);
    return WiFi.softAP(ssid, password);
}

void OTADash::handleUpdate(AsyncWebServerRequest *request) {
    bool hasError = Update.hasError();
    int statusCode = hasError ? 500 : 200;                                                // Return 500 on update error
    AsyncWebServerResponse *response = request->beginResponse(
        statusCode,
        "text/plain",
        hasError ? "FAIL" : "OK"
    );
    response->addHeader("Connection", "close");
    request->send(response);

    xTaskCreate([](void *param) {
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        ESP.restart();
        vTaskDelete(NULL);
    }, "ota_restart", 2048, NULL, 1, NULL);
}

void OTADash::handleUpload(AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final) {
    if (!index) {
        OTADASH_LOGGER(info, "Update Start: %s", filename.c_str());
        if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { 
            Update.printError(Serial);
        }
    }
    if (!Update.hasError()) {
        if (Update.write(data, len) != len) {
            Update.printError(Serial);
        }
    }
    if (final) {
        if (Update.end(true)) {
            OTADASH_LOGGER(info, "Update Success: %u B", index + len);
        } else {
            Update.printError(Serial);
        }
    }
}

void OTADash::handlePairingResult() {
    String response;
    if(pairResult) {
        OTADASH_LOGGER(info, "Pairing successful");
        response = "{\"status\":\"success\",\"message\":\"Pairing successful\"}";
    } else {
        OTADASH_LOGGER(info, "Pairing failed");
        response = "{\"status\":\"error\",\"message\":\"Pairing failed\"}";
    }
    pairRequest = pairResult = false;
    ws->textAll(response);
}

void OTADash::otaDashTask(void *parameter) {
    OTADash* dash = static_cast<OTADash*>(parameter);
    unsigned long previousMillis = 0;
    bool mdnsInitialized = false;
    
    while(dash->serverStarted) {
        dash->handleClient();
        vTaskDelay(10 / portTICK_PERIOD_MS);

        if (!mdnsInitialized && (dash->currentMode == NetworkMode::STATION || dash->currentMode == NetworkMode::DUAL)) {
            if (WiFi.status() == WL_CONNECTED && WiFi.localIP() != IPAddress(0, 0, 0, 0)) {
                String hostName = dash->getCustomDomain();
                if (hostName.endsWith(".local")) {
                    hostName.remove(hostName.length() - 6);
                }
                
                if (MDNS.begin(hostName.c_str())) {
                    dash->mdnsStarted = true;
                    MDNS.addService("http", "tcp", 80);
                    OTADASH_LOGGER(info, "mDNS started successfully!");
                    OTADASH_LOGGER(info, "Access via: http://%s", dash->getCustomDomain().c_str());
                    mdnsInitialized = true;
                } else {
                    OTADASH_LOGGER(info, "Failed to start mDNS - retrying...");
                }
            }
        }

        if(millis() - previousMillis >= 10000 && dash->currentMode == NetworkMode::STATION) {
            previousMillis = millis();
            dash->isWifiConnected = WiFi.status() == WL_CONNECTED;

            if (!dash->isWifiConnected) {
                dash->handleNetworkFailure();
                mdnsInitialized = false;
            }
        }
    }
    vTaskDelete(NULL);
}

void OTADash::handleWifiScanResult(int scanResult) {
    if (scanResult == WIFI_SCAN_FAILED) {
        OTADASH_LOGGER(info, "Wi-Fi scan failed");
        cachedScanResults = "";
        cachedScanCount = -1;
    } else {
        OTADASH_LOGGER(info, "Wi-Fi scan completed! Found %d networks", scanResult);
        String networks = "[";
        for (int i = 0; i < scanResult; i++) {
            if (i > 0) networks += ",";
            String ssidEscaped = WiFi.SSID(i);
            ssidEscaped.replace("\"", "\\\"");
            networks += "{\"ssid\":\"" + ssidEscaped + "\",";
            networks += "\"rssi\":" + String(WiFi.RSSI(i)) + ",";
            networks += "\"channel\":" + String(WiFi.channel(i)) + ",";
            networks += "\"encryption\":\"" + encryptionTypeToString(WiFi.encryptionType(i)) + "\"}";
        }
        networks += "]";
        cachedScanResults = networks;
        cachedScanCount = scanResult;
        ws->textAll(networks);
    }
    scanWiFi = false;
}

void OTADash::publishCachedScanResults() {
    if (!cachedScanResults.isEmpty()) {
        ws->textAll(cachedScanResults);
        OTADASH_LOGGER(info, "Serving cached Wi-Fi scan results (%d networks)", cachedScanCount >= 0 ? cachedScanCount : 0);
    } else {
        ws->textAll("[]");
        OTADASH_LOGGER(info, "No cached Wi-Fi scan results available");
    }
}

void OTADash::handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
    AwsFrameInfo *info = (AwsFrameInfo*)arg;
    if(info->final && info->index == 0 && info->len == len) {
        if(info->opcode == WS_TEXT) {
            String message;
            for (size_t i = 0; i < len; i++) {
                message += (char)data[i];
            }
            printDebug("Received message: " + message);
        }
    }
}

void OTADash::handleNetworkFailure() {
    static int reconnectCount = 0;
    static unsigned long lastReconnectAttempt = 0;
    
    if (millis() - lastReconnectAttempt >= reconnectDelay) {
        if (reconnectCount < maxReconnectAttempts) {
            OTADASH_LOGGER(info, "Attempting to reconnect to WiFi...");
            reconnectWifi();
            reconnectCount++;
            lastReconnectAttempt = millis();
        } else {
            OTADASH_LOGGER(info, "Max reconnection attempts reached");
            reconnectCount = 0;
        }
    }
}

void OTADash::onWifiSaved(std::function<void(const String&, const String&)> callback) {
    wifiSavedCallback = callback;
}

// Custom Page Implementation
void OTADash::addCustomPage(
    const String& path, const String& htmlContent, 
    std::function<String(const String&)> getCallback,
    std::function<String(const String&)> postCallback
) {
    customPages.emplace_back(path, htmlContent, getCallback, postCallback);
    if (serverStarted) {
        setupCustomPageRoutes();
    }
}

void OTADash::addCustomDataHandler(
    const String& path,
    std::function<String(const String&)> getCallback,
    std::function<String(const String&)> postCallback
) {
    auto it = std::find_if(customPages.begin(), customPages.end(), [&path](const CustomPage& page) { return page.path == path; });
    
    if (it != customPages.end()) {
        if (getCallback) it->getCallback = getCallback;
        if (postCallback) it->postCallback = postCallback;
    } else {
        customPages.emplace_back(path, "", getCallback, postCallback);
    }
    
    if (serverStarted) {
        setupCustomPageRoutes();
    }
}

void OTADash::setupCustomPageRoutes() {
    for (const auto& page : customPages) {
        if (!page.path.isEmpty()) {
            if (!page.htmlContent.isEmpty()) {
                server->on(page.path.c_str(), HTTP_GET, [this, page](AsyncWebServerRequest *request){
                    handleCustomPageGet(request, page);
                });
            }

            String dataPath = page.path + "/data";
            if (page.getCallback) {
                server->on(dataPath.c_str(), HTTP_GET, [this, page](AsyncWebServerRequest *request){
                    handleCustomDataGet(request, page);
                });
            }

            if (page.postCallback) {
                server->on(dataPath.c_str(), HTTP_POST, [this, page](AsyncWebServerRequest *request){
                }, nullptr, [this, page](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
                    if (len == 0) {
                        AsyncWebServerResponse *response = request->beginResponse(400, "application/json", "{\"error\":\"Empty request body\"}");
                        response->addHeader("Access-Control-Allow-Origin", "*");
                        request->send(response);
                        return;
                    }

                    String bodyString;
                    bodyString.reserve(len + 1);
                    for (size_t i = 0; i < len; i++) {
                        bodyString += (char)data[i];
                    }

                    String response = page.postCallback(bodyString);
                    
                    AsyncWebServerResponse *resp = request->beginResponse(200, "application/json", response);
                    resp->addHeader("Access-Control-Allow-Origin", "*");
                    resp->addHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
                    resp->addHeader("Access-Control-Allow-Headers", "Content-Type");
                    request->send(resp);
                });
                
                server->on(dataPath.c_str(), HTTP_OPTIONS, [](AsyncWebServerRequest *request){
                    AsyncWebServerResponse *response = request->beginResponse(204);
                    response->addHeader("Access-Control-Allow-Origin", "*");
                    response->addHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
                    response->addHeader("Access-Control-Allow-Headers", "Content-Type");
                    request->send(response);
                });
            }
        }
    }
}

String OTADash::queryParamsToJson(AsyncWebServerRequest *request) {
    JsonDocument doc;
    
    for (size_t i = 0; i < request->params(); i++) {
        const AsyncWebParameter* p = request->getParam(i);
        if (p->isPost() == false) {
            doc[p->name()] = p->value();
        }
    }
    
    String jsonString;
    serializeJson(doc, jsonString);
    return jsonString;
}

void OTADash::handleCustomPageGet(AsyncWebServerRequest *request, const CustomPage& page) {
    request->send(200, "text/html", page.htmlContent.c_str());
}

void OTADash::handleCustomDataGet(AsyncWebServerRequest *request, const CustomPage& page) {
    if (page.getCallback) {
        String queryJson = queryParamsToJson(request);
        String response = page.getCallback(queryJson);
        
        AsyncWebServerResponse *resp = request->beginResponse(200, "application/json", response);
        resp->addHeader("Access-Control-Allow-Origin", "*");
        resp->addHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        resp->addHeader("Access-Control-Allow-Headers", "Content-Type");
        request->send(resp);
    } else {
        request->send(404, "application/json", "{\"error\":\"GET handler not configured\"}");
    }
}

void OTADash::onPaired(std::function<void(JsonDocument&)> callback) {
    pairingCallback = callback;
}