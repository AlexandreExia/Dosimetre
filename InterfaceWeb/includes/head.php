<?php 
function setHead($title="Gestion BdD", $encoding='"UTF-8"', $filename="users")
{
	echo '<head>';
	echo '<title>'.$title.'</title>';
	echo '<meta charset='.$encoding.'/>';
    echo '<link href="http://fonts.googleapis.com/icon?family=Material+Icons" rel="stylesheet">';
	echo '<link type="text/css" rel="stylesheet" href="assets/css/materialize.min.css"  media="screen,projection"/>';
    echo '<meta name="viewport" content="width=device-width, initial-scale=1.0"/>';
	echo '<script type="text/javascript" src="assets/js/jquery-2.1.1.min.js"></script>';
    echo '<script type="text/javascript" src="assets/js/materialize.min.js"></script>';
	echo '<script type="text/javascript" src="assets/js/Chart.min.js"></script>';
	echo '<script type="text/javascript" src="assets/js/'.$filename.'.js"></script>';
	echo '<link type="text/css" rel="stylesheet" href="assets/css/style.css"/>';
	echo '</head>';
}
?>