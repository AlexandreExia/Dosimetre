<!DOCTYPE html>
<html>
	<head>
		<meta charset="utf-8">
		<title>Batterie pour Raspberry></title>
	</head>
	<body>
		<?php
		include ("../includes/mysql.php");
		$db = enableConnection();
		$_date=date('Y-m-d G:i:s');
		/*
		Code 1 = Lecture de la BdD
		Code 2 = Mise à jour de la BdD
		Code 3 = Insertion dans la BdD (en cas de nouveau dosimètre)
		*/
		if (!empty($_GET['code']))		//Code doit être avec la valeur 1 ou + sans quoi rien ne s'afiche ==> code=0 n'affiche rien car 0=NULL=Vide
		{
		
			$code = $_GET['code'];
			$dosi = $_GET['dosi'];
			$flag = $_GET['flag'];
			$action= $_GET['action'];
			$sonde = $_GET['sonde'];
			$sonde = str_replace("\\", "", $sonde);
			$sonde = str_replace("/", "", $sonde);
			//$taillesonde= strlen($sonde);
			//$sonde[$taillesonde-1]="\0";
			echo $sonde."<br>";
			//$cycle = $_GET['cycle']+1;
			if($code==1)
			{
				$rep = $db->query("SELECT * FROM batterie WHERE(SONDE='$sonde')");
				$count = $rep->rowCount();
				if($count)
				{
					echo "%=1/<br>"; //Si le rasp detecte ce 1 c'est car le dosi chercher est dans la table
					//---------------------------------------------------     chargement de la table selon l'id Dosi		-------------------------------
					$var = "<table>";
					while($result = $rep->fetch(PDO::FETCH_ASSOC))
					{
						$var .= "<tr>";
						$var .= "<td>&=".$result['ID_DOSI']."/</td>";
						$var .= "<td>é=".$result['NBR_CYCLE']."/</td>";
						$var .= "<td>ç=".$result['DATE_CONNECTION']."/</td>";
						$var .= "<td>à=".$result['DATE_DECONNEC']."/</td>";
						$var .= "<td>@=".$result['TEST_DECHARGE']."/</td>";
						$var .= "<td>*=".$result['TEST_CHARGE']."/</td>";
						$var .= "<td>µ=".$result['CHARGE_RESTANTE']."/</td>";
						$var .= "<td>+=".$result['FLAG']."/</td>";
						$var .= "<td>!=".$result['SONDE']."/</td>";
						$var .= "</tr>";
					}
					$var .= "</table>";
					echo $var;
					$testcycle= $result['NBR_CYCLE'];
					echo $testcycle."toto";
										
					//-------------------------------------  Test du cycle   ------------------------------
					$testcycle= $result['NBR_CYCLE'];
					if($testcycle == 40)
					{
						echo "`=1/";//Si le rasp voit ce 1, il sait qu'il faut faire un test de capacité
						$db->query("UPDATE batterie SET NBR_CYCLE='0' WHERE(SONDE='$sonde')"); //comme le rasp fait un test capacité, on met les cycles a 0 car il y aura un entretien
					}	
				}
				
				else echo "%=0/"; // si le rasp detecte ce 0, il insere le dosi dont la sonde était recherhcer car il n'est pas sur la bdd
			}
			
			elseif($code==2 && $action==0)
			{
				$db->query("UPDATE batterie SET DATE_CONNECTION='$_date' WHERE (SONDE='$sonde')");
				echo "Update fait";
			}
			
			elseif($code==2 && $action==1)
			{
				$db->query("UPDATE batterie SET DATE_DECONNEC='$_date', FLAG='$flag', NBR_CYCLE='$cycle' WHERE (SONDE='$sonde')");
				echo "Update fait";
			}
			
			elseif($code==3)
			{
				$db->query("INSERT INTO batterie (NBR_CYCLE, DATE_CONNECTION, DATE_DECONNEC, TEST_DECHARGE, TEST_CHARGE, CHARGE_RESTANTE, FLAG, SONDE) VALUES('$cycle', '$_date','' , '', '', '', '$flag','$sonde')");
				echo "Insertion effectuée";
		
			}
			
		}
		else echo "Rien à afficher";
		?>
	</body>
</html>
