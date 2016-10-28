<?php

include ("../includes/mysql.php");
$db = enableConnection();

$query = $db->prepare("SELECT ID_Personne FROM users WHERE ID_Badge = :id");
$query->execute(array("id" => $_GET['userid']));
$count = $query->rowCount();
if($count)
{
	$result = $query->fetch();
	$query = $db->prepare("SELECT a FROM users WHERE ID_Badge = :id");
	$query->execute(array("id" => $_GET['userid']));

	echo "USERID ".$result['ID_Personne'];
}
?>