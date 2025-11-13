   /*
 ====================================================================================================
 * File:        WebPages.h
 * Author:      Hamas Saeed
 * Version:     Rev_1.0.0
 * Date:        Feb 10 2025
 * Brief:       This File Contains HTML Templates For Web Pages Used In OTADash
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

#ifndef WEBPAGES_H
#define WEBPAGES_H

#define OTADASH_DEVICE_INFO               1
#define OTADASH_WIFI_MANAGER              1
#define OTADASH_FIRMWARE_UPDATE           1
#define OTADASH_ERASE_SETTINGS            1
#define OTADASH_DEBUG_PAGE                1

static const char index_html[] = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <title>Device Control Portal</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="/styles.css">
</head>
<body>
  <div class="container">
    <h1>%PORTAL_HEADING%</h1>
    <a href="/info" class="button">Device Info</a>
    <a href="/wifimanage" class="button">Manage WIFI</a>
    <a href="/debug" class="button">Wireless Debug</a>
    <a href="/update" class="button">Update Firmware</a>
    <a href="/erase" class="button">Erase Settings</a>
    <a href="/restart" class="button">Restart Device</a>
    <div class="separator"></div> 
    <div class="note">
      <h5>Note</h5>
      If the update function isn't working, open the portal in your browser: <a href="http://%CUSTOM_DOMAIN%">%CUSTOM_DOMAIN%</a>
    </div>
  </div>
</body>
</html>
)rawliteral";

#if OTADASH_DEVICE_INFO
static const char device_info_html[] = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>Device Info</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="/styles.css">
</head>
<body>
  <div class="container">
    <h1>Device Info</h1>
    <table>
      <tr><th>Property</th><th>Value</th></tr>
      %DEVICE_INFO%
    </table>
    <a href="/" class="button">Back</a>
    <a href="/about" class="button">About</a>
    <div class="footer">
      Developed by <a href="https://github.com/Hamas888" target="_blank">Hamas Saeed</a>
    </div>
  </div>
</body>
</html>
)rawliteral";
#endif // OTADASH_DEVICE_INFO

#if OTADASH_WIFI_MANAGER
static const char wifi_manage_html[] = R"rawliteral(
<!DOCTYPE HTML>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>WiFi Management</title>
  <link rel="stylesheet" href="/styles.css">
</head>
<body>
  <div class="container">
    <h1>WiFi Management</h1>
    <div class="wifi-list" id="wifiList">
        <p>Scanning for networks...</p>
    </div>

    <div class="form-group">
      <label for="ssid">SSID</label>
      <input type="text" id="ssid" placeholder="SSID" readonly>
    </div>
    <div class="form-group">
      <label for="password">Password</label>
      <input type="password" id="password" placeholder="Password">
    </div>
    <div class="button-container">
      <button class="button" onclick="saveWifi()">Save</button>
      <button class="button" onclick="window.location.href='/'">Back</button>     
    </div>
    <div class="separator"></div>
    <div class="note">
        <h5>Note</h5>
        If the device is not connecting, ensure the credentials are correct.
    </div>
  </div>

  <script>
    let socket = new WebSocket(`ws://${window.location.hostname}/ws`);

    const wifiListContainer = document.getElementById("wifiList");
    const ssidInput = document.getElementById("ssid");
    const passwordInput = document.getElementById("password");

    function addWifiNetwork(network) {
      const wifiItem = document.createElement("div");
      wifiItem.classList.add("wifi-item");
      wifiItem.textContent = network.ssid + " (" + network.rssi + " dBm)";
      wifiItem.onclick = () => {
        ssidInput.value = network.ssid;
      };
      wifiListContainer.appendChild(wifiItem);
    }

    socket.onmessage = function(event) {
      const networks = JSON.parse(event.data);
      wifiListContainer.innerHTML = "";
      if (networks.length > 0) {
        networks.forEach(addWifiNetwork);
      } else {
        wifiListContainer.innerHTML = "<p>No networks found.</p>";
      }
    };

    socket.onopen = function() {
      console.log("WebSocket connection established.");
    };

    socket.onclose = function() {
      console.log("WebSocket connection closed.");
    };

    socket.onerror = function(error) {
      console.log("WebSocket Error: " + error);
    };

    function saveWifi() {
        const ssid = ssidInput.value;
        const password = passwordInput.value;

        if (!ssid) {
            alert("Please select a network.");
            return;
        }
        if (password.length < 8) {
            alert("Password must be at least 8 characters.");
            return;
        }

        const formData = new FormData();
        formData.append("ssid", ssid);
        formData.append("password", password);

        fetch('/save-wifi', {
            method: 'POST',
            body: formData  // Sending data as form data, not JSON
        })
        .then(response => {
            if (response.ok) {
            alert("WiFi settings saved. Restarting device...");
            window.location.href = '/';
            } else {
            alert("Failed to save WiFi settings.");
            }
        })
        .catch(error => {
            console.error('Error:', error);
            alert('Error saving WiFi settings.');
        });
        }

  </script>
</body>
</html>
)rawliteral";
#endif // OTADASH_WIFI_SETTINGS

#if OTADASH_FIRMWARE_UPDATE
static const char update_firmware_html[] = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <title>Firmware Update</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="/styles.css">
</head>
<body>
  <div class="container">
    <h1>Firmware Update</h1>
    <form id="updateForm" enctype="multipart/form-data">
      <input type="file" id="firmwareFile" name="firmware" accept=".bin" required>
      <input type="button" value="Update Firmware" class="button" id="updateButton" onclick="submitUpdate()">
    </form>
    <div id="progressContainer">
      <div id="progressBar">
        <div id="progressText">0%</div> <!-- Percentage text inside the progress bar -->
      </div>
    </div>
    <a href="/" class="button">Back</a>
    
    <script>
      function submitUpdate() {
        var firmwareFile = document.getElementById('firmwareFile');
        var updateButton = document.getElementById('updateButton');
        var progressContainer = document.getElementById('progressContainer');
        var progressBar = document.getElementById('progressBar');
        var progressText = document.getElementById('progressText');
        
        // Check if a file is selected
        if (!firmwareFile.files.length) {
          alert('Please select a firmware file.');
          return;
        }

        // Check if the selected file is a .bin file
        var fileName = firmwareFile.files[0].name;
        if (!fileName.endsWith('.bin')) {
          alert('Invalid file selected. Please select a .bin file.');
          return;
        }
    
        var formData = new FormData(document.getElementById('updateForm'));
    
        // Hide the update button and show the progress bar
        updateButton.style.display = 'none';
        progressContainer.style.display = 'block';

        // Create XMLHttpRequest object to handle the file upload
        var xhr = new XMLHttpRequest();

        // Track the upload progress
        xhr.upload.onprogress = function(event) {
          if (event.lengthComputable) {
            var percentComplete = Math.round((event.loaded / event.total) * 100);
            progressBar.style.width = percentComplete + '%';
            progressText.textContent = percentComplete + '%'; // Update the percentage text
          }
        };

        // Handle the response from the server
        xhr.onload = function() {
          if (xhr.status === 200) {
            progressText.textContent = '100%'; // Ensure the text shows 100% on completion
            alert('Firmware update successful! The device will now restart.');
            setTimeout(() => {
              location.reload();
            }, 1000);
          } else {
            alert('Firmware update failed!');
            resetUI();
          }
        };

        // Handle errors
        xhr.onerror = function() {
          alert('Firmware update encountered an error!');
          resetUI();
        };

        // Send the form data
        xhr.open('POST', '/update', true);
        xhr.send(formData);

        function resetUI() {
          updateButton.style.display = 'block';
          progressContainer.style.display = 'none';
          progressBar.style.width = '0%';
          progressText.textContent = '0%'; // Reset percentage text
        }
      }
    </script>
  </div>
</body>
</html>
)rawliteral";
#endif // OTADASH_FIRMWARE_UPDATE

#if OTADASH_ERASE_SETTINGS
static const char erase_settings_html[] = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>Erase Settings</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="/styles.css">
</head>
<body>
  <div class="container">
    <h1>Erase Settings</h1>
    <form method="POST" action="/erase">
      <input type="submit" value="Erase Settings" class="button erase-submit">
    </form>
    <a href="/" class="button">Back</a>
  </div>
</body>
</html>
)rawliteral";
#endif // OTADASH_ERASE_SETTINGS

static const char restart_device_html[] = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>Restart Device</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="/styles.css">
</head>
<body>
  <div class="container">
    <h1>Restart Device</h1>
    <form id="restartForm">
      <input type="button" value="Restart Device" class="button restart-submit" onclick="submitRestart()">
    </form>
    <a href="/" class="button">Back</a>
    
    <script>
      function submitRestart() {
        fetch('/restart', {
          method: 'POST'
        })
        .then(response => {
          if (response.ok) {
            alert('Device is restarting...');
            setTimeout(() => {
              location.reload();
            }, 1000);
          } else {
            alert('Device restart failed!');
          }
        })
        .catch(error => {
          console.error('Error:', error);
          alert('Device restart encountered an error!');
        });
      }
    </script>
  </div>
</body>
</html>
)rawliteral";

#if OTADASH_DEBUG_PAGE
const char debug_html[] = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8" />
  <title>Wireless Debug</title>
  <meta name="viewport" content="width=device-width, initial-scale=1" />
  <link rel="stylesheet" href="/styles.css" />
</head>

<body>
  <div class="container">
    <h1>%PORTAL_HEADING% Debug Logs</h1>

    <div class="log-screen" id="logs">
      <span style="color: #888888;">[Debug Console Ready - Waiting for messages...]</span><br />
    </div>

    <div style="margin-top: 10px;">
      <button onclick="clearLogs()" class="button" style="margin-right: 10px;">Clear Logs</button>
      <a href="/" class="button">Back</a>
    </div>
  </div>

  <script>
    let ws;
    const reconnectInterval = 5000;

    function connectWebSocket() {
      try {
        ws = new WebSocket(`ws://${window.location.hostname}/ws`);

        ws.onopen = () => {
          console.log("WebSocket connected");
          appendLog("[WebSocket Connected]", "#00ff00");
        };

        ws.onmessage = (event) => {
          const logsDiv = document.getElementById("logs");
          logsDiv.innerHTML += event.data + "<br/>";
          logsDiv.scrollTop = logsDiv.scrollHeight;
        };

        ws.onclose = () => {
          console.log("WebSocket disconnected");
          appendLog("[WebSocket Disconnected - Reconnecting...]", "#ff0000");
          setTimeout(connectWebSocket, reconnectInterval);
        };

        ws.onerror = (error) => {
          console.error("WebSocket error:", error);
          appendLog("[WebSocket Error]", "#ff0000");
        };
      } catch (e) {
        console.error("Failed to create WebSocket:", e);
        appendLog("[Failed to connect WebSocket]", "#ff0000");
        setTimeout(connectWebSocket, reconnectInterval);
      }
    }

    function appendLog(message, color) {
      const logsDiv = document.getElementById("logs");
      logsDiv.innerHTML += `<span style="color: ${color};">${message}</span><br/>`;
    }

    function clearLogs() {
      document.getElementById("logs").innerHTML =
        '<span style="color: #888888;">[Debug Console Cleared]</span><br/>';
    }

    connectWebSocket();
  </script>
</body>
</html>
)rawliteral";
#endif // OTADASH_DEBUG_PAGE

static const char about_html[] = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <title>About OTA-Dash</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="/styles.css">
</head>
<body>
  <div class="container">
    <div class="header">
      <h1>OTA-Dash</h1>
      <p>Firmware Update & Configuration Dashboard</p>
    </div>
    
    <div class="content">
      <p><strong>Version:</strong> 1.0.1</p>
      <p>A plug-and-play OTA and configuration dashboard for ESP32-S3 devices using a web interface.</p>
    </div>

    <div class="credits">
      <h3>Developed By</h3>
      <p>
        <a href="https://github.com/Hamas888" target="_blank"><strong>Hamas Saeed</strong></a><br>
        <a href="mailto:hamasaeed888@gmail.com">hamasaeed888@gmail.com</a>
      </p>
    </div>

    <div class="credits">
      <h3>Resources</h3>
      <p>
        <a href="https://github.com/Hamas888/OTA-Dash" target="_blank">GitHub Repository</a><br>
        <a href="https://github.com/Hamas888/OTA-Dash" target="_blank">Library Documentation</a>
      </p>
    </div>

    <div class="credits">
      <h3>License</h3>
      <p>MIT License - Free for personal and commercial use</p>
    </div>

    <div class="separator"></div>

    <a href="/" class="button">Home</a>
    <a href="/info" class="button">Device Info</a>
  </div>
</body>
</html>
)rawliteral";

#endif // WEBPAGES_H