#!/bin/bash 
if [[ $#>0 ]]
then
	if [[ "$1" -eq "0" ]]
	then
		#switch on
		python /home/pi/ser.py 2 4 0 0
	elif [[ "$1" -eq "1" ]]
	then
		#switch off
		python /home/pi/ser.py 2 4 1 0
	elif [[ "$1" -eq "2" ]]
	then
		#switch light
		python /home/pi/ser.py 2 4 2 0
	elif [[ "$1" -eq "3" ]]
	then
		#open curtains
		python /home/pi/ser.py 1 1 0 0
	elif [[ "$1" -eq "4" ]]
	then
		#close curtains
		python /home/pi/ser.py 1 0 0 0
	elif [[ "$1" -eq "5" ]]
	then
		#stop curtains
		python /home/pi/ser.py 1 2 0 0
	elif [[ "$1" -eq "6" ]]
	then
		#close door
		python /home/pi/ser.py 3 4 0 0
	elif [[ "$1" -eq "7" ]]
	then
		#open door
		python /home/pi/ser.py 3 4 1 0
	elif [[ "$1" -eq "8" ]]
	then
		#switch door
		python /home/pi/ser.py 3 4 2 0
	elif [[ "$1" -eq "9" ]]
	then
		#switch hall on
		python /home/pi/ser.py 5 4 0 0
	elif [[ "$1" -eq "10" ]]
	then
		#switch hall off
		python /home/pi/ser.py 5 4 1 0
	elif [[ "$1" -eq "11" ]]
	then
		#switch hall light
		python /home/pi/ser.py 5 4 2 0
	fi
else
	echo "[command] argument"
fi

