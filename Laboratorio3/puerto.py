import csv
import serial
#Se utlizo la siguiente fuente: https://www.pythontutorial.net/python-basics/python-write-csv-file/
serial_comu = serial.Serial("/tmp/ttyS1", 9600) #Comunicacion con la placa mediante el puerto ttyS1

archivo = open("Arduino.csv",'w') 
Write = csv.writer(archivo)

while (1):
    Data = serial_comu.readline().decode().split(' ')
    writer.writerow(Data)

f.close()



