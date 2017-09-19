import serial
import sys
import os
from datetime import datetime
from collections import deque
from enum import Enum



def analyze():
	if request[5] == '3':
		doorState=request[6]
		f = open('/var/www/html/roomStatus/door','w')	
		f.write(doorState)
		f.close()
	elif request[5] == '2':
		lightState=request[6]
		f = open('/var/www/html/roomStatus/light','w')	
		f.write(lightState)
		f.close()
class DOOR_STATE(Enum):
	UNKNOWN = 0
	OPEN = 1
	CLOSE = 2
doorState = DOOR_STATE.UNKNOWN.value


print("init serial reader") 
request=deque()
seri = serial.Serial("/dev/ttyUSB0")    #Open named port 
seri.baudrate = 9600
while 1:
	request.append(seri.read(1))
	while len(request) > 8:
		request.popleft()
	if len(request) > 7:
		if (request[0] == '9'
		and request[1] == '5'
		and request[2] == '6'
		and request[3] == '8'
		):
			print(request)
			if request[4] == '4':
				now=datetime.now()
				f = open('/var/www/html/logRoom','a')
				#f.seek(0,2)
				f.write(now.strftime("[%y/%m/%d %H:%M:%S] ")+request[5]+ " " + request[6] + " " + request[7])
				f.write("<br>")
				f.close()
				print(request)
				analyze()	
		
