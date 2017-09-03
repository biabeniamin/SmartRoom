#!/bin/bash
while [ 1 -eq 1 ]
do
	device=$(ls /sys/bus/w1/devices/ | grep 28)
	temperatureRaw=$(cat /sys/bus/w1/devices/$device/w1_slave | grep 't' | awk '{print $10}')
	temperature=${temperatureRaw:2:5}
	temperaturePrecise=$((temperature%1000))
	temperature=$((temperature/1000))
	echo "$temperature.$temperaturePrecise" > /var/www/html/roomStatus/temperature
	sleep 5
done
