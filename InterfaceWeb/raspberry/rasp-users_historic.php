<?php

include ("../includes/mysql.php");
$db = enableConnection();

$_GET['Type_al'] = str_replace("/","",$_GET['Type_al']);

$query = $db->prepare("INSERT INTO users_historic VALUES (:id,:act,:dose,:ded,:nb,:type,NOW())");
$query->execute(array("id" => $_GET['Id_personne'],"act" => $_GET['Id_activity'],"dose" => $_GET['Dose'],"ded" => $_GET['Debit_max'],"nb" => $_GET['Nb_al'],"type" => $_GET['Type_al']));
$count = $query->rowCount();
if($count)
{
	$userid = $query->fetch();
	echo "&".$userid['ID_Personne'];
}
else echo "NOTUSER";
echo " AUTH/";
?>