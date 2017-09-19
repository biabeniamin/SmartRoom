<?php
	session_start();
	if(isset($_GET['cmd']))
	{
		if($_GET['cmd'] == "open")
		{
			$_SESSION['dell']="jesus";
		}
	}
?>
