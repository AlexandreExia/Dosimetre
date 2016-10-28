<?php
include ("./includes/mysql.php");
include("./includes/head.php");
include("./includes/header.php");

$db = enableConnection();
?>

<!DOCTYPE html>
<html>
	<?php setHead("Gestion de la batterie", '"UTF-8"', "batterie"); ?>
	
	<body>
		<?php setHeader(); ?>
		<div class="container valign-wrapper">
			<div id="table_batt"><!--Div contenant la table de la base de données--></div>
	</body>
</html>