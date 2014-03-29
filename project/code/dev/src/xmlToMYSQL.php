<?php

ini_set('display_errors', 1);
error_reporting(E_ALL);


function GetSpeed() {
	return rand(0, 60);
}


mysql_connect("localhost", "littlechap22", "some_pass") or die(mysql_error());
mysql_select_db("projectm") or die (mysql_error());
$data = mysql_query("select * from UserLatLngData");


$data = simplexml_load_file("./data.xml");
if(!$data) {
	trigger_error("Error reading XML file", E_USER_ERROR);
}


foreach($data->segment as $segment) {

	$newLat = $segment["latNew"];
	$newLon = $segment["longNew"];
	$oldLat = $segment["latOld"];
	$oldLon = $segment["longOld"];
	$speed = GetSpeed();

	$query = "INSERT INTO UserLatLngData (oldLat, oldLon, newLat, newLon, speed) 
			VALUES ('" . $oldLat . "', '" . $oldLon . "', '" . $newLat . "', '" . $newLon . "', '" . $speed . "')";
	//print $query . "\n";
	$executeQuery = mysql_query($query);

	
	if(!$executeQuery) {
		echo ("Error: " . mysql_error());
	}
}
?>
