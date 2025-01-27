#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Motor control pins
#define IN1 14 // GPIO 14 for motor control
#define IN2 12 // GPIO 12 for motor control
#define IN3 13 // GPIO 13 for motor control
#define IN4 15 // GPIO 15 for motor control

ESP8266WebServer server(80); // Create a web server on port 80

void setup() {
  // Motor pins setup
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // WiFi setup
  WiFi.softAP("ESP8266_Car", "12345678"); // Start ESP8266 as an Access Point
  IPAddress IP = WiFi.softAPIP();
  Serial.begin(115200);
  Serial.println("Access Point started:");
  Serial.println(IP);

  // Define server routes
  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", R"rawliteral(
      <!DOCTYPE html>
      <html>
      <head>
        <meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no">
        <style>
          body {
            background-color: white;
            text-align: center;
            font-family: Arial, sans-serif;
            margin: 0;
            padding: 0;
          }
          .arrows {
            font-size: 40px;
            color: red;
          }
          td.button {
            background-color: black;
            border-radius: 25%;
            box-shadow: 5px 5px #888888;
            width: 80px;
            height: 80px;
            text-align: center;
            vertical-align: middle;
          }
          td.button:active {
            transform: translate(5px, 5px);
            box-shadow: none;
          }
          .noselect {
            -webkit-touch-callout: none;
            -webkit-user-select: none;
            -khtml-user-select: none;
            -moz-user-select: none;
            -ms-user-select: none;
            user-select: none;
          }
          table {
            width: 300px;
            margin: 50px auto;
            table-layout: fixed;
          }
          td {
            padding: 10px;
          }
          h1 {
            color: teal;
          }
        </style>
      </head>
      <body class="noselect">
        <h1>RC Car Remote</h1>
        <table id="mainTable">
          <tr>
            <td></td>
            <td class="button" ontouchstart='sendCommand("/forward")' ontouchend='sendCommand("/stop")'>
              <span class="arrows">&#8679;</span>
            </td>
            <td></td>
          </tr>
          <tr>
            <td class="button" ontouchstart='sendCommand("/left")' ontouchend='sendCommand("/stop")'>
              <span class="arrows">&#8678;</span>
            </td>
            <td></td>
            <td class="button" ontouchstart='sendCommand("/right")' ontouchend='sendCommand("/stop")'>
              <span class="arrows">&#8680;</span>
            </td>
          </tr>
          <tr>
            <td></td>
            <td class="button" ontouchstart='sendCommand("/backward")' ontouchend='sendCommand("/stop")'>
              <span class="arrows">&#8681;</span>
            </td>
            <td></td>
          </tr>
        </table>
        <script>
          function sendCommand(command) {
            fetch(command).catch((error) => console.log("Error:", error));
          }
        </script>
      </body>
      </html>
    )rawliteral");
  });

  // Movement routes
  server.on("/forward", HTTP_GET, []() { moveForward(); server.send(200, "text/plain", "Moving Forward"); });
  server.on("/backward", HTTP_GET, []() { moveBackward(); server.send(200, "text/plain", "Moving Backward"); });
  server.on("/left", HTTP_GET, []() { turnLeft(); server.send(200, "text/plain", "Turning Left"); });
  server.on("/right", HTTP_GET, []() { turnRight(); server.send(200, "text/plain", "Turning Right"); });
  server.on("/stop", HTTP_GET, []() { stopCar(); server.send(200, "text/plain", "Stopping"); });

  server.begin();
}

void loop() {
  server.handleClient();
}

// Motor control functions
void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void moveBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnLeft() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnRight() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void stopCar() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
