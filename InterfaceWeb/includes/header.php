<?php

function setHeader()
{
	echo '<header><nav>';
	echo '<div class="blue-grey nav-wrapper">';
    echo '<a href="#" class="brand-logo">Interface de gestion</a>';
	echo '
	<ul id="nav-mobile" class="right hide-on-med-and-down">
        <li><a href="./index.php">Utilisateurs</a></li>
        <li><a href="./activity.php">Activitées</a></li>
        <li><a href="./batterie.php">Batteries</a></li>
    </ul>';
    echo '</div>';
	echo '</nav></header>';
}

?>