#include "Zigbee.h"
#include <OneWire.h>

#define OW_PIN 1           
#define RAIN_PIN 2         
#define BUTTON_PIN 9       
#define TEMP_ENDPOINT 1    
#define RAIN_ENDPOINT 2    

const unsigned long interval = 1800000; // 30 минут
const unsigned long debounceDelay = 2000; // 2 сек стабильности

ZigbeeTempSensor zbTempSensor = ZigbeeTempSensor(TEMP_ENDPOINT);
ZigbeeLight zbRainSensor = ZigbeeLight(RAIN_ENDPOINT); // В Алисе как Лампа
OneWire ds(OW_PIN);

float getSimpleTemp() {
  byte data[9], addr[8];
  if (!ds.search(addr)) { ds.reset_search(); return -127.0; }
  if (OneWire::crc8(addr, 7) != addr[7] || addr[0] != 0x28) return -127.0;
  ds.reset(); ds.select(addr); ds.write(0x44); delay(850); 
  ds.reset(); ds.select(addr); ds.write(0xBE);
  for (int i = 0; i < 9; i++) data[i] = ds.read();
  return (float)((int16_t)(data[1] << 8) | data[0]) / 16.0;
}

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(RAIN_PIN, INPUT_PULLUP); 
  delay(3000);
  
  zbTempSensor.setManufacturerAndModel("DIY-C6", "Meteo-v2.5");
  zbRainSensor.setManufacturerAndModel("DIY-C6", "Rain-v2.5");
  Zigbee.addEndpoint(&zbTempSensor);
  Zigbee.addEndpoint(&zbRainSensor);

  if (!Zigbee.begin(ZIGBEE_ROUTER)) { while (1) delay(100); }
}

void loop() {
  static unsigned long lastMsg = 0, lastChangeTime = 0;
  static bool lastRainState = false;

  bool reading = (digitalRead(RAIN_PIN) == LOW);
  if (reading != lastRainState) {
    if (lastChangeTime == 0) lastChangeTime = millis();
    if (millis() - lastChangeTime > debounceDelay) {
      lastRainState = reading;
      zbRainSensor.setLight(lastRainState); 
      lastChangeTime = 0;
    }
  } else { lastChangeTime = 0; }

  if (digitalRead(BUTTON_PIN) == LOW) {
    delay(3000); if (digitalRead(BUTTON_PIN) == LOW) Zigbee.factoryReset();
  }

  if (millis() - lastMsg >= interval || lastMsg == 0) {
    lastMsg = millis();
    float t = getSimpleTemp();
    if (t > -50.0) zbTempSensor.setTemperature(t);
  }
  delay(10);
}
