
#bibliotecas para rodas o RF
import cv2
import face_recognition as fr
import sys
import os
import cvzone
import time

#bibliotecas para enviar dados para o ESP32
import requests
import argparse

nomes=[]
encods=[]
lista = os.listdir('Pessoas')

#carregar base de dados de pessoas autorizadas
for arquivo in lista:
    img = cv2.imread(f'Pessoas/{arquivo}')
    img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    encods.append(fr.face_encodings(img)[0])
    nomes.append(os.path.splitext(arquivo)[0])
    
if nomes != []:
    print('Base carregada!')
else:
    sys.exit('Base de Dados Vazia')

#Faz o login no web server do ESP32
def login_esp32(user, password, host="192.168.1.120", port=80):
    url = f"http://{host}:{port}/login"
    try:
        resp = requests.post(url, data={"user": user, "pass": password}, timeout=5)
        resp.raise_for_status()
        print("Login realizado!")
        return True
    except Exception as e:
        print("Erro no login:", e)
        return False

#Envia o comando para o servo motor do Esp32
def send_servo_command(angle, host="192.168.1.120", port=80):
    angle = int(max(0, min(180, angle)))
    url = f"http://{host}:{port}/servo"

    try:
        resp = requests.get(url, params={"angle": angle}, timeout=5)
        resp.raise_for_status()
        return True, resp.json()
    except Exception as e:
        return False, str(e)

#comparar encodificação
def compararEnc(encImg):
    for id,enc in enumerate(encods):
        comp = fr.compare_faces([encImg],enc)
        if comp[0]:
            break
    return comp[0], nomes[id]

login_esp32("marte", "plutao")
video = cv2.VideoCapture(0)

faceLoc = []

while True:
    check,img = video.read()
    imgRGB = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    try:
        faceLoc.append(fr.face_locations(imgRGB)[0])
    except:
        faceLoc = []

    if faceLoc:
        y1,x2,y2,x1 = faceLoc[-1]
        w,h = x2-x1, y2-y1
        cvzone.cornerRect(img,(x1,y1,w,h),colorR=(255,0,0))
        cvzone.putTextRect(img,'Analisando ...',(50,50),colorR=(255,0,0))

    if len(faceLoc) > 20:
        encodeImg = fr.face_encodings(imgRGB)[0]
        encodeImg = fr.face_encodings(imgRGB)[0]
        comp,nome = compararEnc(encodeImg)

        if comp:
            cvzone.putTextRect(img,f'Autorizado: {nome}',(50,50),colorR=(0,255,0),font=1)
            #abrindo a porta
            success, msg = send_servo_command(135)
            if success:
                print("Porta Aberta, Comando Enviado OK:", msg)
            else:
                print("Erro ao enviar comando:", msg)

        else:
            cvzone.putTextRect(img,'Nao Autorizado',(50,50),colorR=(0,0,255))
        #fechando a porta somente necessario pois no prototipo se utilizou um servo motor
        time.sleep(10)
        success, msg = send_servo_command(45)
        print("Porta Fechada, Comando Enviado OK:", msg)


    cv2.imshow('IMG',img)

    cv2.waitKey(1)
