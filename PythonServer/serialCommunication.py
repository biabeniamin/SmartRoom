import serial
import RPi.GPIO as GPIO
import os
import glob

GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)
pin = 17
GPIO.setup(pin, GPIO.OUT)

ports = glob.glob('/dev/ttyUSB*')
if(len(ports) < 1):
	print("No usb serial port detected")
serial = serial.Serial(ports[0])    #Open named port 
serial.baudrate = 9600


def sendCommand(request):

	GPIO.output(pin, GPIO.HIGH)
	serial.write(str(9))
	serial.write(str(5))
	serial.write(str(6))
	serial.write(str(8))
	serial.write(request[0])
	serial.write(request[1])
	serial.write(request[2])
	serial.write(request[3])

	os.system('gpio write 0 0');
	GPIO.output(pin, GPIO.LOW)
def sendNormalCommand(request):
	print(request)
	GPIO.output(pin, GPIO.HIGH)
	serial.write(unichr(9))
	serial.write(unichr(5))
	serial.write(unichr(6))
	serial.write(unichr(7))
	serial.write(unichr(request[0]))
	serial.write(unichr(request[1]))
	serial.write(unichr(request[2]))
	serial.write(unichr(request[3]))
	serial.write(unichr(request[4]))

	os.system('gpio write 0 0');
	GPIO.output(pin, GPIO.LOW)
