<?php

include("../includes/mysql.php");

$db = enableConnection();

function GetUsersList()
{
	$db = enableConnection();
	$query = $db->query("SELECT Prenom, Nom, ID_Personne FROM users");
	$result = $query->fetchAll();
	$db = NULL;
	return $result;
}

if(isset($_POST['type']) && $_POST['type'] == "view_rtr")
{
	$query = $db->prepare("SELECT aIZ FROM activity_config WHERE aID=:aid AND aUserID=:userid");
	$query->execute(array("aid" => $_POST['activityid'], "userid" => $_POST['userid']));
	$result = $query->fetch();
	echo $result['aIZ'];
}

if(isset($_POST['type']) && $_POST['type'] == "add_user")
{
	srand();
	$iz = rand (1000, 5000);
	$iz *= 9999;
	
	$query = $db->prepare("INSERT INTO activity_config (aID,aUserID,aIZ,aDED,aDose,aCoeff) VALUES (:aid, :auserid, :aiz,:aded, :adose, :acoeff)");
	$query->execute(array(
		"aid" => $_POST['activityid'],
		"auserid" => $_POST['userID'],
		"aiz" => $iz,
		"aded" => $_POST['ded'],
		"adose" => $_POST['dose'],
		"acoeff" => $_POST['coeff']
	));
}

if(isset($_POST['type']) && $_POST['type'] == "refresh_users_list")
{
	if(isset($_POST['activityid']) && $_POST['activityid'] > 0)
	{
		$query = $db->prepare("SELECT * FROM activity_config WHERE aID = :id");
		$query->execute(array("id" => $_POST['activityid']));
		$count = $query->rowCount();
		if($count)
		{
			while($result = $query->fetch())
			{
				$query2 = $db->prepare("SELECT * FROM users WHERE ID_Personne = :id");
				$query2->execute(array("id" => $result['aUserID']));
				$user = $query2->fetch();
				echo "<tr>";
				echo "<td>".$user['Nom']." ".$user['Prenom']."</td>";
				echo "<td>".$result['aDED']."</td>";
				echo "<td>".$result['aDose']."</td>";
				echo "<td>".$result['aCoeff']."</td>";
				echo "<td><a href='#' class='view_rtr' userid='".$user['ID_Personne']."' >Voir</a></td>";
				echo "<td><a href='#' class='remove_user_act' userid='".$user['ID_Personne']."' >Retirer</a></td>";
				echo "</tr>";
			}
		}
		else echo "<tr><td>Aucun utilisateurs lié à cette activité</td></tr>";
	}
}

