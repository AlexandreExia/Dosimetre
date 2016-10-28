<?php

require("barcoder.php");

// Instanciation de l'objet pdf
$pdf = new PDF_Code128();
	
// Initialisation du PDF pour l'affichage de l'activité
$sizePage = array(100,100);
$pdf->AddPage('P', $sizePage);
$pdf->SetFont('Arial','',10);
$pdf->SetTitle('R.T.R '.$_GET['iz']);
	
// Placement et affichage du code barre
$code=$_GET['iz'];
/*
*	Argument: x,y,code,width,height
*/
$pdf->Code128(10, 10, $code, 80, 20);
$pdf->Text(10, 35, $code);


// Affichage du PDF
$pdf->Output();

?>
