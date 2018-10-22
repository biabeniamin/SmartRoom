import time
import roomLan
import datetime

lastTimeLightAlert=time.time() - 100 * 60
lastTimeSmsLightAlert=time.time()
def alert(priority):
	global lastTimeLightAlert
	global lastTimeSmsLightAlert
	alertWithLight = 0
	lightInterval = 2;
	color = 0
	now = datetime.datetime.now()

	if (4 == priority):
		alertWithLight = 1
		color = 7
	if (3 == priority):
		if(3*60 < time.time()-lastTimeSmsLightAlert and 22 > now.hour):
			alertWithLight = 1
			lightInterval = 1;
			lastTimeSmsLightAlert = time.time()
		color = 4
	if (2 == priority):
		if(7 < now.hour):
			if((5*60 < time.time()-lastTimeLightAlert and 22 > now.hour)) or ((30*60 < time.time()-lastTimeLightAlert)):
				alertWithLight = 1
				lightInterval = 1;
		color = 2
	if (1 == priority):
		color = 1

	if (1 ==alertWithLight):
		lastTimeLightAlert=time.time()
		roomLan.alertLight(lightInterval)
	print("color = ",color)
	roomLan.alertPcCase(1, color)

