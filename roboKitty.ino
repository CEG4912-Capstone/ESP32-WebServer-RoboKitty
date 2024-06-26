#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

// add your network settings
const char* ssid = "WIFI_NAME";
const char* password = "WIFI_PASSWORD";

WebServer server(80);

const int ledPin26 = 26;
const int ledPin27 = 27;


// handle preflight request options
void handleOptions() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
  server.send(204); // No Content
}

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

  // get json response
  DynamicJsonDocument doc(1024 * 4); 
  DeserializationError error = deserializeJson(doc, message);

  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    server.send(400, "text/plain", "Invalid JSON");
    return;
  }

  //list of commands
  JsonArray commands = doc.as<JsonArray>();

  for (JsonObject command : commands) {
    // important command data
    int steps = command["steps"];
    const char* movementType = command["movementType"]; // 'Move' | 'Turn'
    const char* direction = command["direction"]; // 'Forward' | 'Backward' | 'Right' | 'Left'

    // gotta process command after command to preserve the sequence
    if(movementType == "Move"){
      if(direction == "Forward"){

        // robot.forward(steps)
        Serial.println("Forward: "+steps);

      } else if(direction == "Backward"){

        // robot.backward(steps)
        Serial.println("Backward: "+steps);

      }
  
    } else if(movementType == "Turn"){
      if(direction == "Right"){

        // robot.right(steps)
        Serial.println("Turn right: "+steps);

      } else if(direction == "Left"){

        // robot.left(steps)
        Serial.println("Turn left: "+steps);
        
      }
    }
  
  server.send(200, "application/json", "{\"message\":\"request received\"}");
  }
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
  server.on("/post", HTTP_OPTIONS, handleOptions); // Handle preflight requests

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
