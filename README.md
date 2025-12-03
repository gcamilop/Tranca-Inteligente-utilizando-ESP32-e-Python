# Tranca-Inteligente-utilizando-ESP32-e-Python
# 🔒 Tranca Inteligente com ESP32 e Reconhecimento Facial (Python + ESP32)

Este projeto implementa um sistema de controle de acesso inteligente, combinando reconhecimento facial em Python com o acionamento de uma tranca eletrônica controlada por ESP32.
O objetivo é criar uma solução de baixo custo, rápida e confiável para ambientes de construção civil.

![Codigo Python com reconhecimento facial](https://github.com/user-attachments/assets/584642d8-1945-46c6-955d-8dcbf3a9cc34)

## 🚀 Funcionalidades

🔍 Reconhecimento facial em tempo real usando OpenCV

🔗 Comunicação Python ↔ ESP32 via Wi-Fi (HTTP com validação por login e senha)

🔒 Acionamento de fechadura elétrica utilizando GPIO do ESP32

📡 Envio e recebimento de comandos pela rede

🧪 Logs e feedback instantâneo no console

## 🧠 Tecnologias Utilizadas

### ⚙️ Para desenvolver o reconhecimento facial:

Python 3.10.0

pip 25.3

cmake 4.1.2

face-recognition 1.3.0

OpenCV 4.12.0.88

cvzone 1.6.1

requests 2.32.5

argparse 1.4.0

### ⚙️ Para desenvolver o sistema do ESP32:

C++

Bibliotecas: WIFI.h,    WebServer.h,       Esp32Servo.h

HTML

Arduino IDE

### ⚙️ Hardware necessario para o protótipo:

ESP32

Servo motor SG90 (em caso de uso real pode ser substituido por uma tranca solenóide, porém o projeto abaixo não contém essa adaptação ainda)

Fonte 5V / 1 A

Cabos Jumper

Protoboard

![WhatsApp Image 2025-11-25 at 06 33 58](https://github.com/user-attachments/assets/5d117990-4232-49ed-9f04-d1d4b36b949a)

## 📌 Arquitetura do Sistema

O Python captura, processa imagens, identifica rostos autorizados e ao reconhecer uma pessoa, envia um comando pela rede ao ESP32. Para que isso aconteça se faz necessario que algumas etapas listadas abaixo:

Carregamento da base de dados de pessoas autorizadas (Necessario criar uma pasta no mesmo diretorio que o programa com nome 'Pessoas' onde ficaram as pessoas autorizadas com o acesso)

```python
for arquivo in lista:
    img = cv2.imread(f'Pessoas/{arquivo}')
    img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    encods.append(fr.face_encodings(img)[0])
    nomes.append(os.path.splitext(arquivo)[0])
    
 
if nomes != []:
    print('Base carregada!')
else:
    sys.exit('Base de Dados Vazia')
```

Comparar encodificação

```python
def compararEnc(encImg):
    for id,enc in enumerate(encods):
        comp = fr.compare_faces([encImg],enc)
        if comp[0]:
            break
    return comp[0], nomes[id]

```

Enviar comandos ao servo

```python
def send_servo_command(angle, host="192.168.1.10", port=80, timeout=10, path="/servo"):
    angle = int(max(0, min(180, angle)))
    url = f"http://{host}:{port}{path}"
    try:
        resp = requests.get(url, params={"angle": str(angle)}, timeout=timeout)
        resp.raise_for_status()
        return True, resp.text
    except requests.RequestException as e:
        return False, str(e)
```

O ESP32 aciona a fechadura eletrônica.

Pagina web login

```C++
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

```

Pagina web controle servo

```C++
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

```

Pagina web login

```C++
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

```

Pagina web login

```C++
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

```

Pagina web login

```C++
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

```


## 🛠️ Possíveis Expansões

Dashboard web para gerenciamento

Banco de dados para histórico de acessos

Criptografia na comunicação

Reconhecimento aprimorado com modelos DNN ou Mediapipe (utilizado para face liviness)

Reconhecimento de EPI

Troca do servo motor por um tranca elétrica solenóide
