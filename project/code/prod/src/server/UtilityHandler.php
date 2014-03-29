<?php

require_once "Utility.php";

$myUtility = new Utility();
$result = $myUtility->GetUserLatLonData();

echo $result;

?>
