

#include <Arduino.h>
#include <WiFi.h>
#include <tools/Utils.h>
#include <ESPDateTime.h>
#include <ArduinoJson.h>
#include <static/http-methods.h>

class API {       // The class
  public:             // Access specifier
    API();

    void initialize();
    
    void Sweep();
    /** GET worldtime.org /api/asdf */
    void Request(HTTP_METHOD_ENUM method, char *host, char *endpoint);

    void _processResponseBuffer(int i, String cLength);

  private:
    WiFiClient client;
    char buff[2048];
    int buffIdx;
    bool inflightReq;
    int contentLengthIdx;
    bool consumingContentLength;
    String detectedContentLength;
};