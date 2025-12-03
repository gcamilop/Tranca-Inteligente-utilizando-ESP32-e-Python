# Tranca-Inteligente-utilizando-ESP32-e-Python
# 🏠🔒 Tranca Inteligente com ESP32 e Reconhecimento Facial (Python + ESP32)

Este projeto implementa um sistema de controle de acesso inteligente, combinando reconhecimento facial em Python com o acionamento de uma tranca eletrônica controlada por ESP32.
O objetivo é criar uma solução de baixo custo, rápida e confiável para ambientes residenciais, comerciais e acadêmicos.

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

O ESP32 aciona a fechadura eletrônica e registra o evento.

## 🛠️ Possíveis Expansões

Integração via MQTT

Dashboard web para gerenciamento

Banco de dados para histórico de acessos

Criptografia na comunicação

Reconhecimento aprimorado com modelos DNN ou Mediapipe

📷 Objetivo

Fornecer uma solução prática e acessível de automação de acesso, unindo visão computacional com IoT.
