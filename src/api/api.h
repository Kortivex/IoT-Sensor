#include <HTTPClient.h>
#include <string>


class API {
public:
    String ApiHost = "";
    String ApiKey = "";

    API(String apiHost, String apiKey);

    void PostSensorData(String id, String data) const;

    void PostSensorConnectionData(String id, String data) const;

private:
    static void processResponse(int httpCode, HTTPClient &http);
};