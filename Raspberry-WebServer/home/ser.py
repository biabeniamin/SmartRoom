import serial
import sys
import os
if 1:#len(sys.argv)>4:
	seri = serial.Serial("/dev/ttyUSB0")    #Open named port 
	os.system('gpio mode 8 out');
	os.system('gpio write 8 1');
	seri.baudrate = 9600 
	seri.write(str(9))
	seri.write(str(5))
	seri.write(str(6))
	seri.write(str(8))
	seri.write(sys.argv[1])
	seri.write(sys.argv[2])
	seri.write(sys.argv[3])
	seri.write(sys.argv[4])
	os.system('gpio write 8 0');
	seri.close()     
