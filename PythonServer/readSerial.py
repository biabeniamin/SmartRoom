import serialCommunication
import tempMonitor
import serial
import sys
import RPi.GPIO as GPIO
import os
from thread import start_new_thread, allocate_lock
import time
import tcpSocketClient
import tcpSocketServer
import roomLan
from datetime import datetime
from collections import deque
from remoteCommands import REMOTE_COMMANDS
from enum import Enum
import alerts

lastTimeLock = allocate_lock()
lastTimeDoorClosed = 0
lastTimeDoorOpened = 0

serial = serialCommunication.serial

def getDeviceName(id):
	if 4 == id:
		return "web server"
	elif 0 == id:
		return "multimedia controller"

	return "unknown"

def writeLogAscii(request):
	global lastTimeDoorClosed
	global lastTimeDoorOpened
	now=datetime.now()
	f = open('/var/www/html/logRoom','a')
	print("logging")
	f.write(now.strftime("[%y/%m/%d %H:%M:%S] "))
	if request[5] == '3':
		if '0' == request[6]:
			f.write("The door was unlocked")
		elif '1' == request[6]:
			f.write("The door was locked")
		elif '3' == request[6]:
			f.write("The door was opened")
			print("door opened")

			lastTimeLock.acquire()
			lastTimeDoorOpened = time.time()
			lastTimeLock.release()
		elif '4' == request[6]:
			f.write("The door was closed")
			print("door closed")

			lastTimeLock.acquire()
			lastTimeDoorClosed = time.time()
			lastTimeLock.release()
	elif request[5] == '2':
		if '3' == request[6]:
			f.write("The light was turned on")
		elif '4' == request[6]:
			f.write("The light was turned off")
		elif '5' == request[6]:
			f.write("The light was switched")
	elif request[5] == '0':
		if '1' == request[6]:
			f.write("The workbench light was turned on")
		elif '0' == request[6]:
			f.write("The workbench light was turned off")
	elif request[5] == '6':
		heaterState=request[6]
		f = open('/var/www/html/roomStatus/heater','w')	
		f.write(heaterState)
		f.close()
	#f.seek(0,2)
	f.write(" by " + getDeviceName(ord(request[7])) - 48)
	f.write("<br>\n")
	f.close()

def writeLog(request):
	now=datetime.now()
	f = open('/var/www/html/logRoom','a')
	print("logging")
	f.write(now.strftime("[%y/%m/%d %H:%M:%S] "))
	if ord(request[5]) == 8:
		if 0== ord(request[6]):
			f.write("The ac was switched")
		if 51 == ord(request[6]):
			f.write("The alert failed")
	#f.seek(0,2)
	f.write(" by " + getDeviceName(ord(request[7])))
	print(ord(request[7]))
	print(chr(ord(request[7])))
	f.write("<br>\n")
	f.close()

def analyze(request):
	if request[5] == '3':
		doorState=request[6]
		f = open('/var/www/html/roomStatus/door','w')	
		f.write(doorState)
		f.close()
		if '3' == request[7]:
			if '0' == doorState:
				os.system("service motion stop")
			else:
				os.system("service motion start")
	elif request[5] == '2':
		lightState=request[6]
		f = open('/var/www/html/roomStatus/light','w')	
		f.write(lightState)
		f.close()
	elif request[5] == '0':
		lightState=request[6]
		f = open('/var/www/html/roomStatus/workbenchLight','w')	
		f.write(lightState)
		f.close()
	elif request[5] == '6':
		heaterState=request[6]
		f = open('/var/www/html/roomStatus/heater','w')	
		f.write(heaterState)
		f.close()
class DOOR_STATE(Enum):
	UNKNOWN = 0
	OPEN = 1
	CLOSE = 2
doorState = DOOR_STATE.UNKNOWN.value
def analyzeCommand(request):
	print("ok")
def analyzeAsciiCommand(request):
	if(10 == ord(request[6])):
		print("set time")
		roomLan.setNixieClockTime()
	else:
		remoteCommand(ord(request[6]), [1])

def readSerial():
	global serial
	print("init serial reader") 
	request=deque()
	while 1:
		request.append(serial.read(1))
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
					writeLogAscii(request)
					print(request)
					analyze(request)	
				elif '7' == request[4]:
					analyzeCommand(request)
			elif (ord(request[0]) == 9
			and ord(request[1]) == 5
			and ord(request[2]) == 6
			and ord(request[3]) == 7
			):
				if ord(request[4]) == 4:
					writeLog(request)
					print(request)
				print(ord(request[0]),ord(request[1]),ord(request[2]),ord(request[3]))
				print(ord(request[4]),ord(request[5]),ord(request[6]),ord(request[7]))
				print("not ascii")
				analyzeAsciiCommand(request)
			request.popleft()
			
def getLightStatus():
	status=0
	f = open('/var/www/html/roomStatus/light','r')	
	status=int(f.read(1))
	f.close()
	
	return status
