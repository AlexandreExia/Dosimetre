<?php

function enableConnection($host="localhost", $dbname="dosibtssncouf", $username="dosibtssncouf", $password="PkFqfefJWK")
{
	try {
		$dbh = new PDO('mysql:host='.$host.';dbname='.$dbname.'', $username, $password);
		return $dbh;
	} catch (PDOException $e) {
		print "Erreur !: " . $e->getMessage() . "<br/>";
		die();
	}
}

function disableConnection(&$db)
{
	$db = null;
}

?>