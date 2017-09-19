<html>
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
?>
<html>
	 <head>
		<meta http-equiv="refresh" content="5;url=tabletModeRefreshed.php">
		<style type="text/css">
			body
			{
				font-size:30px;
			}
			.main
			{
				text-align: center;
				width:100px;
				margin:0 auto;
				font-size:40px;

			}
	
		 </style>
	 </head>
	 <body>
		<?php

			$temperature=readAllFile("roomStatus/temperature");
			echo "Temperature: ".$temperature."<br>";


			$doorStatus=readAllFile("roomStatus/door");
			if($doorStatus == 0)
			{
				echo "<img style='height:40px;' src=off.png>";
				echo "Door is unlocked<br>";
			}
			else if($doorStatus== 1)
			{
				echo "<img style='height:40px;' src=on.png>";
				echo "Door is locked<br>";

			}
		?>

	</div>
	</body>
</html>
