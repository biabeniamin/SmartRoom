import serial
from datetime import datetime
from collections import deque
import tcpSocketClient
from remoteCommands import REMOTE_COMMANDS
from serialCommunication import sendCommand, sendNormalCommand



def turnOnLight():
	request = deque('2410')
	print(request)
	sendCommand(request)
def turnOffLight():
	request = deque('2400')
	print(request)
	sendCommand(request)
def turnOnWorkbenchLight():
	request = [0,4,1,0,0]
	sendNormalCommand(serial,request)
def turnOffWorkbenchLight():
	request = [0,4,0,0,0]
	sendNormalCommand(request)

def switchAc():
	request = [8,4,2,0,0]
	print("Switch ac")
	sendNormalCommand(request)

def alertLight(seconds):
	#send to giroscop
	request = [8,4,1,seconds,0]
	sendNormalCommand(request)

def alertPcCase(seconds, color):
	#send to computer
	tcpSocketClient.sendTcpSocketMessageOnNewThread([REMOTE_COMMANDS.ALERT.value, seconds, color])

def setNixieClockTime():
	now=datetime.now()
	print(now.hour)
	request = [9,0,now.hour,now.minute, now.second]
	sendNormalCommand(request)

def volumeUp(times):
	tcpSocketClient.sendTcpSocketMessageOnNewThread([REMOTE_COMMANDS.MEDIA_VOLUMEUP.value, times])

def volumeDown(times):
	tcpSocketClient.sendTcpSocketMessageOnNewThread([REMOTE_COMMANDS.MEDIA_VOLUMEDOWN.value, times])

def playPause():
	tcpSocketClient.sendTcpSocketMessageOnNewThread([REMOTE_COMMANDS.MEDIA_PLAYPAUSE.value, 0])

def spacebar():
	tcpSocketClient.sendTcpSocketMessageOnNewThread([REMOTE_COMMANDS.SPACEBAR.value, 0])

def next():
	tcpSocketClient.sendTcpSocketMessageOnNewThread([REMOTE_COMMANDS.MEDIA_NEXT.value, 0])

def previous():
	tcpSocketClient.sendTcpSocketMessageOnNewThread([REMOTE_COMMANDS.MEDIA_PREVIOUS.value, 0])

def stopMusic():
	tcpSocketClient.sendTcpSocketMessageOnNewThread([REMOTE_COMMANDS.STOP_MUSIC.value, 0])

def mute():
	tcpSocketClient.sendTcpSocketMessageOnNewThread([REMOTE_COMMANDS.MEDIA_VOLUMEMUTE.value, 0])

def lock():
	tcpSocketClient.sendTcpSocketMessageOnNewThread([REMOTE_COMMANDS.LOCK.value, 0])
