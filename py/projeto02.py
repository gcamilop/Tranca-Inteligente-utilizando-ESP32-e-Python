import cv2
import face_recognition as fr
import os
import cvzone
#import numpy as np

nomes=[]
encods=[]
lista = os.listdir('Pessoas')

#carregar base de dados de pessoas autorizadas
for arquivo in lista:
    img = cv2.imread(f'Pessoas/{arquivo}')
    img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    encods.append(fr.face_encodings(img)[0])
    nomes.append(os.path.splitext(arquivo)[0])

def compararEnc(encImg):
    for id,enc in enumerate(encods):
        comp = fr.compare_faces([encImg],enc)
        if comp[0]:
            break
    return comp[0], nomes[id]

print('Base carregada!')

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
            cvzone.putTextRect(img,f'Autorizado: {nome}',(50,50),colorR=(0,255,0))
        else:
            cvzone.putTextRect(img,'Nao Autorizado',(50,50),colorR=(0,0,255))


    cv2.imshow('IMG',img)
    cv2.waitKey(1)

