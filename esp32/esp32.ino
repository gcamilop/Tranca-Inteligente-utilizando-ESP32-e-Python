#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

const char* ssid = "Sua Rede";
const char* password = "Sua Senha";

const int servoPin = 25;
WebServer server(80);

Servo myServo;

IPAddress staticIP(192, 168, 1, 120);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(192, 168, 1, 1);
IPAddress secondaryDNS(0, 0, 0, 0);

// ------- LOGIN E VALIDAÇÃO -------
bool autenticado = false;
String USER = "marte"; 
String PASS = "plutao";  

String loginPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta charset='UTF-8'>
<title>Login ESP32</title>
<style>
body { font-family: Arial; text-align:center; margin-top:50px; }
input { padding:10px; margin:5px; }
button { padding:10px 20px; }
</style>
</head>
<body>
<h2>Login Necessário</h2>
<form action="/login" method="POST">
  <input type="text" name="user" placeholder="Usuário"><br>
  <input type="password" name="pass" placeholder="Senha"><br>
  <button type="submit">Entrar</button>
</form>
</body>
</html>
)rawliteral";


String controlPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta charset='UTF-8'>
<title>Controle do Servo ESP32</title>
<style>
body {
  font-family: Arial, sans-serif;
  text-align: center;
  margin-top: 40px;
}
.container {
  width: 280px;
  margin: auto;
  padding: 20px;
  border-radius: 10px;
  background: #f5f5f5;
  box-shadow: 0 0 10px rgba(0,0,0,0.2);
}
h2 {
  margin-bottom: 20px;
}
input[type=number] {
  width: 80px;
  padding: 10px;
  font-size: 18px;
  margin-top: 10px;
}
button {
  padding: 10px 20px;
  margin-top: 20px;
  font-size: 16px;
  cursor: pointer;
}
#status {
  margin-top: 20px;
  font-weight: bold;
}
</style>
</head>
<body>

<div class='container'>
  <h2>Controle do Servo</h2>

  <p>Digite o ângulo (0–180):</p>

  <!-- CAMPO COM LIMITES HTML -->
  <input type='number' id='angleInput' value='90' min='0' max='180' required>

  <br><br>
  <button onclick='sendAngle()'>Mover Servo</button>

  <div id='status'></div>
</div>

<script>
function sendAngle() {
  const angle = parseInt(document.getElementById("angleInput").value);

  // VALIDAÇÃO JAVASCRIPT
  if (isNaN(angle) || angle < 0 || angle > 180) {
    document.getElementById("status").innerHTML =
      "Valor inválido! Digite entre 0 e 180 graus.";
    return;
  }

  // ENVIO AO ESP32
  fetch(`/servo?angle=${angle}`)
    .then(response => response.json())
    .then(data => {
      document.getElementById("status").innerHTML =
        "Servo movido para " + data.angle + "°";
    })
    .catch(error => {
      document.getElementById("status").innerHTML =
        "Erro ao comunicar com o ESP32.";
    });
}
</script>

</body>
</html>
)rawliteral";


void checkSession() {
  if (!autenticado) {
    server.sendHeader("Location", "/login");
    server.send(302, "text/plain", "");
  }
}

// ------- PÁGINA PRINCIPAL (apenas após login) -------
void handleRoot() {
  if (!autenticado) {
    server.send(200, "text/html", loginPage);
    return;
  }

  server.send(200, "text/html", controlPage);
}

// ------- PROCESSA LOGIN -------
void handleLogin() {
  String user = server.arg("user");
  String pass = server.arg("pass");

  if (user == USER && pass == PASS) {
    autenticado = true;
    server.send(200, "text/html", controlPage);
  } else {
    autenticado = false;
    server.send(403, "text/html", "<h1>Login inválido!</h1><a href='/'>Tentar novamente</a>");
  }
}

// ------- CONTROLE DO SERVO (rota protegida) -------
void handleServo() {
  if (!autenticado) {
    server.send(200, "text/html", loginPage);
    return;
  }

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

void handleNotFound() {
  server.send(404, "text/plain", "Not found");
}

void setup() {
  Serial.begin(115200);

  myServo.setPeriodHertz(50);
  myServo.attach(servoPin, 500, 2400);
  myServo.write(90);

  WiFi.begin(ssid, password);
  Serial.println("\n\n\\");
  Serial.print("Conectando ao WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/login", HTTP_POST, handleLogin);
  server.on("/servo", handleServo);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("Servidor HTTP iniciado.");
}

void loop() {
  server.handleClient();
}