def getWorkbenchLightStatus():
	status=0
	f = open('/var/www/html/roomStatus/workbenchLight','r')	
	status=int(f.read(1))
	f.close()
	
	return status

def remoteCommand(command, arguments):
	if command == REMOTE_COMMANDS.MEDIA_PLAYPAUSE.value:
		roomLan.playPause()
	elif command == REMOTE_COMMANDS.MEDIA_VOLUMEUP.value:
		roomLan.volumeUp(arguments[0])
	elif command == REMOTE_COMMANDS.MEDIA_VOLUMEDOWN.value:
		roomLan.volumeDown(arguments[0])
	elif command == REMOTE_COMMANDS.MEDIA_VOLUMEMUTE.value:
		roomLan.mute()
	elif command == REMOTE_COMMANDS.LOCK.value:
		roomLan.lock()
	elif command == REMOTE_COMMANDS.MEDIA_PLAYPAUSE.value:
		roomLan.playPause()
	elif command == REMOTE_COMMANDS.SPACEBAR.value:
		roomLan.spacebar()
	elif command == REMOTE_COMMANDS.MEDIA_NEXT.value:
		roomLan.next()
	elif command == REMOTE_COMMANDS.MEDIA_PREVIOUS.value:
		roomLan.previous()
	elif command == REMOTE_COMMANDS.STOP_MUSIC.value:
		roomLan.stopMusic()
	elif command == REMOTE_COMMANDS.SWITCH_AC.value:
		roomLan.switchAc()
	elif command == REMOTE_COMMANDS.ALERT.value:
		alerts.alert(arguments[0])
	elif command == REMOTE_COMMANDS.TURN_ON_AC.value:
		roomLan.turnOnAc()
	elif command == REMOTE_COMMANDS.TURN_OFF_AC.value:
		roomLan.turnOffAc()
	elif command == REMOTE_COMMANDS.FAST_MODE_AC.value:
		roomLan.fastModeAc()
	elif command == REMOTE_COMMANDS.HEAT_AC.value:
		print("activate heat")
		roomLan.heatAc()

def newTcpSocketClient(message, arguments):
	print(message, arguments)
	remoteCommand(message, arguments)

#start thread for reading serial
start_new_thread(readSerial,())

tcpSocketServer.startTcpSocketServerOnNewThread(newTcpSocketClient)

start_new_thread(tempMonitor.readTemperature,())

#set pin for motion sensor
GPIO.setmode(GPIO.BCM)
GPIO.setup(26, GPIO.IN, pull_up_down=GPIO.PUD_UP)

lastMotion=GPIO.input(26)
lightBeforeLeaving=4
workbenchLightBeforeLeaving=0
lastTimePersonCame=0
lastTimeMotionDetected=time.time()

#roomLan.setNixieClockTime()

while 1:
	wasMotionDetected=GPIO.input(26)
	#print(wasMotionDetected)
	#print(lastTimeDoorClosed)
	#print(time.time() - lastTimeDoorClosed)
	if 60 * 10 < time.time() - lastTimeMotionDetected:
		lightBeforeLeaving=getLightStatus()
		workbenchLightBeforeLeaving=getWorkbenchLightStatus()
		if 3 == lightBeforeLeaving:
			print("turn off light because of time out")
			roomLan.turnOffLight()
		if 1 == workbenchLightBeforeLeaving:
			print("turn off workbench light because of time out")
			roomLan.turnOffWorkbenchLight()
		lastTimeMotionDetected = time.time()
	if 0 != lastTimeDoorClosed:
		if(5 > time.time() - lastTimePersonCame):
			#when person came, trigger actions but continously motion is detected and was motion detected doesn't change
			lastTimeDoorClosed = 0
		else:
			if 5 < (time.time() - lastTimeDoorClosed):
				#i left the room
				print("person left the room")
				roomLan.stopMusic()
				lightBeforeLeaving=getLightStatus()
				workbenchLightBeforeLeaving=getWorkbenchLightStatus()
				print(lightBeforeLeaving)
				if 3 == lightBeforeLeaving:
					roomLan.turnOffLight()
				if 1 == workbenchLightBeforeLeaving:
					roomLan.turnOffWorkbenchLight()
				lastTimeDoorClosed = 0

	if(wasMotionDetected!= lastMotion):
		print("diff")
		f = open('/var/www/html/roomStatus/motion','w')	
		f.write(str(wasMotionDetected))
		f.close()
		if 1 == wasMotionDetected:
			lastTimeMotionDetected=time.time()
			if 0 != lastTimeDoorOpened:
				if 10 > (time.time() - lastTimeDoorOpened):
					#i came into room
					lastTimePersonCame=time.time()
					print("person came in room")
					roomLan.playPause()
					print(lightBeforeLeaving)
					if 3 == lightBeforeLeaving:
						roomLan.turnOnLight()
					#if 1 == workbenchLightBeforeLeaving:
						#roomLan.turnOnWorkbenchLight()
					lastTimeDoorOpened = 0
			lastTimeDoorClosed = 0
		

		lastMotion = wasMotionDetected

	time.sleep(1)
