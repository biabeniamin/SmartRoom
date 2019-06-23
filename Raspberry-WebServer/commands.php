<?php
	abstract class REMOTE_COMMANDS
	{
		const MEDIA_PLAY = 0;
		const MEDIA_PAUSE = 1;
		const MEDIA_PLAYPAUSE = 2;
		const MEDIA_NEXT = 3;
		const MEDIA_PREVIOUS = 4;
		const MEDIA_VOLUMEUP = 5;
		const MEDIA_VOLUMEDOWN = 6;
		const MEDIA_VOLUMEMUTE = 7;
		const SPACEBAR = 8;
		const LOCK = 9;
		const LEAVING = 10;
		const SWITCH_AC = 11;
		const ALERT = 12;
		const TURN_ON_AC=13;
		const TURN_OFF_AC=15;
	}
	function sendCommandTwoParameters($command, $argument1, $argument2)
	{
		$fp = fsockopen("192.168.0.108", 10000, $errno, $errstr, 30);
		if (!$fp)
		{
		    echo "$errstr ($errno)<br />\n";
		} else {
		echo $command;
			$buffer = "asdfg";
			$buffer[0] = chr($command);
			$buffer[1] = chr($argument1);
			$buffer[2] = chr($argument2);
			fputs($fp, $buffer, 3);
		    fclose($fp);
		}
	}
	
	function sendCommand($command, $argument)
	{
		sendCommandTwoParameters($command, $argument, 0);
	}
		
	$argument = 1;


	if(isset($_GET["value"]))
	{
		$argument = $_GET["value"];
	}

	if($_GET["cmd"]=="volumeUp")
	{
		sendCommand(REMOTE_COMMANDS::MEDIA_VOLUMEUP, $argument);
	}
	else if($_GET["cmd"]=="volumeDown")
	{
		sendCommand(REMOTE_COMMANDS::MEDIA_VOLUMEDOWN, $argument);
	}
	else if($_GET["cmd"]=="volumeMute")
	{
		sendCommand(REMOTE_COMMANDS::MEDIA_VOLUMEMUTE, $argument);
	}
	else if($_GET["cmd"]=="playPause")
	{
		sendCommand(REMOTE_COMMANDS::MEDIA_PLAYPAUSE, $argument);
	}
	else if($_GET["cmd"]=="next")
	{
		sendCommand(REMOTE_COMMANDS::MEDIA_NEXT, $argument);
	}
	else if($_GET["cmd"]=="previous")
	{
		sendCommand(REMOTE_COMMANDS::MEDIA_PREVIOUS, $argument);
	}
	else if($_GET["cmd"]=="leaving")
	{
		sendCommand(REMOTE_COMMANDS::LEAVING, $argument);
	}
	else if($_GET["cmd"]=="switchAc")
	{
		sendCommand(REMOTE_COMMANDS::SWITCH_AC, $argument);
	}
	else if($_GET["cmd"]=="alert")
	{
		echo $argument;
		sendCommand(REMOTE_COMMANDS::ALERT, $argument);
	}
	else if($_GET["cmd"]=="turnOnAc")
	{
		sendCommand(REMOTE_COMMANDS::TURN_ON_AC, $argument);
	}
	else if($_GET["cmd"]=="turnOffAc")
	{
		sendCommand(REMOTE_COMMANDS::TURN_OFF_AC, $argument);
	}
?>
