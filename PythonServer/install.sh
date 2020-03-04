#!/bin/bash
sudo apt-get update
sudo apt-get install python-pip -y
sudo pip install pyserial
sudo pip install RPi.GPIO
sudo apt-get install apache2 -y
sudo apt install php libapache2-mod-php -y
sudo rm /var/www/html/* -r
sudo cp ../Raspberry-WebServer/* /var/www/html/. -r
