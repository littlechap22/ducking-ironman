<?php

require_once "DBConfig.php";
require_once "MiscConfig.php";

class Utility {

	function TestFunction() {
		return "you reached server\n";
	}

	function GetUserLatLonData() {
		$dbConn = new DBConfig();
		if($dbConn->IsConnected()) {
		
			$query = "CALL GetUserLatLonData()";
			$result = mysql_query($query);

			$output = array();
			while($r = mysql_fetch_assoc($result)) {
				$output[] = $r;
			}
			print json_encode($output);
		}
	}
}

?>
