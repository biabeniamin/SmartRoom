#!/bin/bash
count=0
while [ 1 -eq 1 ]
do
	device=$(ls /sys/bus/w1/devices/ | grep 28)
	temperatureRaw=$(cat /sys/bus/w1/devices/$device/w1_slave | grep 't' | awk '{print $10}')
	temperature=${temperatureRaw:2:5}
	temperaturePrecise=$((temperature%1000))
	temperature=$((temperature/1000))
	echo "$temperature.$temperaturePrecise" > /var/www/html/roomStatus/temperature
	if ((  $count > 4 ))
	then
		curl -s -o /dev/null -X GET "http://iot.robofun.ro/api/v1/senzor/jf69kq5fjqo500mklevcahc9u8//input?value=$temperature.$temperaturePrecise"
		count=0
	fi
	count=$((count+1))
	sleep 5
done
