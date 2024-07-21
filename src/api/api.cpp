#include "api.h"

API::API(String &apiHost, String &apiKey) {
    this->ApiHost = apiHost;
    this->ApiKey = apiKey;
}

void API::PostSensorData(String &id, String &data) const {
    if (WiFiClass::status() == WL_CONNECTED) {
        WiFiClientSecure client;
        client.setInsecure();

        HTTPClient https;
        https.begin(client, this->ApiHost + "/sensors/" + id + "/data");
        https.addHeader("Content-Type", "application/json");
        https.addHeader("Authorization", "Bearer " + this->ApiKey);

        int httpCode = https.POST(data);
        processResponse(httpCode, https);
    }
}

void API::PutSensorMetaData(String &id, String &data) const {
    if (WiFiClass::status() == WL_CONNECTED) {
        WiFiClientSecure client;
        client.setInsecure();

        HTTPClient https;
        https.begin(client, this->ApiHost + "/sensors/" + id);
        https.addHeader("Content-Type", "application/json");
        https.addHeader("Authorization", "Bearer " + this->ApiKey);

        int httpCode = https.PUT(data);
        processResponse(httpCode, https);
    }
}

void API::PutSensorConnectionData(String &id, String &data) const {
    if (WiFiClass::status() == WL_CONNECTED) {
        WiFiClientSecure client;
        client.setInsecure();

        HTTPClient https;
        https.begin(client, this->ApiHost + "/sensors/" + id);
        https.addHeader("Content-Type", "application/json");
        https.addHeader("Authorization", "Bearer " + this->ApiKey);

        int httpCode = https.PUT(data);
        processResponse(httpCode, https);
    }
}

void API::processResponse(int httpCode, HTTPClient &http) {
    if (httpCode > 0) {
        Serial.printf("\nResponse Status Code: %d\t", httpCode);

        if (httpCode >= HTTP_CODE_OK && httpCode <= HTTP_CODE_NO_CONTENT) {
            String payload = http.getString();
            Serial.printf("Response Message: %s\n", payload.c_str());
        }
    } else {
        Serial.printf("Request failed, error: %s\n", HTTPClient::errorToString(httpCode).c_str());
    }
    http.end();
}
