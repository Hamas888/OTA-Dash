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

#ifndef WEBPAGES_BUTTON_COLOR
  #define WEBPAGES_BUTTON_COLOR "#00838f"
#endif

#ifndef WEBPAGES_BUTTON_TEXT_COLOR
  #define WEBPAGES_BUTTON_TEXT_COLOR "#ffffff"
#endif

#ifndef WEBPAGES_ACCENT_COLOR
  #define WEBPAGES_ACCENT_COLOR "#004d40"
#endif

#ifndef WEBPAGES_PRIMARY_COLOR
  #define WEBPAGES_PRIMARY_COLOR "#006064"
#endif

#ifndef WEBPAGES_BACKGROUND_COLOR
  #define WEBPAGES_BACKGROUND_COLOR "#e0f7fa"
#endif

#ifndef WEBPAGES_BACKGROUND_2_COLOR
  #define WEBPAGES_BACKGROUND_2_COLOR "#ffffff"
#endif

static const char index_html[] = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <title>Device Control Portal</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body {
      display: flex;
      justify-content: center; 
      align-items: center;
      text-align: center; 
      font-family: "Arial"; 
      background-color: )rawliteral" WEBPAGES_BACKGROUND_COLOR R"rawliteral(; 
      color: )rawliteral" WEBPAGES_PRIMARY_COLOR R"rawliteral(; 
      margin: 0; 
      padding: 0; 
    }
    .container {
      width: 400px; 
      height: auto; 
      display: flex; 
      flex-direction: column;
      align-items: center;
      margin-top: 50px; 
      background: )rawliteral" WEBPAGES_BACKGROUND_2_COLOR R"rawliteral(; 
      padding: 20px; 
      border-radius: 10px; 
      box-shadow: 0 0 10px rgba(0, 0, 0, 0.1); 
    }
    .button { 
      padding: 10px 20px; 
      font-size: 18px; 
      margin: 10px; 
      cursor: pointer; 
      border: none; 
      border-radius: 5px; 
      background-color: )rawliteral" WEBPAGES_BUTTON_COLOR R"rawliteral(; 
      color: )rawliteral" WEBPAGES_BUTTON_TEXT_COLOR R"rawliteral(; 
      text-decoration: none; 
      display: inline-block;
      width: 160px; 
      height: 20px;
      text-align: center;
      line-height: 20px;
    }
    .note {
      margin-top: 10px; 
      font-size: 14px; 
      color: )rawliteral" WEBPAGES_ACCENT_COLOR R"rawliteral(; 
    }
    .note h5 {
      margin-bottom: 5px; 
    }
    .separator {
      width: 100%; 
      height: 1px;
      background-color: )rawliteral" WEBPAGES_ACCENT_COLOR R"rawliteral(; 
      margin: 20px 0px;
    }
  </style>
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

static const char device_info_html[] = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>Device Info</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body {
      display: flex;
      justify-content: center; 
      align-items: center;
      text-align: center; 
      font-family: "Arial"; 
      background-color:  )rawliteral"  WEBPAGES_BACKGROUND_COLOR  R"rawliteral(; 
      color: )rawliteral"  WEBPAGES_PRIMARY_COLOR  R"rawliteral(; 
      margin: 0; 
      padding: 0; 
    }
    .container {
    width: 400px; 
      height: auto;
      display: flex; 
      flex-direction: column;
      align-items: center;
      margin-top: 50px; 
      background: )rawliteral" WEBPAGES_BACKGROUND_2_COLOR R"rawliteral(; 
      padding: 20px; 
      border-radius: 10px; 
      box-shadow: 0 0 10px rgba(0, 0, 0, 0.1); 
    }
    .button { 
      padding: 10px 20px; 
      font-size: 18px; 
      margin: 10px; 
      cursor: pointer; 
      border: none; 
      border-radius: 5px; 
      background-color: )rawliteral" WEBPAGES_BUTTON_COLOR R"rawliteral(; 
      color: )rawliteral" WEBPAGES_BUTTON_TEXT_COLOR R"rawliteral(; 
      text-decoration: none; 
      display: inline-block;
      width: 140px; 
      height: 20px;
      text-align: center;
      line-height: 20px;
    }
    table {
      width: 100%;
      table-layout: fixed;
      border-collapse: collapse;
      border-collapse: collapse;
      margin: 20px 0;
    }
    th, td {
      padding: 10px;
      border: 1px solid #ddd;
      word-wrap: break-word;
      word-break: break-word;
      white-space: normal;
      text-align: left;
    }
    th {
      background-color: #f2f2f2;
      text-align: center;
    }
    tr {
      height: 50px; 
    }
    .footer {
      margin-top: 20px;
      padding-top: 15px;
      border-top: 1px solid #ddd;
      font-size: 11px;
      color: #666;
    }
    .footer a {
      color: #00838f;
      text-decoration: none;
    }
    .footer a:hover {
      text-decoration: underline;
    }
  </style>
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

