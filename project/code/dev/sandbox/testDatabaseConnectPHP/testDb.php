<?php

ini_set('display_errors', 1);
error_reporting(E_ALL);

mysql_connect("localhost", "littlechap22", "some_pass") or die(mysql_error());

mysql_select_db("testPHP") or die (mysql_error());

$data = mysql_query("select * from testTable");

while($info = mysql_fetch_array($data)) {
	print $info['id']. " " . $info['name'];
}
?>
