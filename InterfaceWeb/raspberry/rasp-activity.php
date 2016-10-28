<?php

include ("../includes/mysql.php");
$db = enableConnection();
$_GET['iz'] = str_replace("/","",$_GET['iz']);
$query = $db->prepare("SELECT aDED, aDose, aCoeff, aID FROM activity_config WHERE aIZ = :iz");
$query->execute(array("iz" => $_GET['iz']));
$count = $query->rowCount();
if($count)
{
	$result = $query->fetch();
	echo '&'.$result['aID'].' '.$result['aDED'].' '.$result['aDose'].' '.$result['aCoeff'].' ACT/';
}
else echo "NOTUSER";
?>