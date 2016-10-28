<?php

include ("../includes/mysql.php");

$db = enableConnection();

// chargement de toute le table Users dans la page users.php
if(isset($_POST["type"]) && $_POST["type"] == "users")
{
	$rep = $db->query('SELECT * FROM users ORDER BY ID_Personne');
	$count = $rep->rowCount();
	
	$var="
	<a class='waves-effect waves-light btn' id='modal_boutton_form'>Ajouter</a>
	<a class='waves-effect waves-light btn' id='modal_boutton_suppr_all'>Supprimer tout</a>
	<h5>Liste des utilisateurs</h5>
	<table class='bordered'>
		<thead>
			<tr>
				<th data-field='N° utilisateur'>N° utilisateur</th>
				<th data-field='N° badge'>N° badge</th>
				<th data-field='Nom'>Nom</th>
				<th data-field='Prenom'>Prenom</th>
				<th data-field='Historique'>Historique</th>
				<th data-field='Suppression'>Suppression</th>
			</tr>
		</thead>
	<tbody>	";	
	if($count)
	{	
			while($result = $rep->fetch(PDO::FETCH_ASSOC))
			{
				$var .= "<tr>";
				$var .= "<td>".$result['ID_Personne']."</td>";
				$var .= "<td>".$result['ID_Badge']."</td>";
				$var .= "<td>".$result['Nom']."</td>";
				$var .= "<td>".$result['Prenom']."</td>";
				$var .= "<td><a href='#' id='view_historic' userid='".$result['ID_Personne']."'>Voir</a></td>";
				$var .= "<td><a href='#' id='delete_user' userid='".$result['ID_Personne']."' >Supprimer</a></td>";
				$var .= "</tr>";
			}
	}
	else $var .= "<tr><td>Aucun utilisateur</td></tr>";
	$var .= "</tbody></table>";
	echo $var;
}


// chargement des modals dans la page users.php

if(isset($_POST["type"]) && $_POST["type"] == "modal_users")
{
	$var = "
	<div id='modal_Ajout_form' class='modal'>
		<div class='modal-content'>
			<div class='row'>
				<form id='formajoutusers' class='col s12' method='POST'>
					<div class='row'>
						<div class='input-field col s12'>
							<input id='id_badge' name='Users_2' type='text' class='validate'>
							<label for=''>Identifiant du badge</label>
						</div>
						<div class='input-field col s12'>
							<input id='id_nom' name='Users_3' type='text' class='validate'>
							<label for=''>Nom de la personne</label>
						</div>
						<div class='input-field col s12'>
							<input id='id_prenom' name='Users_4' type='text' class='validate'>
							<label for=''>Prenom de la personne</label>
						</div>
					</div>
					<button id='ajout_users' class='btn'>Ajouter</button>
				</form>
			</div>
		</div>
	</div>
	<div id='modal_suppr_all' class='modal'>
		<div class='modal-content'>
			<div class='row'>
				<form id='formajoutusers' class='col s12' method='POST' >
					<div class='row'>
						<div class='input-field col s12'>
							<h3>Etes vous sûr de tout supprimer ?</h3>
						</div>
					</div>
					<button id='delete_all' name='sup_all_users' class='btn'>Tout supprimer</button>
				</form>
			</div>
		</div>
	</div>";
	echo $var;
}

// Suppression/Création d'un utilisateur

if(isset($_POST["type"]) && $_POST["type"] == "add_users")
{
	$req = $db->prepare("INSERT INTO users (ID_Badge, Prenom, Nom, Date_Creation) VALUES (:idbadge, :prenom, :nom, NOW())");
	$req->execute(array("idbadge" => $_POST['badge'], "prenom" => $_POST['prenom'], "nom" => $_POST['nom']));
}

if(isset($_POST["type"]) && $_POST["type"] == "delete_all")
{
	$req = $db->query("DELETE FROM users");
	$query = $db->query("DELETE FROM activity_config");
}

if(isset($_POST["type"]) && $_POST["type"] == "delete_user")
{
	$req = $db->prepare("DELETE FROM users WHERE ID_Personne = :id");
	$req->execute(array("id" => $_POST['id']));
	$query = $db->prepare("DELETE FROM activity_config WHERE aUserID=:id");
	$query->execute(array("id" => $_POST["id"]));
}

// chargement de l'historique de l'utilisateur

if(isset($_POST["type"]) && $_POST["type"] == "view_historic")
{
	$rep = $db->prepare('SELECT * FROM users_historic WHERE ID_Personne = :id ORDER BY Date_Creation');
	$rep->execute(array("id" => $_POST['id']));
	
	$count = $rep->rowCount();
		$var = "	<h5>Historique de l'utilisateur</h5>
					<table class='bordered'>
						<thread>
							<tr>
								<th data-field='ID_Personne'>Id Personne</th>
								<th data-field='ID_Activity'>Id Activiter</th>
								<th data-field='Dose'>Dose</th>
								<th data-field='Ded_max'>Débit maximum</th>
								<th data-field='Nombre_Alarme'>Nombre d'Alarme</th>
								<th data-field='Type_Alarme'>Alarme Dose</th>
								<th data-field='Date_Modification'>Date de Modification</th></tr>
						</thread>
					<tbody>	";
	if($count)
	{
		while($result = $rep->fetch(PDO::FETCH_ASSOC)){
			$var .= "<tr>";
			$var .= "<td>".$result['ID_Personne']."</td>";
			$var .= "<td>".$result['ID_Activity']."</td>";
			$var .= "<td>".$result['Dose']."</td>";
			$var .= "<td>".$result['Ded_max']."</td>";
			$var .= "<td>".$result['Nombre_Alarme']."</td>";
			$var .= "<td>".$result['Type_Alarme']."</td>";
			$var .= "<td>".$result['Date_Creation']."</td>";
			$var .= "</tr>";
		}
	}
	else $var .= "<tr><td>Aucun historique pour cet utilisateur</td></tr>";
	$var .= "</tbody></table>";
	echo $var;
}


?>
