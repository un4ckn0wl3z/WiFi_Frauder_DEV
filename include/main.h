// main.h
#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <FS.h>
#include <SPIFFS.h>

// OLED configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
extern Adafruit_SSD1306 display;

// Pin definitions
#define SDA_PIN 16
#define SCL_PIN 17
#define BTN_DOWN 23
#define BTN_UP 22
#define BTN_ENTER 21

// Devil logo
extern const uint8_t DEVIL_LOGO[];

// Menu and state variables
extern const char* menu_items[];
extern int current_menu;
extern volatile bool menu_active;
extern volatile bool spamming;
extern String custom_ssids[];
extern int custom_ssid_count;
extern const int num_ssids;
extern String ssids[];
extern TaskHandle_t displayTaskHandle;
extern TaskHandle_t wifiTaskHandle;

// Function declarations
void displayLogoAnimation();
void displayMenu();
String generateRandomSSID();
void spamRandomSSIDSlow(void *pvParameters);
void spamCustomSSID(void *pvParameters);
void spamRandomSSIDFast(void *pvParameters);
void displayTask(void *pvParameters);
bool readSSIDList();

#endif