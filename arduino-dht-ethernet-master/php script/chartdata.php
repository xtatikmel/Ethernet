<?php
	$connect = mysql_connect("db.mysite.com","myusername","mypassword","mydb") or die('Could not connect: ' . mysql_error());
	mysql_select_db("mydb", $connect);

	
	$query = "SELECT * FROM arduino_logger ORDER BY timestamp LIMIT 100";
	$result = mysql_query($query, $connect) or die("SQL Error 1: " . mysql_error());

	// get data and store in a json array
	while ($row = mysql_fetch_array($result)) {
		$readings[] = array(
			'temperature' => $row['temperature'],
			'humidity' => $row['humidity'],
			'timestamp' => $row['timestamp']
		  );
	}
  
	echo json_encode($readings);
	mysql_close($connection);
?>