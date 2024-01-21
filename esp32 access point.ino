#include "WiFi.h"
#include "ESPAsyncWebServer.h"

#define AO_PIN 35

const char* ssid = "ESP32-Access-Point";
const char* password = "123456789";

AsyncWebServer server(80);

String readTemp() {
  int tem = analogRead(AO_PIN);
  int tempp = tem*4.88;
  tempp= tempp/10;
  return String(tempp);
}

void setup() {
  Serial.begin(115200);

  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.on("/events", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/event-stream", "data: " + readTemp() + "\n\n");
  });

  server.on("/readTemp", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", readTemp().c_str());
  });

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    String page = "<html><head>";
    page += "<style>body { font-family: Arial, sans-serif; background-color: #f4f4f4; display: flex; align-items: center; justify-content: center; height: 100vh; margin: 0; }</style>";
    page += "<link rel='stylesheet' href='https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.15.4/css/all.min.css' integrity='sha512-Zh2e5lR6b1E7G5kmC5q2/Jz2QHiyyZB49UoMKApslEKnEz1o6eHAh0FvsNpMOG/z6b1d0OE7QxTUQA9QHJ5oQ==' crossorigin='anonymous' />";
    page += "<script>var eventSource = new EventSource('/events'); "
            "eventSource.onmessage = function(event) { document.getElementById('temp').innerHTML = '<i class=\"fas fa-thermometer-half\"></i> Temperature: ' + event.data; };</script>";
    page += "</head><body>";
    page += "<div id='temp' style='font-size: 100px; margin: 20px; text-align: center;'></div>";
    page += "</body></html>";
    request->send(200, "text/html", page);
  });

  server.begin();
}

void loop() {
  // Your loop code goes here
}
