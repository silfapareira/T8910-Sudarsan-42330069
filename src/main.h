#ifndef MAIN_H
#define MAIN_H
#include <Arduino.h>
#include <params.h>
#include <secret.h>
#include <UrusanWiFi.h>
#include <UrusanIoT.h>
#include <ArduinoJson.h>
#include <UrusanLED.h>
#include <BH1750.h>
#include <Wire.h>


// Tulis parameter lokal mu di sini
const uint8_t pinLED = 27;
const uint8_t pinDHT = 4;
String clientId = String(CURRENT_FIRMWARE_TITLE) + String(NIM) + String("-") + String(random(1, 9999)); // Client ID untuk MQTT
unsigned long lastSent = 0;
bool flagUpdate = false;
float lastLux = 0;


int bottomThreshold = 1000;
int upperThreshold = 4000;


// Tulis instansiasi objek global mu di sini
UrusanWiFi urusanWiFi(wssid, wpass);
UrusanLED urusanLED(pinLED, pinLED, pinLED); // LED RGB dengan warna putih
UrusanIoT urusanIoT(tbAddr, tbPort, clientId.c_str(), "latihan", "bukanbudakai");
BH1750 lightMeter;


// Tulis deklarasi fungsi mu di sini:
void cetakIdentitasDeveloper();
void penangkapPesan(String topic, String message);
void subscribe();


#endif
