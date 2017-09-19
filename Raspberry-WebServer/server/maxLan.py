import RPi.GPIO as GPIO   # Import the GPIO library.
from time import sleep    # Import the sleep function from the time library.
GPIO.setmode(GPIO.BCM)
maxModePin=26
GPIO.setup(maxModePin, GPIO.OUT)   # Set GPIO pin 3 to output mode.
GPIO.output(maxModePin, True)  # Turn pin 3 on (LED on).
GPIO.cleanup()
