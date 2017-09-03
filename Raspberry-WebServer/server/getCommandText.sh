#!/bin/sh
if [ "$#" -gt  1 ]
then
	if [ "$1" -eq "3" ]
	then
		echo -n "Door"
		if [ "$2" -eq "0" ]
		then
			echo " was unlocked"
		elif [ "$2" -eq "1" ]
		then
			echo " was locked"
		elif [ "$2" -eq "2" ]
		then
			echo " invalid pin"
		fi
	fi
fi
