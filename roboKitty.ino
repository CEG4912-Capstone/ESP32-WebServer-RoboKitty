#include <WiFi.h>
#include <WebServer.h>

// add your network settings
const char* ssid = "WIFI_NAME";
const char* password = "WIFI_PASSWORD";

WebServer server(80);

const int ledPin26 = 26;
const int ledPin27 = 27;

void handlePost() {
  //allow origin
  server.sendHeader("Access-Control-Allow-Origin", "*");

  if (server.hasArg("plain") == false) { // check if body received
    server.send(200, "application/json", "{\"error\":\"Body not received\"}");
    return;
  }
  
  String message = server.arg("plain"); // get the body
  Serial.print("POST Body: ");
  Serial.println(message);
  
  server.send(200, "application/json", "{\"message\":\"request received\"}");
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin26, OUTPUT);
  pinMode(ledPin27, OUTPUT);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", getHtml());
  });

  server.on("/control", HTTP_GET, []() {
    if (server.hasArg("led") && server.hasArg("state")) {
      int ledPin = server.arg("led") == "26" ? ledPin26 : ledPin27;
      digitalWrite(ledPin, server.arg("state") == "on" ? HIGH : LOW);
    }
    server.sendHeader("Location", "/", true); // redirect back
    server.send(303);
  });

  // request from website
  server.on("/post", HTTP_POST, handlePost);

  // start
  server.begin();
}

void loop() {
  server.handleClient();
}

String getHtml() {
  //default web page
  String html = R"=====(
    <!DOCTYPE html>
    <html>
    <head>
      <title>ESP32 LED Control</title>
    </head>
    <body>
      <h2>ESP32 LED Control</h2>
      <p>
        <a href="/control?led=26&state=on"><button>LED 26 ON</button></a>
        <a href="/control?led=26&state=off"><button>LED 26 OFF</button></a>
      </p>
      <p>
        <a href="/control?led=27&state=on"><button>LED 27 ON</button></a>
        <a href="/control?led=27&state=off"><button>LED 27 OFF</button></a>
      </p>
    </body>
    </html>
  )=====";
  return html;
}
