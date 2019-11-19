<html>
<script src="javascriptFunctions.js"></script>
<script>
	function performAction(url)
	{
		httpRequestSync(url);
	}
</script>
<?php
	require 'header.php';
	function readAllFile($filePath)
	{
		$f=fopen($filePath,"r");
		$content=fread($f,filesize($filePath));
		fclose($f);

		return $content;
	}
	function writeFile($filePath,$content)
	{
		$f=fopen($filePath,"w");
		fwrite($f,$content);
		fclose($f);
	}
ini_set('display_errors', 1);
if(isset($_GET["cmd"]))
{
	if($_GET["cmd"]=="lightOn")
	{
		echo shell_exec("/home/pi/sendCommand.sh 1");
	}
	else if($_GET["cmd"]=="lightOff")
	{
		echo shell_exec("/home/pi/sendCommand.sh 0");
	}
	else if($_GET["cmd"]=="lightSwitch")
	{
		echo shell_exec("/home/pi/sendCommand.sh 2");
	}
	else if($_GET["cmd"]=="benchLightSwitch")
	{
		echo shell_exec("/home/pi/sendCommand.sh 15");
	}
	else if($_GET["cmd"]=="audioOff")
	{
		echo shell_exec("/home/pi/sendCommand.sh 16");
	}
	else if($_GET["cmd"]=="audioOn")
	{
		echo shell_exec("/home/pi/sendCommand.sh 17");
	}
	else if($_GET["cmd"]=="audioSwitch")
	{
		echo shell_exec("/home/pi/sendCommand.sh 18");
	}
	else if($_GET["cmd"]=="acSwitch")
	{
		echo shell_exec("/home/pi/sendCommand.sh 14");
	}
	else if($_GET["cmd"]=="lightHallOn")
	{
		echo shell_exec("/home/pi/sendCommand.sh 10");
	}
	else if($_GET["cmd"]=="lightHallOff")
	{
		echo shell_exec("/home/pi/sendCommand.sh 9");
	}
	else if($_GET["cmd"]=="heaterOn")
	{
		echo shell_exec("/home/pi/sendCommand.sh 13");
	}
	else if($_GET["cmd"]=="heaterOff")
	{
		echo shell_exec("/home/pi/sendCommand.sh 12");
	}
	else if($_GET["cmd"]=="lightHallSwitch")
	{
		echo shell_exec("/home/pi/sendCommand.sh 11");
	}
	if($_GET["cmd"]=="openCurtains")
	{
		writeFile("notif", 6);
		echo shell_exec("/home/pi/sendCommand.sh 3");
	}
	if($_GET["cmd"]=="closeCurtains")
	{	
		writeFile("notif", 0);
		echo shell_exec("/home/pi/sendCommand.sh 4");
	}
	if($_GET["cmd"]=="stopCurtains")
	{
		echo shell_exec("/home/pi/sendCommand.sh 5");
	}
	if($_GET["cmd"]=="openDoor")
	{
		//$content = readAllFile("logRoom");
		//echo $content;
		//$content = $content . "[00/00/00 00:00:00] 3 0 ".$_SERVER['REMOTE_ADDR']."<br>";
		//writeFile("logRoom", $content);
		echo shell_exec("/home/pi/sendCommand.sh 7");
		writeFile("roomStatus/door",0);
	}
	if($_GET["cmd"]=="closeDoor")
	{
		echo shell_exec("/home/pi/sendCommand.sh 6");
		writeFile("roomStatus/door",1);
	}
	if($_GET["cmd"]=="switchDoor")
	{
		echo shell_exec("/home/pi/sendCommand.sh 8");
	}
	if($_GET["cmd"]=="plus")
	{
		$currentThreshold = readAllFile("roomStatus/thresholdTemperature");
		writeFile("roomStatus/thresholdTemperature",$currentThreshold + 1);
	}
	if($_GET["cmd"]=="minus")
	{
		$currentThreshold = readAllFile("roomStatus/thresholdTemperature");
		writeFile("roomStatus/thresholdTemperature",$currentThreshold - 1);
	}
	if($_GET["cmd"]=="clearLog")
	{
		$fileName="logRoom";
		$file=fopen($fileName,"w");
		fwrite($file,"");
		fclose($file);
	}
}
?>
<html>
	 <head>
		<style type="text/css">
			body
			{
				background-color: #011F33;
			}
			.main
			{
				background-color: #1B6A9E;
				text-align: center;
				width: 800px;
				margin:0 auto;
				font-size:20px;

			}
.button {
    background-color: #BD7B01;
    border: none;
    color: white;
    padding: 15px 15px;
    text-align: center;
    text-decoration: none;
    display: inline-block;
    font-size: 15px;
    margin: 4px 2px;
    cursor: pointer;
}	
		 </style>
	 </head>
	 <body>
	<div class="main">
		<h1>Smart room</h1>
		<a href="?cmd=lightOn"><Button class="button">Turn on light</Button></a>
		<a href="?cmd=lightOff"><Button class="button">Turn off light</Button></a>
		<button onclick="performAction('?cmd=lightSwitch')" class="button">Switch light</Button><br>
		<!- hall->
		<a href="?cmd=lightHallOn"><Button class="button">Turn on hall light</Button></a>
		<a href="?cmd=lightHallOff"><Button class="button">Turn off hall light</Button></a>
		<a href="?cmd=lightHallSwitch"><Button class="button">Switch hall light</Button></a><br>
		<!- workbench light  ->
		<button onclick="performAction('?cmd=benchLightSwitch')" class="button">Switch workbench light</Button><br>
		<!- audio  ->
		<button onclick="performAction('?cmd=audioOn')" class="button">Turn on audio</Button>
		<button onclick="performAction('?cmd=audioOff')" class="button">Turn off audio</Button>
		<button onclick="performAction('?cmd=audioSwitch')" class="button">Switch audio</Button>
		
