import time

def readTemperature():
	while 1:
		f = open('/sys/bus/w1/devices/28-000008a82661/hwmon/hwmon1/temp1_input','r')	
		temperature = f.read()
		f.close()
		f = open('/var/www/html/roomStatus/temperature','w')	
		f.write(temperature)
		f.close()
		time.sleep(10)

