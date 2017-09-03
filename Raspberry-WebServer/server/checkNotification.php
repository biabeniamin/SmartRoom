<?php
$path="hasNotification";
$myfile = fopen($path, "r") or die("Unable to open file!");
echo fread($myfile,filesize($path));
fclose($myfile);
//write 0
$myfile = fopen($path, "w") or die("Unable to open file!");
fwrite($myfile, "0");
fclose($myfile);
?>

