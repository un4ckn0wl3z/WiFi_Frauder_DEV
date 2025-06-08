// functions.cpp
#include "main.h"

bool readSSIDList() {
    File file = SPIFFS.open("/wlist.txt", "r");
    if (!file) {
        Serial.println(F("Failed to open wlist.txt"));
        return false;
    }

    custom_ssid_count = 0;
    while (file.available() && custom_ssid_count < 64) {
        String line = file.readStringUntil('\n');
        line.trim();
        if (line.length() > 0) {
            custom_ssids[custom_ssid_count] = line;
            custom_ssid_count++;
        }
    }
    file.close();
    return custom_ssid_count > 0;
}

void displayLogoAnimation() {
    // Slide logo up from bottom to top
    for (int y = 0; y <= 64; y += 1) {
        display.clearDisplay();
        display.drawBitmap(0, y, DEVIL_LOGO, 128, 64, SSD1306_WHITE);
        display.display();
    }
    // Slide logo down from top to final position
    for (int y = 64; y >= 0; y -= 1) {
        display.clearDisplay();
        display.drawBitmap(0, y, DEVIL_LOGO, 128, 64, SSD1306_WHITE);
        display.display();
    }
    // Display final logo position
    display.clearDisplay();
    display.drawBitmap(0, 0, DEVIL_LOGO, 128, 64, SSD1306_WHITE);
    display.display();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
}

void displayMenu() {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(20, 0);
    display.println(F("WiFi Frauder"));
    for (int i = 0; i < 4; i++) {
        display.setCursor(10, 20 + i * 10);
        if (i == current_menu) {
            display.print(F("> "));
        } else {
            display.print(F("  "));
        }
        display.println(menu_items[i]);
    }
    display.display();
}

String generateRandomSSID() {
    const char chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    String ssid = "";
    for (int i = 0; i < 8; i++) {
        ssid += chars[random(0, strlen(chars))];
    }
    return ssid;
}

void displayTask(void *pvParameters) {
    while (true) {
        if (menu_active && !spamming) {
            displayMenu();
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void spamRandomSSIDSlow(void *pvParameters) {
    spamming = true;
    WiFi.softAP("InitialSSID"); // Initialize AP
    while (spamming) {
        String ssid = generateRandomSSID();
        WiFi.softAP(ssid.c_str(), NULL, random(1, 12));
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(20, 20);
        display.println(F("Random SSID (Slow)"));
        display.setCursor(20, 40);
        display.println(ssid);
        display.display();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    WiFi.softAPdisconnect(true);
    vTaskDelete(NULL);
}

void spamCustomSSID(void *pvParameters) {
    spamming = true;
    WiFi.softAP("InitialSSID"); // Initialize AP
    if (custom_ssid_count == 0) {
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(10, 20);
        display.println(F("No SSIDs in wlist.txt"));
        display.display();
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        spamming = false;
        menu_active = true;
        WiFi.softAPdisconnect(true);
        vTaskDelete(NULL);
        return;
    }
    while (spamming) {
        for (int i = 0; i < custom_ssid_count; i++) {
            if (!spamming) break;
            WiFi.softAP(custom_ssids[i].c_str(), NULL, random(1, 12));
            display.clearDisplay();
            display.setTextSize(1);
            display.setTextColor(SSD1306_WHITE);
            display.setCursor(10, 20);
            display.println(F("Custom SSID Spam"));
            display.setCursor(10, 40);
            display.println(custom_ssids[i]);
            display.display();
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
    }
    WiFi.softAPdisconnect(true);
    vTaskDelete(NULL);
}

void spamRandomSSIDFast(void *pvParameters) {
    spamming = true;
    WiFi.softAP("InitialSSID"); // Initialize AP
    while (spamming) {
        for (int i = 0; i < num_ssids; i++) {
            if (!spamming) break;
            WiFi.softAP(ssids[i].c_str(), NULL, random(1, 12));
            display.clearDisplay();
            display.setTextSize(1);
            display.setTextColor(SSD1306_WHITE);
            display.setCursor(10, 20);
            display.println(F("Random SSID (Fast)"));
            display.setCursor(10, 40);
            display.println(ssids[i]);
            display.display();
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
    }
    WiFi.softAPdisconnect(true);
    vTaskDelete(NULL);
}