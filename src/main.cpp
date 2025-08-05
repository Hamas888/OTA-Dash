#include <Arduino.h>
#include "OTADash.h"

OTADash *dash = new OTADash("AMS-ADMIN", "amsad@96$","ota_dash", "OTA-Dash");

void setup() {
  Serial.begin(115200);
  dash->begin(NetworkMode::DUAL);
}

void loop() {
  delay(1000);
  dash->printDebug("Hello World!");
}
