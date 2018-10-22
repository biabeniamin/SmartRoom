import socket
import sys
import time
import log
from thread import start_new_thread, allocate_lock
from datetime import datetime


def startTcpSocketServer(callback):
	try:
		# Create a TCP/IP socket
		sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

		# Bind the socket to the port
		server_address = ('0.0.0.0', 10000)
		print >>sys.stderr, 'starting up on %s port %s' % server_address
		log.sendLog("start server")
		sock.bind(server_address)

		# Listen for incoming connections
		sock.listen(1)

		while True:
			# Wait for a connection
			print >>sys.stderr, 'waiting for a connection'
			connection, client_address = sock.accept()
			log.sendLog(str(client_address))
			log.sendLog("start reading data")
			try:
				print >>sys.stderr, 'connection from', client_address

				# Receive the data in small chunks and retransmit it
				data = connection.recv(1024)
				log.sendLog("Data readed" + str(data))
				if data:
					print >>sys.stderr, 'received "%s"' % data
					print len(data)
					log.sendLog(str(len(data)))
					if 1 < len(data):
						callback(ord(data[0]), [ord(data[1]), ord(data[2])])
						log.sendLog("data received")
					print >>sys.stderr, 'sending data back to the client'
					connection.sendall("OK" + data)
					print("data sebt back to tcp client")
				else:
					print >>sys.stderr, 'no more data from', client_address
					log.sendLog("No more data")
					break
		 
				log.sendLog("finished reading data")
			except Exception as ee:
				log.sendLog("except 1:"+str(ee))
			finally:
				# Clean up the connection
				print("Closing connection")
				connection.close()
				print("Connection closed")
	except Exception as e:
		print(e)
		sock.close()
		f = open('/var/www/html/logRoom','a')
		print("logging")
		now=datetime.now()
		f = open('/var/www/html/logRoom','a')
		f.write(now.strftime("[%y/%m/%d %H:%M:%S] "))
		f.write("Server error " + str(e) + "<br>")
		f.close()
		time.sleep(2)
		log.sendLog("restart server")
	return startTcpSocketServer(callback)
	log.sendLog("Server terminated!")

def startTcpSocketServerOnNewThread(callback):
	start_new_thread(startTcpSocketServer,(callback,))