<br>
		<!- audio volume ->
		<button onclick="performAction('commands.php?cmd=volumeUp')" class="button">Volume up</Button>
		<button onclick="performAction('commands.php?cmd=volumeDown')" class="button">Volume down</Button>
		<button onclick="performAction('commands.php?cmd=volumeMute')" class="button">Volume Mute</Button>
<br>

		<!- media control ->
		<button onclick="performAction('commands.php?cmd=playPause')" class="button">Play/Pause</Button>
		<button onclick="performAction('commands.php?cmd=previous')" class="button">Previous</Button>
		<button onclick="performAction('commands.php?cmd=next')" class="button">Next</Button>
<br>
		<!- heater->
		<a href="?cmd=heaterOn"><Button class="button">Turn on heater</Button></a>
		<a href="?cmd=heaterOff"><Button class="button">Turn off heater</Button></a><br>
		<a href="?cmd=openCurtains"><Button class="button">Open Curtains</Button></a>
		<a href="?cmd=closeCurtains"><Button class="button">Close Curtains</Button></a>
		<a href="?cmd=stopCurtains"><Button class="button">Stop Curtains</Button></a><br>
		<a href="?cmd=openDoor"><Button class="button">Open Door</Button></a>
		<a href="?cmd=closeDoor"><Button class="button">Close Door</Button></a>
		<a href="?cmd=switchDoor"><Button class="button">Switch Door</Button></a>
<br>

		<button onclick="performAction('commands.php?cmd=switchAc')" class="button">Switch ac</Button>
		<button onclick="performAction('commands.php?cmd=turnOnAc')" class="button">Turn on ac</Button>
		<button onclick="performAction('commands.php?cmd=turnOffAc')" class="button">Turn off ac</Button>
		<button onclick="performAction('commands.php?cmd=fastModeAc')" class="button">Fast Mode ac</Button>
		<button onclick="performAction('commands.php?cmd=heatAc')" class="button">Heat ac</Button>
		<button onclick="performAction('commands.php?cmd=alert&value=2')" class="button">Alert</Button>
<br>

		<a href="?cmd=clearLog"><Button class="button">Clear Log</Button></a>
		<a href="index.php"><Button class="button">Refresh</Button></a><br>
		<a href="?cmd=plus"><Button class="button">+</Button></a>
		<a href="?cmd=minus"><Button class="button">-</Button></a><br>
		
		<?php

			$thresholdTemperature=readAllFile("roomStatus/thresholdTemperature");
			echo "Threshold Temperature: ".$thresholdTemperature."<br>";

			$temperature=readAllFile("roomStatus/temperature");
			$temperature2=readAllFile("roomStatus/temperature2");
			$humidity=readAllFile("roomStatus/humidity");
			echo "Temperature: ".$temperature."<br>";
			echo "Temperature2: ".$temperature2."<br>";
			echo "Humidity: ".$humidity."<br>";

			$doorStatus=readAllFile("roomStatus/door");
			if($doorStatus == 0)
			{
				echo "<img style='height:30px;' src=off.png>";
				echo "Door is unlocked<br>";
			}
			else if($doorStatus== 1)

			{
				echo "<img style='height:30px;' src=on.png>";
				echo "Door is locked<br>";

			} 

			$lightStatus=readAllFile("roomStatus/light");
			if($lightStatus == 4)
			{
				echo "<img style='height:30px;' src=off.png>";
				echo "Light is turned off<br>";
			}
			else if($lightStatus== 3)

			{
				echo "<img style='height:30px;' src=on.png>";
				echo "Light is turned on<br>";

			} 
			
			$heaterStatus=readAllFile("roomStatus/heater");
			if($heaterStatus == 4)
			{
				echo "<img style='height:30px;' src=off.png>";
				echo "Heater is turned off<br>";
			}
			else if($heaterStatus== 3)

			{
				echo "<img style='height:30px;' src=on.png>";
				echo "Heater is turned on<br>";

			}
 
			$motion=readAllFile("roomStatus/motion");
			if($motion == 1)
			{
				echo "<img style='height:30px;' src=on.png>";
				echo "Motion detected<br>";
			}
			else if($motion == 0)
			{
				echo "<img style='height:30px;' src=off.png>";
				echo " No motion detected<br>";
			}


			$workbenchLight=readAllFile("roomStatus/workbenchLight");
			if($workbenchLight == 1)
			{
				echo "<img style='height:30px;' src=on.png>";
				echo "Workbench light is on<br>";
			}
			else if($workbenchLight == 0)
			{
				echo "<img style='height:30px;' src=off.png>";
				echo " Workbench light is off<br>";
			}
			$fileName="logRoom";
			$file=fopen($fileName,"r");
			if(filesize($fileName)>0)
			{
				$content=fread($file,filesize($fileName));
				$lines=split("<br>",$content);
				foreach($lines as $line)
				{
					echo $line."<br>";
/*	
					$elem=split(" ",$line);

					if(sizeof($elem)<5)
						continue;

					//var_dump($elem);
					//echo "<br><br>";
					echo $elem[0]." ".$elem[1]." ";
					$author=shell_exec("./getDeviceName.sh $elem[4]");
					echo shell_exec("./getCommandText.sh $elem[2] $elem[3] $elem[4]")." by $author".$elem[4]."<br>";
*/
				}
				//echo fread($file,filesize($fileName));
				fclose($file);
			}
		?>

	</div>
	</body>
</html>
