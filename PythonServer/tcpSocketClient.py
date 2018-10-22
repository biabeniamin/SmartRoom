import socket
import sys
from enum import Enum
from thread import start_new_thread, allocate_lock
import log


def sendTcpSocketMessage(package):
	# Create a TCP/IP socket
	sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	sock.settimeout(3)

	# Connect the socket to the port where the server is listening
	server_address = ('192.168.0.100', 10000)
	print >>sys.stderr, 'connecting to %s port %s' % server_address
	try:
		sock.connect(server_address)
		print("connected")
	    
		# Send data
		print >>sys.stderr, 'sending "%s"' % package
		bytes = bytearray()
		bytes.append(chr(package[0]))
		bytes.append(chr(package[1]))
		if(2 < len(package)):
			bytes.append(chr(package[2]))

		sock.send(bytes)

	except Exception as ee:
		log.sendLog(str(ee))	
	finally:
	    print >>sys.stderr, 'closing socket'
	    sock.close()
def sendTcpSocketMessageOnNewThread(message):
	start_new_thread(sendTcpSocketMessage,(message,))
#message = 'This is the message.  It will be repeated.'
#sendTcpSocketMessage(message)
