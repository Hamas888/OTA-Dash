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
}

OTADash::~OTADash() {
    try {
        stop();
    } catch (const std::exception& e) {                                                                             // Handle any exceptions that occur during server shutdown
        Serial.print("[OTADash]: Error during server shutdown: ");
        Serial.println(e.what());
    }
}

void OTADash::stop() {
    if (serverStarted) {
        ws->closeAll();
        server->end();
        dnsServer->stop();
        WiFi.softAPdisconnect(true);
        serverStarted = false;
        Serial.println("Server stopped");
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
    if (currentMode == NetworkMode::ACCESS_POINT || currentMode == NetworkMode::DUAL) {
        dnsServer->processNextRequest();
    }
    
    if ((currentMode == NetworkMode::STATION || currentMode == NetworkMode::DUAL) 
        && WiFi.status() != WL_CONNECTED && autoReconnect) {
        handleNetworkFailure();
    }
    
    int scanResult = WiFi.scanComplete();
    if (scanResult >= 0) {
        handleWifiScanResult(scanResult);
    }

    if(pairRequest) {
        handlePairingResult();
    }
}

void OTADash::begin(NetworkMode mode) {
    currentMode = mode;
    server->reset();
    Serial.println("[OTADash]: Starting server....");

    if(mode == NetworkMode::STATION || mode == NetworkMode::AUTO || mode == NetworkMode::DUAL) {
        if(!readEEPROM()) {
            Serial.println("[OTADash]: Wifi credentials not found. Mode set to ACCESS_POINT");
            currentMode = NetworkMode::ACCESS_POINT;                                                                // Default to Access Point mode if no credentials are found
        } else if(mode == NetworkMode::AUTO) {
            Serial.println("[OTADash]: Auto mode detected. Switching to STATION mode");
            currentMode = NetworkMode::STATION;                                                                     // Switch to Station mode if AUTO is selected
        }
    }
  
    switch (currentMode) {
        case NetworkMode::ACCESS_POINT:
            Serial.println("[OTADash]: Access Point mode");
            if (!startAccessPoint()) {
                Serial.println("[OTADash]: Failed to start AP mode");
                return;
            }
            break;
            
        case NetworkMode::STATION:
            Serial.println("[OTADash]: Station mode");
            if (!startStation()) {
                Serial.println("[OTADash]: Failed to start STA mode");
                return;
            }
            break;

        case NetworkMode::DUAL:
            Serial.println("[OTADash]: Dual AP/STA mode");
            if(!startDualMode()) {
                Serial.println("[OTADash]: Failed to start Dual mode");
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
        try {
            if (type == WS_EVT_DATA) {
                AwsFrameInfo *info = (AwsFrameInfo *)arg;
                if (info->opcode == WS_TEXT) {
                    data[len] = 0;
                    handleWebSocketMessage(arg, data, len);
                }
            }
        } catch (const std::exception &e) {
            Serial.print("[OTADash]: WebSocket error: ");
            Serial.println(e.what());
        }
    });

    server->addHandler(ws.get());
    setupServer();
    server->begin();
    Serial.println("[OTADash]: Server started");

    if (currentMode == NetworkMode::STATION || currentMode == NetworkMode::DUAL) {
        Serial.print("[OTADash]: Station IP: ");
        Serial.println(WiFi.localIP());
        Serial.println("[OTADash]: Access in the browser by: http://" + WiFi.localIP().toString());
    }
    if (currentMode == NetworkMode::ACCESS_POINT || currentMode == NetworkMode::DUAL) {
        Serial.print("[OTADash]: Access point IP: ");
        Serial.println(WiFi.softAPIP());
        Serial.println("[OTADash]: Access in the browser by: http://" + WiFi.softAPIP().toString());
        Serial.println("[OTADash]: Access in the browser by: http://" + customDomain);
    }
    serverStarted = true;

    xTaskCreatePinnedToCore(otaDashTask, "otaDashTask", 4096, instance, 1, NULL, 0);
}

void OTADash::printDebug(const String& message) {
    if (serverStarted && isOnDebugPage) {                                                                           // Replace escape sequences with HTML equivalents
        
        String formattedMessage = message;
        formattedMessage.replace("\n", "<br/>");
        formattedMessage.replace("\r", "");
        formattedMessage.replace("\t", "&emsp;");

        debugLogs += formattedMessage + "<br/>";
        debugLogsCounter++;
        
        try {
            ws->textAll(formattedMessage);                                                                          // Send the formatted message to all WebSocket clients
        } catch (const std::exception& e) {
            Serial.print("[OTADash]: Debug print error: ");
            Serial.println(e.what());
        }

        if (debugLogsCounter >= debugLogsMax) {
            debugLogs = "";
            debugLogsCounter = 0;
        }
    }
}

String OTADash::encryptionTypeToString(int encryptionType) {                                                                                              // Helper function to convert encryption type to a string
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

void OTADash::disconnectWifi() {
    if (currentMode == NetworkMode::STATION || currentMode == NetworkMode::DUAL) {
        WiFi.disconnect();
    }
}

bool OTADash::connectToWifi(const char* ssid, const char* password, uint32_t timeout_ms) {
    if (currentMode == NetworkMode::ACCESS_POINT) {
        Serial.println("[OTADash]: Cannot connect to WiFi in AP-only mode");
        return false;
    }

    WiFi.begin(ssid, password);
    
    uint32_t startTime = millis();
    Serial.print("[OTADash]: Connecting to WiFi ");
    while (WiFi.status() != WL_CONNECTED) {
        if (millis() - startTime >= timeout_ms) {
            Serial.println("[OTADash]: WiFi connection timeout");
            return false;
        }
        delay(500);
        Serial.print(".");
    }
    
    Serial.println("\n[OTADash]: Connected to WiFi");
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
        request->send(200, "text/html", html.c_str());
    });

    server->on("/info", HTTP_GET, [this](AsyncWebServerRequest *request){
        String infoHtml = device_info_html;
        String deviceInfo;
        deviceInfo =  "<tr><td>Chip Model</td><td>"                 + String(ESP.getChipModel())                        + "</td></tr>";
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

    server->on("/wifimanage", HTTP_GET, [this](AsyncWebServerRequest *request) {
        Serial.println("[OTADash]: Wi-Fi scan requested");
        WiFi.scanNetworks(true);                                                                                    // Start scanning
        String html = wifi_manage_html;
        request->send(200, "text/html", html.c_str());
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
            
            Serial.println("[OTADash]: WIFI Saved Request Received");

            if (wifiSavedCallback) {                                                                                // Call the user-defined callback if set
                wifiSavedCallback(ssid, password);
                request->send(200, "text/plain", "WiFi credentials saved");
            } else {
                Serial.println("[OTADash]: SSID:     " + ssid);
                Serial.println("[OTADash]: Password: " + password);
                Serial.println("[OTADash]: Missing Saving Callback, Using Default Method");
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
    });

    server->on("/restart", HTTP_GET, [this](AsyncWebServerRequest *request){
        String html = restart_device_html;
        request->send(200, "text/html", html.c_str());
    });

    server->on("/restart", HTTP_POST, [this](AsyncWebServerRequest *request){
        request->send(200, "text/html", "Device is restarting...<br/>Please wait a moment.");
        vTaskDelay(1000);
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
        Serial.println("[OTADash]: Pairing request received");      
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
        
        Serial.println("[OTADash]: Received Pairing Data:");
        for (size_t i = 0; i < user_ids.size(); i++) {
            Serial.println("[OTADash]: User ID:       " + String((const char*)user_ids[i]));
        }
        Serial.println("[OTADash]: WiFi SSID:     " + wifi_ssid);
        Serial.println("[OTADash]: WiFi Password: " + wifi_password);
        Serial.println("[OTADash]: Master PIN:    " + master_pin);

        AsyncWebServerResponse *response;

        if(pairingCallback) {
            pairingCallback(jsonDoc);
            response = request->beginResponse(202, "application/json", "{\"status\":\"success\",\"message\":\"Request Accepted: Listen On Websocket\"}");
        } else {
            Serial.println("[OTADash]: Missing Pairing Callback");
            response = request->beginResponse(500, "application/json", "{\"status\":\"error\",\"message\":\"Missing Pairing Functionality\"}");
        }
        response->addHeader("Access-Control-Allow-Origin", "*");
        request->send(response);
    });
}

bool OTADash::startStation() {
    WiFi.mode(WIFI_STA);
    return connectToWifi(networkCredentials.ssid, networkCredentials.password);
}

bool OTADash::startDualMode() {
    bool result;
    WiFi.mode(WIFI_AP_STA);
    result = WiFi.softAP(String(ssid + String("_AP")).c_str(), password);
    if (!connectToWifi(ssid, password)) {
        Serial.println("[OTADash]: Failed to connect in Dual mode");
        return false;
    }
    return true && result;
}

bool OTADash::startAccessPoint() {
    WiFi.mode(WIFI_AP);
    return WiFi.softAP(ssid, password);
}

void OTADash::reconnectWifi() {
    disconnectWifi();
    connectToWifi(ssid, password, 5000);
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
        vTaskDelay(pdMS_TO_TICKS(2000));
        ESP.restart();
        vTaskDelete(NULL);
    }, "ota_restart", 2048, NULL, 1, NULL);
}

void OTADash::handleUpload(AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final) {
    try {
        if (!index) {
            Serial.printf("[OTADash]: Update Start: %s\n", filename.c_str());
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
                Serial.printf("[OTADash]: Update Success: %u B\n", index + len);
            } else {
                Update.printError(Serial);
            }
        }
    } catch (const std::exception& e) {
        Serial.print("[OTADash]: Update error: ");
        Serial.println(e.what());
    }
}

