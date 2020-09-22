
#include <classes/API.h>

const char CONTENT_LENGTH_STR[] = "Content-Length: ";
const int CONTENT_LENGTH_LENGTH = strlen(CONTENT_LENGTH_STR);

API::API() {
  inflightReq = false;
  buffIdx = 0;
  contentLengthIdx = 0;
  consumingContentLength = false;
}

void API::initialize() {
  WiFi.mode(WIFI_STA);
  delay(100);
  WiFi.begin("guest :)");
  while (WiFi.status() != WL_CONNECTED) {
    delay(50);
  }
}

void API::Sweep() {
  if (inflightReq) {
    while (client.available()) {
      char c = client.read();
      buff[buffIdx] = c;
      // searching for content length...
      if (consumingContentLength) {
        if (strcmp(&c, "\n") == 0) {
          consumingContentLength = false;
        } else {
          detectedContentLength += buff[buffIdx];
        }
      } else {
        if (buff[buffIdx] == CONTENT_LENGTH_STR[contentLengthIdx]) {
          contentLengthIdx++;
          if (contentLengthIdx == CONTENT_LENGTH_LENGTH) {
            consumingContentLength = true;
            contentLengthIdx = 0;
          }
        } else {
          contentLengthIdx = 0;
        }
      }
      //
      buffIdx++;
    }
    // no idea if this should go here or what
    if (!client.connected()) {
      Serial.println();
      Serial.println("disconnecting from server.");
      inflightReq = false;
      client.stop();

      _processResponseBuffer(buffIdx, detectedContentLength);
      buffIdx = 0;
      detectedContentLength = String();
      consumingContentLength = false;
      contentLengthIdx = 0;
    }
  }
}

void API::Request(HTTP_METHOD_ENUM method, char *host, char *endpoint) {
  inflightReq = true;
  if (client.connect(host, 80)) {
    // Make a HTTP request:
    char reqStr[1000];
    sprintf(reqStr, "GET %s HTTP/1.1", endpoint);
    client.println(reqStr);
    char hostStr[1000];
    sprintf(hostStr, "Host: %s", host);
    client.println(hostStr);
    client.println("Connection: close");
    client.println();
  }
}

void API::_processResponseBuffer(int i, String cLength) {
  // deserialize the object
  String rawJson = String(buff).substring(i - cLength.toInt(), i);

  // allocate the memory for the document
  const size_t CAPACITY = JSON_OBJECT_SIZE(1);
  StaticJsonDocument<CAPACITY> doc;

  char jsonBuff[rawJson.length()];
  rawJson.toCharArray(jsonBuff, rawJson.length() + 1);
  deserializeJson(doc, jsonBuff, i);
  
  // extract the data
  JsonObject object = doc.as<JsonObject>();
  const char* f = object["someprop"];

  Serial.println("final:");
  Serial.println(f);
}