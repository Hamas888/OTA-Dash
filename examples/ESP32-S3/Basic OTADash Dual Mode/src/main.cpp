#include <Arduino.h>
#include "OTADash.h"

OTADash* otaDash = new OTADash("OTADash", "", "ota-dash", "OTA Dash");

void setup() {
  Serial.begin(115200);
  otaDash-setProductName("OTA Dash Tester");
  otaDash->setFirmwareVersion("1.0.0");
  otaDash->begin(DUAL);
}

void loop() {
  otaDash->printDebug("Hello World");
  delay(1000);
}