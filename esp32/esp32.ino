

#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>


const char* ssid = "TONILMA_2.4";
const char* password = "205639gc";

IPAddress local_IP(192, 168, 1, 120);  // IP Fixo 
IPAddress gateway(192, 168, 1, 1);     // IP do roteador MUDAR NA DOCUMENTAÇÃO
IPAddress subnet(255, 255, 255, 0);   
IPAddress primaryDNS(8, 8, 8, 8);      
IPAddress secondaryDNS(8, 8, 4, 4);    

const int servoPin = 25; // pino de sinal do servo

WebServer server(80);
Servo myServo;

void handleRoot() {
  String html = "<!doctype html><html><head><meta charset='utf-8'><title>ESP32 Servo</title></head><body>";
  html += "<h1>ESP32 Servo Control</h1>";
  html += "<form action='/servo' method='get'>Ângulo (0-180): <input name='angle' value='90'/> <input type='submit' value='Mover'/></form>";
  html += "<p>Exemplo de requisição: /servo?angle=45</p>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleServo() {
  if (!server.hasArg("angle")) {
    server.send(400, "application/json", "{\"error\":\"missing angle\"}");
    return;
  }
  int angle = server.arg("angle").toInt();
  angle = constrain(angle, 0, 180);
  myServo.write(angle);
  String payload = "{\"angle\":" + String(angle) + "}";
  server.send(200, "application/json", payload);
  delay(10000);
}

void handleNotFound(){
  server.send(404, "text/plain", "Not found");
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Configura servo: período padrão 50Hz (servo hobby)
  myServo.setPeriodHertz(50);
  // attach(pin, minPulse, maxPulse) em microsegundos; ajuste se seu servo precisar
  myServo.attach(servoPin, 500, 2400);
  myServo.write(90); // posição inicial (opcional)

  // Conexão WiFi
  WiFi.begin(ssid, password);
  Serial.print("Conectando ao WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Conectado! IP: ");
  Serial.println(WiFi.localIP());

  // Rotas HTTP
  server.on("/", handleRoot);
  server.on("/servo", handleServo);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("Servidor HTTP iniciado na porta 80");
}

void loop() {
  server.handleClient();
}