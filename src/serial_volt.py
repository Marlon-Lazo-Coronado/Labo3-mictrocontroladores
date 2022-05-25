# Programa para leer el puerto serial
# Marlon Lazo Coronado 
# GOkeh Avila Blanco

# se importa el modulo para escribir un archivo .csv y el de comunicacion serial en python 
import csv
import serial


p_serial = serial.Serial(port = '/dev/ttyS0') 

# para abrir el archivo en modo de escritura
archivo= open("val.csv",'w') 

# se crea la funcion para escribir
escribir = csv.escribir(archivo) 

while(1):
    	val = p_serial.readline().decode().split(' ')
    	escribir.escribirlinea(val) # escribe al archivo csv
	print(val)

# se cierra el archivo
archivo.close()
