<?php

if(isset($_GET['com']))
{
	$file=fopen("defenderActivatorStatus","w") or die("cannot write");
	fwrite($file,$_GET['com']);
	fclose($file);
}
	$file=fopen("defenderActivatorStatus","r") or die("cannot read");
	echo fread($file,filesize("defenderActivatorStatus"));
	fclose($file);
?>