static const char wifi_manage_html[] = R"rawliteral(
<!DOCTYPE HTML>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>WiFi Management</title>
  <style>
    body {
      display: flex;
      justify-content: center;
      align-items: center;
      text-align: center;
      font-family: "Arial";
      background-color: #e0f7fa;
      color: #006064;
      margin: 0;
      padding: 0;
    }
    .container {
      width: 400px;
      height: auto;
      display: flex;
      flex-direction: column;
      align-items: center;
      margin-top: 50px;
      background: #ffffff;
      padding: 20px;
      border-radius: 10px;
      box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
    }
    .button {
      padding: 10px 20px;
      font-size: 18px;
      margin: 10px;
      cursor: pointer;
      border: none;
      border-radius: 5px;
      background-color: #00838f;
      color: #ffffff;
      text-decoration: none;
      display: inline-block;
      width: 140px;
      text-align: center;
    }
    h1 {
      font-size: 24px;
      margin-bottom: 20px;
    }
    .form-group {
      width: 100%;
      margin-bottom: 15px;
    }
    label {
      display: block;
      margin-bottom: 5px;
      font-size: 16px;
      color: #004d40;
    }
    input[type="text"], input[type="password"], button {
      width: 100%;
      padding: 10px;
      margin-bottom: 10px;
      border: 1px solid #ddd;
      border-radius: 4px;
      box-sizing: border-box;
    }
    .wifi-list {
      margin-top: 20px;
      width: 100%;
      padding: 10px;
      background-color: white;
      border-radius: 8px;
      box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
      max-height: 250px;
      overflow-y: auto;
      margin-bottom: 30px;
    }
    .wifi-item {
      padding: 10px;
      border-bottom: 1px solid #ddd;
      cursor: pointer;
    }
    .wifi-item:last-child {
      border-bottom: none;
    }
    .wifi-item:hover {
      background-color: #f0f0f0;
    }
    .note {
      margin-top: 10px;
      font-size: 14px;
      color: #004d40;
    }
    .note h5 {
      margin-bottom: 5px;
    }
    .separator {
      width: 100%;
      height: 1px;
      background-color: #004d40;
      margin: 20px 0px;
    }
    .button-container {
      display: flex;
      justify-content: center;
      gap: 20px;
      width: 100%;
    }
  </style>
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

