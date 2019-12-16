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

	if(isset($_GET["temperature"]))
	{
		writeFile("roomStatus/temperature2", $_GET["temperature"]);
	}
	if(isset($_GET["humidity"]))
	{
		writeFile("roomStatus/humidity", $_GET["humidity"]);
	}

?>
