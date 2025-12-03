
#bibliotecas para rodas o RF
import cv2
import face_recognition as fr
import sys
import os
import cvzone
import time
#import numpy as np

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

#print('Base carregada!')

#comparar encodificação
def compararEnc(encImg):
    for id,enc in enumerate(encods):
        comp = fr.compare_faces([encImg],enc)
        if comp[0]:
            break
    return comp[0], nomes[id]

def send_servo_command(angle, host="192.168.1.10", port=80, timeout=10, path="/servo"):
    angle = int(max(0, min(180, angle)))
    url = f"http://{host}:{port}{path}"
    try:
        resp = requests.get(url, params={"angle": str(angle)}, timeout=timeout)
        resp.raise_for_status()
        return True, resp.text
    except requests.RequestException as e:
        return False, str(e)

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
        #print(encodeImg)
        comp,nome = compararEnc(encodeImg)

        if comp:
            cvzone.putTextRect(img,f'Autorizado: {nome}',(50,50),colorR=(0,255,0),font=1)
            #abrindo a porta
             success, msg = send_servo_command(135, host="192.168.1.10")
             if success:
                 print("Porta Aberta, Comando Enviado OK:", msg)
             else:
                 print("Erro ao enviar comando:", msg)

        else:
            cvzone.putTextRect(img,'Nao Autorizado',(50,50),colorR=(0,0,255))
        #fechando a porta
        success, msg = send_servo_command(45, host="192.168.1.10")
        print("Porta Fechada, Comando Enviado OK:", msg)


    cv2.imshow('IMG',img)
    cv2.waitKey(1)

