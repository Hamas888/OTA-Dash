# OTA-Dash

**OTA-Dash** is a lightweight, plug-and-play web-based dashboard for **ESP32** boards. It enables developers to easily perform OTA firmware updates, view real-time logs, configure Wi-Fi, and manage EEPROM — all through a self-hosted web interface launched automatically on device startup.

This makes **OTA-Dash** the perfect foundation for wireless ESP32-based IoT applications.

<p float="left">
  <img src="https://drive.google.com/uc?export=view&id=1e4YEDkk-VxShtc-lEdMTcfTe6KJGad_c" width="300"/>
  <img src="https://drive.google.com/uc?export=view&id=1o2v3N3FQZ6gbugEWfdIV8cAaacaHumOK" width="300"/>
  <img src="https://drive.google.com/uc?export=view&id=1HH2neCO7Lo-CBrbSsdSSZbog_s6-COyo" width="300"/>
  <img src="https://drive.google.com/uc?export=view&id=1NY8f-K08Ymx7n66Z_tx4gfBJwhYkKW05" width="300"/>
  <img src="https://drive.google.com/uc?export=view&id=1TkqiBSwDM08ZmCffWzjmslpVc4k8ylvU" width="300"/>
  <img src="https://drive.google.com/uc?export=view&id=1NfvBIrgzHX6G0DHLDz2o2cMpGHtgv3uG" width="300"/>
</p>

---

## 🌐 Table of Contents

- [Features](#features)
- [Getting Started](#getting-started)
  - [PlatformIO Integration](#platformio-integration)
  - [Arduino IDE Integration](#arduino-ide-integration)
- [Web Dashboard](#web-dashboard)
- [Dependencies](#dependencies)
- [Planned Improvements](#planned-improvements)
- [Example Usage](#example-usage)
- [License](#license)
- [Contributions](#contributions)

---

## ✨ Features

- 📶 Auto-launch Access Point with web portal
- ⚙️ Wi-Fi configuration and reset
- 🔄 OTA firmware upload via web interface
- 📜 Real-time serial log streaming
- 💾 EEPROM read/write management
- 🔐 Optional password protection
- 🧠 Minimal memory footprint
- 🌍 Custom `.local` domain support (e.g., `ota.local`)

---

## 🚀 Getting Started

### PlatformIO Integration

1. Add this library to your `platformio.ini`:
   ```ini
   lib_deps = 
     https://github.com/Hamas888/OTA-Dash.git
   ````

2. Include the library in your code:

   ```cpp
   #include <OTADash.h>
   ```

3. Initialize and launch:

   ```cpp
   OTADash dash("OTA_SSID", "12345678", "ota", "My OTA Portal");
   dash.begin();
   ```

### Arduino IDE Integration

1. Clone the repository:

   ```sh
   git clone https://github.com/Hamas888/OTA-Dash.git
   ```

2. Copy the folder into your Arduino `libraries/` directory.

3. Restart Arduino IDE and include the library:

   ```cpp
   #include <OTADash.h>
   ```

---

## 🖥 Web Dashboard

Once the ESP32 boots up:

* Connect to the Wi-Fi network: `OTA_SSID`
* Open your browser and visit: `http://ota.local` *(or use the IP shown in the Serial Monitor)*
* The web dashboard includes:

  * OTA upload form
  * EEPROM viewer/editor
  * Live serial logs
  * Wi-Fi setup/reset

---

## 📦 Dependencies

OTA-Dash relies on the following libraries. These **must be installed** in your project via PlatformIO or Arduino Library Manager:

* [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)
* [AsyncTCP](https://github.com/me-no-dev/AsyncTCP)
* [ArduinoJson](https://github.com/bblanchon/ArduinoJson)

---

## 🔮 Planned Improvements

* 🧪 Improved EEPROM interface (JSON-based or structured key-value config)
* 🔐 Login/authentication system
* 💻 SPIFFS/LittleFS file manager with drag-and-drop
* 📈 Real-time system info (RAM, Flash, uptime, etc.)
* 🌐 Dual AP + STA support with fallback handling
* 🧾 JSON config import/export
* 📘 Onboard help/documentation page

---

## 📁 Example Usage

```cpp
#include <OTADash.h>

OTADash dash("OTA_SSID", "12345678", "ota", "My OTA Portal");

void setup() {
  Serial.begin(115200);
  dash.begin();
}

void loop() {
  dash.handle();
}
```

---

## 📄 License

This project is licensed under the [MIT License](LICENSE).

> ⚠️ Dependencies like `ESPAsyncWebServer`, `AsyncTCP`, and `ArduinoJson` are under their own licenses (LGPL/MIT). They are **dynamically linked** and **not modified** in this project.

---

## 🙌 Contributions

Bug reports, feature suggestions, and pull requests are warmly welcome.
Let’s make ESP32 development faster and easier together!
Fork the repo, star the project ⭐, and contribute to the next-gen dashboard.

---
