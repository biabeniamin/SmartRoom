<?php
 function customError($errno, $errstr)
        {
                echo "<b>Error:</b> [$errno] $errstr";
        }
        //set error handler
        set_error_handler("customError");

if ($handle = opendir('motion')) {

    while (false !== ($entry = readdir($handle))) {

        if ($entry != "." && $entry != "..") {

            $path="motion/$entry";
	    echo $path."deleted <br>";
	    unlink($path);
        }
    }

    closedir($handle);
}
?>
