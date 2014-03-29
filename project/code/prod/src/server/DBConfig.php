<?php

class DBConfig {

	function IsConnected() {

		$server = "localhost";
		$username = "littlechap22";
		$password = "some_pass";
		$dbName = "projectm";

		$conn = mysql_connect($server, $username, $password);
		if(!$conn) {
			return false;
			//log
			//die("Could not connect: " . mysql_error());
		}

		$isDbSelected = mysql_select_db($dbName, $conn);
		if(!$isDbSelected) {
			//log
			die("Cannot use " . $dbName . ": " . mysql_error());
		}
		return true;
	}
}
?>
