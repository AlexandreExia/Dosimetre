jQuery.fn.extend({
    live: function (event, callback) {
       if (this.selector) {            
            jQuery(document).on(event, this.selector, callback);
        }
    }
});
function containsWord(haystack, needle) {
    return (" " + haystack + " ").indexOf(" " + needle + " ") !== -1;
}
$( document ).ready(function() {
	

	/*
		Chargement des données utilisateurs
	*/
	
	$.ajax({							//Création de la requête HTTP
		method: "POST",					//Requête de type POST
		url: "./ajax/ajax_index.php",	//Url cible
		data: {type: "users"}			//Données transmise lors de la requetes
		})
	.done (function(msg){				//lorsque la requête est executé et la réponse arrivé
		$("#table_users").html(msg);
	});
	
	$.ajax({
		method: "POST",
		url: "./ajax/ajax_index.php",
		data: {type: "modal_users"}
		})
	.done (function(msg){
		console.log(msg);
		$("#modal_users").html(msg);
	});
	
	
	/*
		Affichage des modals
	*/
	$("#modal_boutton_form").live("click", function(){
		$.ajax({
			method: "POST",
			url: "ajax/ajax_index.php",
			data: { type:'modal_boutton_form' }
		})
		.done (function(msg){
			$('#modal_Ajout_form').openModal();
		});
	});

	$("#modal_boutton_suppr_all").live("click", function(){
		$.ajax({
			method: "POST",
			url: "ajax/ajax_index.php",
			data: {type: "modal_boutton_suppr_all"}
		})
		.done (function(msg){
			$('#modal_suppr_all').openModal();
		});
	});
	
	/*
		Visualisation de l'historique de l'utilisateur
	*/
	$("#view_historic").live("click", function(){
		$.ajax({
			method: "POST",
			url: "./ajax/ajax_index.php",
			data: {type: "view_historic", id : $(this).attr("userid") }
			})
			.done (function(msg){
				console.log(msg);
				$("#table_historic").html(msg);
		});
	});
		
	
	/*
		Création/Suppression des utilisateurs
	*/
	
	$("#ajout_users").live("click", function(){
		
		$.ajax({
			method: "POST",
			url: "ajax/ajax_index.php",
			data: {type: "add_users", nom : $("#id_nom").val() ,  prenom : $("#id_prenom").val() ,  badge : $("#id_badge").val() }
		})
		.done (function(msg){
			$.ajax({
				method: "POST",
				url: "./ajax/ajax_index.php",
				data: {type: "users"}
				})
			.done (function(msg){
				console.log(msg);
				$("#table_users").html(msg);
			});
		});
		
	});
	
	$("#delete_all").live("click", function(){
		
		$.ajax({
			method: "POST",
			url: "ajax/ajax_index.php",
			data: {type: "delete_all"}
		})
		.done (function(msg){
			$.ajax({
				method: "POST",
				url: "./ajax/ajax_index.php",
				data: {type: "users"}
				})
			.done (function(msg){
				console.log(msg);
				$("#table_users").html(msg);
			});
		});
		
	});
	
	$("#delete_user").live("click", function(){
		
		$.ajax({
			method: "POST",
			url: "ajax/ajax_index.php",
			data: {type: "delete_user", id : $(this).attr("userid") }
		})
		.done (function(msg){
			$.ajax({
				method: "POST",
				url: "./ajax/ajax_index.php",
				data: {type: "users"}
				})
			.done (function(msg){
				console.log(msg);
				$("#table_users").html(msg);
			});
		});
		
	});
	
});