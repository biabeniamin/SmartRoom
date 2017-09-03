#! /bin/bash
inotifywait -m /var/www/html/motion -e create |
    while read path action file; do
	echo "1">/var/www/html/hasNotification
        # do something with the file
    done