static const char update_firmware_html[] = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <title>Firmware Update</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body {
      display: flex;
      justify-content: center; 
      align-items: center;
      text-align: center; 
      font-family: "Arial"; 
      background-color: #e0f7fa; 
      color: #006064; 
      margin: 0; 
      padding: 0; 
    }
    .container {
      width: 400px; 
      height: auto; 
      display: flex; 
      flex-direction: column;
      align-items: center;
      margin-top: 50px; 
      background: #ffffff; 
      padding: 20px; 
      border-radius: 10px; 
      box-shadow: 0 0 10px rgba(0, 0, 0, 0.1); 
    }
    .button { 
      padding: 10px 20px; 
      font-size: 18px; 
      margin: 10px; 
      cursor: pointer; 
      border: none; 
      border-radius: 5px; 
      background-color: #00838f; 
      color: #ffffff; 
      text-decoration: none; 
      display: inline-block; 
    }
    input[type="submit"] {
      padding: 10px 20px; 
      font-size: 18px; 
      margin: 20px; 
      cursor: pointer; 
      border: none; 
      border-radius: 5px; 
      background-color: #00838f; 
      color: #ffffff; 
      text-decoration: none; 
      display: inline-block;
      width: 180px; 
      height: 40px; 
      text-align: center;
      line-height: 20px; 
    }

    input[type="file"] { 
      padding: 10px; 
      border: 2px dashed #00838f; 
      border-radius: 5px; 
      width: 200px; 
      cursor: pointer; 
    }
    #progressContainer {
      width: 100%;
      margin-top: 20px;
      display: none; /* Hidden initially */
      border: 2px solid #00838f; /* Add border to make it more visible */
      border-radius: 5px;
      position: relative;
    }
    #progressBar {
      width: 0%;
      height: 20px;
      background-color: #00838f;
      border-radius: 5px;
      transition: width 0.4s ease;
      position: relative;
    }
    #progressText {
      position: absolute;
      width: 100%;
      text-align: center;
      top: 0;
      left: 0;
      color: #ffffff;
      font-weight: bold;
      line-height: 20px; /* Align text vertically */
    }
  </style>
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

static const char erase_settings_html[] = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>Erase Settings</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body {
      display: flex;
      justify-content: center; 
      align-items: center;
      text-align: center; 
      font-family: "Arial"; 
      background-color: #e0f7fa; 
      color: #006064; 
      margin: 0; 
      padding: 0; 
    }
    .container {
      width: 400px; 
      height: auto; 
      display: flex; 
      flex-direction: column;
      align-items: center;
      margin-top: 50px; 
      background: #ffffff; 
      padding: 20px; 
      border-radius: 10px; 
      box-shadow: 0 0 10px rgba(0, 0, 0, 0.1); 
    }
    input[type="submit"] {
      padding: 10px 20px; 
      font-size: 18px; 
      margin: 20px; 
      cursor: pointer; 
      border: none; 
      border-radius: 5px; 
      background-color: #d32f2f; 
      color: #ffffff; 
      text-decoration: none; 
      display: inline-block;
      width: 180px; 
      height: 40px; 
      text-align: center;
      line-height: 20px; 
    }
    .button { 
      padding: 10px 20px; 
      font-size: 18px; 
      margin: 10px; 
      cursor: pointer; 
      border: none; 
      border-radius: 5px; 
      background-color: #00838f; 
      color: #ffffff; 
      text-decoration: none; 
      display: inline-block; 
    }
  </style>
</head>
<body>
  <div class="container">
    <h1>Erase Settings</h1>
    <form method="POST" action="/erase">
      <input type="submit" value="Erase Settings" class="button">
    </form>
    <a href="/" class="button">Back</a>
  </div>
</body>
</html>
)rawliteral";

static const char restart_device_html[] = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>Restart Device</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body {
      display: flex;
      justify-content: center; 
      align-items: center;
      text-align: center; 
      font-family: "Arial"; 
      background-color: #e0f7fa; 
      color: #006064; 
      margin: 0; 
      padding: 0; 
    }
    .container {
      width: 400px; 
      height: auto; 
      display: flex; 
      flex-direction: column;
      align-items: center;
      margin-top: 50px; 
      background: #ffffff; 
      padding: 20px; 
      border-radius: 10px; 
      box-shadow: 0 0 10px rgba(0, 0, 0, 0.1); 
    }
    input[type="submit"] {
      padding: 10px 20px; 
      font-size: 18px; 
      margin: 20px; 
      cursor: pointer; 
      border: none; 
      border-radius: 5px; 
      background-color: #f09c3d; 
      color: #ffffff; 
      text-decoration: none; 
      display: inline-block;
      width: 180px; 
      height: 40px; 
      text-align: center;
      line-height: 20px; 
    }
    .button { 
      padding: 10px 20px; 
      font-size: 18px; 
      margin: 10px; 
      cursor: pointer; 
      border: none; 
      border-radius: 5px; 
      background-color: #00838f; 
      color: #ffffff; 
      text-decoration: none; 
      display: inline-block; 
    }
  </style>
