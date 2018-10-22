import socket
import sys
from enum import Enum
from thread import start_new_thread, allocate_lock
import time


def Test():
	works=0
	# Create a TCP/IP socket
	sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	sock.settimeout(3)

	# Connect the socket to the port where the server is listening
	server_address = ('192.168.0.108', 10000)
	print >>sys.stderr, 'connecting to %s port %s' % server_address
	try:
		sock.connect(server_address)
		print("connected")
	    
		# Send data
		bytes = bytearray()
		bytes.append(5)
		bytes.append(0)
		bytes.append(0)

		sock.send(bytes)
		works=1

	except Exception as ee:
		print(ee)
	finally:
	    print >>sys.stderr, 'closing socket'
	    sock.close()

	return works
def TestTcp(notUsed):
	while 1==1:
		if 1==Test():
			print("Works")
		else:
			print("Doesn't work")
		time.sleep(1)
def sendTcpSocketMessageOnNewThread(message):
	start_new_thread(TestTcp,(1,))
message = 'This is the message.  It will be repeated.'
sendTcpSocketMessageOnNewThread(message)
while 1==1:
	time.sleep(1)
