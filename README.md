# 🌧️ ESP32-C6 Zigbee Temperature + Rain Sensor | Датчик температуры + дождя

[![ESP32](https://img.shields.io/badge/ESP32-C6-000000?style=for-the-badge&logo=espressif&logoColor=white)](https://www.espressif.com/)
[![Zigbee](https://img.shields.io/badge/Zigbee-Router-EB0443?style=for-the-badge&logo=zigbee&logoColor=white)](https://zigbeealliance.org/)
[![Home Assistant](https://img.shields.io/badge/Home%20Assistant-Integration-18BCF2?style=for-the-badge&logo=home-assistant&logoColor=white)](https://www.home-assistant.io/)

---

## 📋 Project Passport | Паспорт проекта

| Parameter | Значение |
|-----------|----------|
| **Version / Версия** | V1.0 |
| **Board / Плата** | ESP32-C6 DevKit (full version with Boot/Reset buttons) |
| **Network Role / Роль в сети** | Zigbee Router (ZCZR) — non-sleeping, extends network |
| **Power / Питание** | Stationary (USB / 18650 via UPS) |

---

## 🔌 Wiring | Подключение

| Component | ESP32-C6 Pin | Notes |
|-----------|--------------|-------|
| **DS18B20** (temperature) | GPIO 1 | 4.7kΩ resistor to 3.3V |
| **YL-83** (rain sensor) | GPIO 2 | Digital output (DO) |
| **Reset button** | GPIO 9 | connect to GND |

---

## ⚙️ Logic | Логика работы

| Feature | Поведение |
|---------|-----------|
| **Temperature** | Sent every 30 minutes + instantly on boot |
| **Rain detection** | Instant response with 2 sec debounce protection |
| **Status in Home Assistant** | LAMP: ON = raining / OFF = dry |
| **Bootloop protection** | Debounce algorithm prevents crashes from contact bounce |
| **Factory reset** | Hold GPIO9 button for 3 seconds |

---

## 🛠️ Arduino IDE Settings (v3.3.7+)

| Setting | Значение |
|---------|----------|
| **Board** | ESP32C6 Dev Module |
| **Zigbee Mode** | Zigbee Router and Coordinator (ZCZR) |
| **Partition Scheme** | Zigbee 4MB (ZCZR) |
| **USB CDC On Boot** | Disabled (for Zigbee stability) |
| **Erase All Flash** | Disabled (enable only on full code change) |

---

## 📦 Required Libraries | Необходимые библиотеки

```cpp
#include "Zigbee.h"      // ESP32-C6 Zigbee core
#include <OneWire.h>     // for DS18B20
