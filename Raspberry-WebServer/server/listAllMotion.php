
<?php
	require 'header.php';
 function customError($errno, $errstr)
        {
                echo "<b>Error:</b> [$errno] $errstr";
        }
        //set error handler
        set_error_handler("customError");

echo "test";
$dir="motion";
$lis=["0"];
$count=0;
if (is_dir($dir)) {
    if ($dh = opendir($dir)) {
        while (($file = readdir($dh)) !== false) {
		//echo $file."<br>";
		$list[$count]=$file;
		$count++;
        }
	array_multisort($list);
	for($i=0;$i<$count;$i++)
	{
	        echo "<img src='motion/".$list[$i]."'/>";
		//echo $list[$i]."<br>";
	}
        closedir($dh);
    }
}
else
{
	echo "folder doesn't exists!";
}
/*
if (0==1)//$handle = opendir('/motion')) {
{
    echo "Directory handle: $handle\n";
    echo "Entries:\n";

    while (false !== ($entry = readdir($handle))) {
        echo "<img src='".$entry."'/>";
	break;
    }

    closedir($handle);
}*/
?>