</head>
<body>
  <div class="container">
    <h1>Restart Device</h1>
    <form id="restartForm">
      <input type="button" value="Restart Device" class="button" onclick="submitRestart()">
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

const char debug_html[] = R"rawliteral(
<!DOCTYPE HTML>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>Wireless Debug</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body {
      display: flex;
      justify-content: center; 
      align-items: center;
      text-align: center; 
      font-family: "Arial"; 
      background-color: #e0f7fa; 
      color: #006064; 
      margin: 0; 
      padding: 0; 
    }
    .container {
      width: 400px; 
      height: auto; 
      display: flex; 
      flex-direction: column;
      align-items: center;
      margin-top: 50px; 
      background: #ffffff; 
      padding: 20px; 
      border-radius: 10px; 
      box-shadow: 0 0 10px rgba(0, 0, 0, 0.1); 
    }
    .log-screen {
      border: 1px solid #ccc;
      background-color: #fff;
      width: 300px;
      height: 500px;
      overflow-y: scroll;
      padding: 10px;
      text-align: left; 
    }
    .button { 
      padding: 10px 20px; 
      font-size: 18px; 
      margin: 10px; 
      cursor: pointer; 
      border: none; 
      border-radius: 5px; 
      background-color: #00838f; 
      color: #ffffff; 
      text-decoration: none; 
      display: inline-block; 
    }
  </style>
</head>
<body>
  <div class="container">
    <h1>%PORTAL_HEADING% Debug Logs</h1>
    <div class="log-screen" id="logs">
    </div>
    <a href="/" class="button">Back</a>
  </div>
  <script>
    var ws = new WebSocket(`ws://${window.location.hostname}/ws`);
    ws.onmessage = function(event) {
      var logsDiv = document.getElementById('logs');
      logsDiv.innerHTML += event.data + "<br/>";
      logsDiv.scrollTop = logsDiv.scrollHeight; // Auto-scroll to the bottom
    };
  </script>
</body>
</html>
)rawliteral";

static const char about_html[] = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <title>About OTA-Dash</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body {
      display: flex;
      justify-content: center; 
      align-items: center;
      text-align: center; 
      font-family: "Arial"; 
      background-color: #e0f7fa; 
      color: #006064; 
      margin: 0; 
      padding: 0; 
    }
    .container {
      width: 400px; 
      height: auto;
      display: flex; 
      flex-direction: column;
      align-items: center;
      margin-top: 50px; 
      background: #ffffff; 
      padding: 20px; 
      border-radius: 10px; 
      box-shadow: 0 0 10px rgba(0, 0, 0, 0.1); 
    }
    .button { 
      padding: 10px 20px; 
      font-size: 18px; 
      margin: 10px; 
      cursor: pointer; 
      border: none; 
      border-radius: 5px; 
      background-color: #00838f; 
      color: #ffffff; 
      text-decoration: none; 
      display: inline-block;
      width: 140px; 
      height: 20px;
      text-align: center;
      line-height: 20px;
    }
    .button:hover {
      background-color: #006064;
      transition: 0.3s;
    }
    .content {
      text-align: left;
      margin: 20px 0;
      font-size: 14px;
      line-height: 1.8;
    }
    .header {
      text-align: center;
      margin-bottom: 20px;
    }
    .header h1 {
      margin: 0;
      color: #00838f;
    }
    .header p {
      margin: 5px 0;
      font-size: 12px;
      color: #666;
    }
    .credits {
      background-color: #f5f5f5;
      padding: 15px;
      border-radius: 5px;
      margin: 15px 0;
    }
    .credits h3 {
      margin-top: 0;
      color: #004d40;
      font-size: 14px;
    }
    .credits p {
      margin: 8px 0;
      font-size: 13px;
    }
    .credits a {
      color: #00838f;
      text-decoration: none;
    }
    .credits a:hover {
      text-decoration: underline;
    }
    .separator {
      width: 100%; 
      height: 1px;
      background-color: #ddd; 
      margin: 15px 0;
    }
  </style>
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