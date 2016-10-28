<?php
include ("./includes/mysql.php");
include("./includes/head.php");
include("./includes/header.php");

$db = enableConnection();
?>

<!DOCTYPE html>
<html>
	<?php setHead("Gestion des utilisateurs"); ?>
	<body>
		<?php setHeader(); ?>
		<div class="container valign-wrapper">
			<div class="row" style="width: 100%;">
				<div id="table_users" class="col s12"  style="margin: 10px"><!--Div contenant la table de la base de données--></div>
				<div id="table_historic" class="col s12"  style="margin: 10px"><!--Div contenant l'historique de l'utilisateur--></div>
				<div id="modal_users" class="col s12"  style="margin: 10px"><!--Div contenant les modals pour modifier la base de données users--></div>
			</div>
		</div>
	</body>
</html>