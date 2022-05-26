import serial

# el puerto serial que se esta haciendo con socat
puerto = "/tmp/ttyS1" 
# la velocidad de arduino es de 9600 BAUD
baud = 9600 
# se pone nombre al archivo csv que se va a generar
archivo="datos.csv" 

seri = serial.Serial(puerto, baud)
file = open(archivo, "w") # para escribir en el archivo

muestras = 10000 # cantidad maxima de muestras que se van a recolectar
etiq = False
line = 0 # donde empezamos a imprimir
while line <= muestras:
    if etiq:
        if line==0:
            print("encabezados")
        else:
            print("Linea " + str(line) + ":escribiendo")
    getData=str(seri.readline())
    data=getData[0:][:-2]
    print(data)

    file = open(archivo, "a")
    file.write(data + "\n") 
    line = line+1

file.close()
