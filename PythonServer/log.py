from datetime import datetime

def sendLog(message):
	f = open('/var/www/html/logRoom','a')
	print("logging")
	now=datetime.now()
	f = open('/var/www/html/logRoom','a')
	f.write(now.strftime("[%y/%m/%d %H:%M:%S]"))
	f.write(str(message) + "<br>")
	f.close()
