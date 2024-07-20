#include <WiFi.h>
#include "configuration.h"

void setupWiFi() {
    WiFiClass::mode(WIFI_STA);

    Serial.print("Connect SSID: ");
    Serial.print(WIFI_SSID);
    Serial.print(" Password: ");
    Serial.println(WIFI_PASSWD);

    WiFi.begin(WIFI_SSID, WIFI_PASSWD);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.println("WiFi connect fail!");
        delay(3000);
        esp_restart();
    }
    Serial.println("WiFi connect success: ");
    Serial.print("- IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.print("- Subnet Mask: ");
    Serial.println(WiFi.subnetMask().toString());
    Serial.print("- MAC Address: ");
    Serial.println(WiFi.macAddress());
}