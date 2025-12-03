# Tranca-Inteligente-utilizando-ESP32-e-Python
# 🏠🔒 Tranca Inteligente com ESP32 e Reconhecimento Facial (Python + ESP32)

Este projeto implementa um sistema de controle de acesso inteligente, combinando reconhecimento facial em Python com o acionamento de uma tranca eletrônica controlada por ESP32.
O objetivo é criar uma solução de baixo custo, rápida e confiável para ambientes residenciais, comerciais e acadêmicos.

##🚀 Funcionalidades

🔍 Reconhecimento facial em tempo real usando OpenCV

🔗 Comunicação Python ↔ ESP32 via Wi-Fi (HTTP com validação por login e senha)

🔒 Acionamento de fechadura elétrica utilizando GPIO do ESP32

📡 Envio e recebimento de comandos pela rede

🧪 Logs e feedback instantâneo no console

⚙️ Suporte a mensagens via HTTP ou sockets (dependendo da versão do projeto)

##🧠 Tecnologias Utilizadas

C++

Backend / Reconhecimento

Python

OpenCV

NumPy

LBPH para reconhecimento facial

Bibliotecas de comunicação via rede

Hardware / IoT

ESP32

Arduino IDE / PlatformIO

Wi-Fi integrado

##📌 Arquitetura do Sistema

O Python captura e processa imagens.

```python
print("Olá, mundo!")
```


O sistema identifica rostos autorizados.

Ao reconhecer uma pessoa, envia um comando pela rede ao ESP32.

O ESP32 aciona a fechadura eletrônica e registra o evento.

##🛠️ Possíveis Expansões

Integração via MQTT

Dashboard web para gerenciamento

Banco de dados para histórico de acessos

Criptografia na comunicação

Reconhecimento aprimorado com modelos DNN ou Mediapipe

📷 Objetivo

Fornecer uma solução prática e acessível de automação de acesso, unindo visão computacional com IoT.
