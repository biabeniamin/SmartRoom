echo "initializing smart room"


chmod a=rwx /dev/ttyUSB0

/home/pi/readTemperature.sh & 
nohup python /home/pi/readSerial.py &


gpio mode 8 out
gpio write 8 0

