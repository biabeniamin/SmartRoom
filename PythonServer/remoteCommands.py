from enum import Enum

class REMOTE_COMMANDS(Enum):
	MEDIA_PLAY = 0
	MEDIA_PAUSE = 1
	MEDIA_PLAYPAUSE = 2
	MEDIA_NEXT = 3
	MEDIA_PREVIOUS = 4
	MEDIA_VOLUMEUP = 5
	MEDIA_VOLUMEDOWN = 6
	MEDIA_VOLUMEMUTE = 7
	SPACEBAR = 8
	LOCK = 9
	STOP_MUSIC = 10
	SWITCH_AC = 11
	ALERT = 12
	TURN_ON_AC = 13
	TURN_OFF_AC = 15
	FAST_MODE_AC = 16
	HEAT_AC = 17