void OTADash::handlePairingResult() {
    String response;
    if(pairResult) {
        Serial.println("[OTADash]: Pairing successful");
        response = "{\"status\":\"success\",\"message\":\"Pairing successful\"}";
    } else {
        Serial.println("[OTADash]: Pairing failed");
        response = "{\"status\":\"error\",\"message\":\"Pairing failed\"}";
    }
    pairRequest = pairResult = false;
    ws->textAll(response);
}

void OTADash::otaDashTask(void *parameter) {
    OTADash* dash = static_cast<OTADash*>(parameter);
    unsigned long previousMillis = 0;
    while(dash->serverStarted) {
        dash->handleClient();
        vTaskDelay(10);

        if(millis() - previousMillis >= 10000 && dash->currentMode == NetworkMode::STATION) {
            previousMillis = millis();
            dash->isWifiConnected = WiFi.status() == WL_CONNECTED;

            if (!dash->isWifiConnected) {
                dash->handleNetworkFailure();
            }
        }
    }
    vTaskDelete(NULL);
}

void OTADash::handleWifiScanResult(int scanResult) {
    if (scanResult == WIFI_SCAN_FAILED) {
        Serial.println("[OTADash]: Wi-Fi scan failed");
    } else {       
        Serial.println("[OTADash]: Wi-Fi scan completed");
        String networks = "[";
        for (int i = 0; i < scanResult; i++) {
            if (i > 0) networks += ",";
            String ssidEscaped = WiFi.SSID(i);
            ssidEscaped.replace("\"", "\\\""); // Escape quotes
            networks += "{\"ssid\":\"" + ssidEscaped + "\",";
            networks += "\"rssi\":" + String(WiFi.RSSI(i)) + ",";
            networks += "\"channel\":" + String(WiFi.channel(i)) + ",";
            networks += "\"encryption\":\"" + encryptionTypeToString(WiFi.encryptionType(i)) + "\"}";
        }
        networks += "]";
        ws->textAll(networks); // Send the results over WebSocket
    }
    WiFi.scanDelete(); // Clear the scan results
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
            Serial.println("[OTADash]: Attempting to reconnect to WiFi...");
            reconnectWifi();
            reconnectCount++;
            lastReconnectAttempt = millis();
        } else {
            Serial.println("[OTADash]: Max reconnection attempts reached");
            reconnectCount = 0;
        }
    }
}

void OTADash::onWifiSaved(std::function<void(const String&, const String&)> callback) {
    wifiSavedCallback = callback;
}

void OTADash::onPaired(std::function<void(JsonDocument&)> callback) {
    pairingCallback = callback;
}