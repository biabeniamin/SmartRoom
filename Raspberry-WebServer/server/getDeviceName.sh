#!/bin/bash
if (( "$#" > "0" ))
then
	if (( "$1" == "0" ))
	then
		echo "multimedia controller"
	elif (( "$1" == "4" ))
	then
		echo "web server"
	fi
fi
