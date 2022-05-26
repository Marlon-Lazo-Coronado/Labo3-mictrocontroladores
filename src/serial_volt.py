# Código para almacenar datos provenientes de un puerto serial en un
# archivo CSV

import serial


ser = serial.Serial(
	port = "/tmp/ttyS1",\
	baudrate=9600,\
	parity=serial.PARITY_NONE,\
	stopbits=serial.STOPBITS_ONE,\
	bytesize=serial.EIGHTBITS,\
	timeout=0,\
	)
	
f = open("output.csv", "w+")
	
line =[]
	
	
print("conect to port"+ser.portstr)

while True:

	for c in ser.read():
	
		c = chr(c)
		line.append(c)
		
		
		
		print("Line: "+"".join(line))
		str = "".join(line)
		f.write(str)
