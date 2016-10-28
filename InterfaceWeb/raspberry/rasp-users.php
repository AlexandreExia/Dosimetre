<?php

include ("../includes/mysql.php");
$db = enableConnection();

$_GET['badge'] = str_replace("/","",$_GET['badge']);

$query = $db->prepare("SELECT ID_Personne FROM users WHERE ID_Badge = :id");
$query->execute(array("id" => $_GET['badge']));
$count = $query->rowCount();
if($count)
{
	$userid = $query->fetch();
	echo "&".$userid['ID_Personne'];
}
else echo "NOTUSER";
echo " AUTH/";
?>