if(isset($_POST["type"]) && $_POST["type"] == "modals")
{
	$var="
	<div id='modal_add_act' class='modal'>
		<div class='modal-content'>
			<div class='row'>
				<div class='input-field col s6'>
					<input id='act_name' type='text' class='validate'>
					<label for='act_name'>Nom de l'activité</label>
				</div>
				<div class='input-field col s6'>
					<input id='act_class' type='text' class='validate'>
					<label for='act_class'>Classe</label>
				</div>
				<div class='input-field col s12'>
					<a id='btn_next_act' class='right waves-effect waves-light btn'>Suivant</a>
				</div>
			</div>
		</div>
	</div>
	<div id='modal_add_users_act' class='modal'>
		<div class='modal-content'>
			<div class='row'>
				<div class='input-field col s12'>
					<div class='input-field col s4'>
						<select id='usersSelect' class='browser-default'>
							<option value='' disabled selected>Choisissez un utilisateur</option>
							";
							$userslist = GetUsersList();
							foreach($userslist as $key)
							{
								$var .= "<option value='".$key["ID_Personne"]."'>".$key['Nom']." ".$key['Prenom']."";
							}
						$var .= "
						</select>
					</div>
					<div class='input-field col s2'>
						<input id='act_ded' type='text' class='validate'>
						<label for='act_ded'>DED</label>
					</div>
					<div class='input-field col s2'>
						<input id='act_dose' type='text' class='validate'>
						<label for='act_dose'>Dose théorique</label>
					</div>
					<div class='input-field col s2'>
						<input id='act_coeff' type='text' class='validate'>
						<label for='act_coeff'>Coefficient d'exposition</label>
					</div>
					<div class='input-field col s2'>
						 <a id='btn_add_user_act' class='btn-floating btn-large waves-effect waves-light blue'><i class='material-icons'>add</i></a>
					</div>
				</div>
				<div id='table_act_config' class='input-field col s12'>
					<h5>Utilisateurs ajoutés</h5>
					<table class='bordered'>
						<thead>
							<tr>
								<th data-field='PNom'>Utilisateur</th>
								<th data-field='DED'>DED</th>
								<th data-field='Dose'>Dose théorique</th>
								<th data-field='CoeffExp'>Coefficient d'exposition</th>
								<th data-field='RTR'>RTR</th>
								<th data-field='Retirer'>Retirer</th>
								
							</tr>
						</thead>
						<tbody>";
							if(isset($_POST['activityid']) && $_POST['activityid'] > 0)
							{
								$query = $db->prepare("SELECT * FROM activity_config WHERE aID = :id");
								$query->execute(array("id" => $_POST['activityid']));
								$count = $query->rowCount();
								if($count)
								{
									while($result = $query->fetch())
									{
										$query2 = $db->prepare("SELECT * FROM users WHERE ID_Personne = :id");
										$query2->execute(array("id" => $result['aUserID']));
										$user = $query2->fetch();
										$var .= "<tr>";
										$var .= "<td>".$user['Nom']." ".$user['Prenom']."</td>";
										$var .= "<td>".$result['aDED']."</td>";
										$var .= "<td>".$result['aDose']."</td>";
										$var .= "<td>".$result['aCoeff']."</td>";
										$var = "<td><a href='#' class='view_rtr' userid='".$user['ID_Personne']."' >Voir</a></td>";
										$var .= "<td><a href='#' class='remove_user_act' userid='".$user['ID_Personne']."' >Retirer</a></td>";
										$var .= "</tr>";
									}
								}
								else $var .= "<tr><td>Aucun utilisateurs lié à cette activité</td></tr>";
							}
						$var .= 
						"</tbody>
					</table>		
				</div>
				<div class='input-field col s12'>
					<a id='btn_submit_act' class='right waves-effect waves-light btn'>Fermer</a>
				</div>
			</div>			
		</div>
	</div>";
	echo $var;
	echo $_SESSION['aID'];
}
// chargement de toute le table activity dans la page activity.php
if(isset($_POST["type"]) && $_POST["type"] == "activity")
{
	$rep = $db->query('SELECT * FROM activity ORDER BY aID');
	$count = $rep->rowCount();
	$var ="
	<a class='waves-effect waves-light btn' id='btn_add_act'>Ajouter</a>
	<a class='waves-effect waves-light btn' id='btn_delete_all_act'>Supprimer tout</a>
	<h5>Liste des activitées</h5>
	<table class='bordered'>
		<thead>
			<tr>
				<th data-field='Nom'>Nom</th>
				<th data-field='Date'>Date de création</th>
				<th data-field='Classe'>Classe</th>
				<th data-field='Modification'>Modification</th>	
				<th data-field='Suppression'>Suppression</th>
			</tr>
		</thead>
	<tbody>	";	
	if($count)
	{	
			while($result = $rep->fetch(PDO::FETCH_ASSOC))
			{
				$checked = "";
				if($result['aUse']) $checked = "checked='checked'";
				$var .= "<tr>";
				$var .= "<td>".$result['aName']."</td>";
				$var .= "<td>".$result['aDate']."</td>";
				$var .= "<td>".$result['aClasse']."</td>";
				$var .= "<td><a href='#' class='modif_act' activityid='".$result['aID']."'>Voir</a></td>";
				$var .= "<td><a href='#' class='delete_act' activityid='".$result['aID']."' >Supprimer</a></td>";
				$var .= "</tr>";
			}
	}
	else $var .= "<tr><td>Aucune activitée.</td></tr>";
	$var .= "</tbody></table>";
	echo $var;
}

if(isset($_POST["type"]) && $_POST["type"] == "add_act")
{

	$query = $db->prepare("INSERT INTO activity (aName, aClasse, aDate) VALUES (:name, :classe, NOW())");
	$query->execute(array(
	"name" => $_POST["name"],
	"classe" => $_POST["classe"]));
	echo $db->lastInsertId();
}

if(isset($_POST["type"]) && $_POST["type"] == "remove_user_act")
{
	$query = $db->prepare("DELETE FROM activity_config WHERE aID=:id AND aUserID = :userid");
	$query->execute(array("id" => $_POST["id"], "userid" => $_POST["userid"]));
}

if(isset($_POST["type"]) && $_POST["type"] == "delete_act")
{
	$query = $db->prepare("DELETE FROM activity WHERE aID=:id");
	$query->execute(array("id" => $_POST["id"]));
	$query = $db->prepare("DELETE FROM activity_config WHERE aID=:id");
	$query->execute(array("id" => $_POST["id"]));
}

if(isset($_POST["type"]) && $_POST["type"] == "delete_all_act")
{
	$query = $db->query("DELETE FROM activity");
	$query = $db->query("DELETE FROM activity_config");
}

?>
