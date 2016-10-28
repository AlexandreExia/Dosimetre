<?php

include ("../includes/mysql.php");

$db = enableConnection();



// chargement de toute le table batterie dans la page batterie.php


 if(isset($_POST["type"]) && $_POST["type"] == "batterie"){
	$rep = $db->query('SELECT * FROM batterie');
	$count = $rep->rowCount();
	if($count){
		$var = "	<h3 align='center'>Table Batterie</h3>
					<table class='bordered'>
						<thread>
							<tr>
								<th data-field='ID_DOSI'>ID Dosi</th>
								<th data-field='NBR_CYCLE'>Cycle</th>
								<th data-field='DATE_CONNECTION'>Date de connection</th>
								<th data-field='DATE_DECONNEC'>Date de deconnection</th>
								<th data-field='TEST_DECHARGE'>Test de décharge</th>
								<th data-field='TEST_CHARGE'>Test de charge</th>
								<th data-field='CHARGE_RESTANTE'>Charge restante</th>
								<th data-field='FLAG'>FLAG</th>
								<th data-field='SONDE'>SONDE</th>
							</tr>
						</thread>
						<tbody>";
		while($result = $rep->fetch(PDO::FETCH_ASSOC)){
			$var .= "<tr>";
			$var .= "<td>".$result['ID_DOSI']."</td>";
			$var .= "<td>".$result['NBR_CYCLE']."</td>";
			$var .= "<td>".$result['DATE_CONNECTION']."</td>";
			$var .= "<td>".$result['DATE_DECONNEC']."</td>";
			$var .= "<td>".$result['TEST_DECHARGE']."</td>";
			$var .= "<td>".$result['TEST_CHARGE']."</td>";
			$var .= "<td>".$result['CHARGE_RESTANTE']."</td>";
			$var .= "<td>".$result['FLAG']."</td>";
			$var .= "<td>".$result['SONDE']."</td>";
			$var .= "</tr>";
		}
		$var .= "	</tbody>
				</table>";
		echo $var;
	}
}
?>
