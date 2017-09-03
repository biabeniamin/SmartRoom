\<html>
<?php
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
	if($_GET["cmd"]=="lightOff")
	{
		echo shell_exec("/home/pi/sendCommand.sh 0");
	}
	if($_GET["cmd"]=="lightSwitch")
	{
		echo shell_exec("/home/pi/sendCommand.sh 2");
	}
	if($_GET["cmd"]=="openCurtains")
	{
		echo shell_exec("/home/pi/sendCommand.sh 3");
	}
	if($_GET["cmd"]=="closeCurtains")
	{
		echo shell_exec("/home/pi/sendCommand.sh 4");
	}
	if($_GET["cmd"]=="stopCurtains")
	{
		echo shell_exec("/home/pi/sendCommand.sh 5");
	}
	if($_GET["cmd"]=="openDoor")
	{
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
		<a href="?cmd=lightSwitch"><Button class="button">Switch light</Button></a><br>
		<a href="?cmd=openCurtains"><Button class="button">Open Curtains</Button></a>
		<a href="?cmd=closeCurtains"><Button class="button">Close Curtains</Button></a>
		<a href="?cmd=stopCurtains"><Button class="button">Stop Curtains</Button></a><br>
		<a href="?cmd=openDoor"><Button class="button">Open Door</Button></a>
		<a href="?cmd=closeDoor"><Button class="button">Close Door</Button></a>
		<a href="?cmd=switchDoor"><Button class="button">Switch Door</Button></a><br>
		<a href="?cmd=clearLog"><Button class="button">Clear Log</Button></a>
		<a href="index.php"><Button class="button">Refresh</Button></a><br>
		
		<?php

			$temperature=readAllFile("roomStatus/temperature");
			echo "Temperature: ".$temperature."<br>";


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
			$fileName="logRoom";
			$file=fopen($fileName,"r");
			if(filesize($fileName)>0)
			{
				$content=fread($file,filesize($fileName));
				$lines=split("<br>",$content);
				foreach($lines as $line)
				{	
					$elem=split(" ",$line);

					if(sizeof($elem)<5)
						continue;

					//var_dump($elem);
					//echo "<br><br>";
					echo $elem[0]." ".$elem[1]." ";
					$author=shell_exec("./getDeviceName.sh $elem[4]");
					echo shell_exec("./getCommandText.sh $elem[2] $elem[3] $elem[4]")." by $author".$elem[4]."<br>";
				}
				//echo fread($file,filesize($fileName));
				fclose($file);
			}
		?>

	</div>
	</body>
</html>
