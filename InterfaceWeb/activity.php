<?php
include ("./includes/mysql.php");
include("./includes/head.php");
include("./includes/header.php");

$db = enableConnection();
?>

<!DOCTYPE html>
<html>
	<?php setHead("Gestion des activitées", '"UTF-8"', "activity"); ?>
	<body>
		<?php setHeader(); ?>
		<div class="container valign-wrapper">
			<div class="row" style="width: 100%;">
				<div id="table_activity" class="col s12" style="margin: 10px"><!--Div contenant la table de la base de donn""es--></div>
				<div id="modal_activity" class="col s12"  style="margin: 10px"><!--Div contenant les modals pour modifier la base de données activity--></div>
			</div>
		</div>
	</body>
</html>