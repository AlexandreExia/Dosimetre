<?php
include ("./includes/mysql.php");
include("./includes/head.php");
include("./includes/header.php");

$db = enableConnection();
?>

<!DOCTYPE html>
<html>
	<?php
		//gestion du POST du formulaire pour supprimer la ligne en id
		if (isset ($_POST['sup_users_historic']) && $_POST['sup_users_historic'] != null){
			$file1=$_POST['sup_users_historic'];
			$db->exec("DELETE FROM users_historic WHERE (Identificateur='". $file1 ."')");
		}
		 if(isset($_POST["sup_all_users_historic"])){
			$db->exec("DELETE FROM users_historic");
		}
?>
	<?php setHead("Base de Données : Historique des utilisateurs"); ?>
	
	<body>
		<?php setHeader(); ?>
		<div>
			<a class="waves-effect waves-light btn" href="./Users.php">Base de Données : Utilisateurs</a>
			<a class="waves-effect waves-light btn" href="./batterie.php">Base de Données : Batterie</a>
		</div>
		<div class="container valign-wrapper">
		<ul>
			<li>
				<div id="table_users_historic"><!--Div contenant la table de la base de données-->
				</div>
			</li>
			<li>
				<div id="bouton_users_historic"><!--Div contenant les bouton pour modifier la base de données exposition_radio-->
				</div>
			</li>
		</ul>
	</body>
</html>