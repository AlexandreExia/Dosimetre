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
	
	
	$.ajax({
		method: "POST",
		url: "./ajax/ajax_activity.php",
		data: {type: "activity"}
		})
	.done (function(msg){
		$("#table_activity").html(msg);
	});
	
	/*
		Chargement des modals
	*/
	$.ajax({
		method: "POST",
		url: "./ajax/ajax_activity.php",
		data: {type: "modals", activityid : 0}
		})
	.done (function(msg){
		$("#modal_activity").html(msg);
	});
	
	/*
		Suppression d'une activité
	*/
	$(".delete_act").live("click", function() {

		$.ajax({
		method: "POST",
		url: "./ajax/ajax_activity.php",
		data: { type: "delete_act", id: $(this).attr("activityid") }
		})
		.done (function(msg){
			$.ajax({
				method: "POST",
				url: "./ajax/ajax_activity.php",
				data: {type: "activity"}
				})
			.done (function(msg){
				
				$("#table_activity").html(msg);
			});
		});
	
	});
	
	/*
		Suppression de toutes les activitées
	*/
	$("#btn_delete_all_act").live("click", function() {

		$.ajax({
			method: "POST",
			url: "./ajax/ajax_activity.php",
			data: { type: "delete_all_act" }
		})
		.done (function(msg){
			$.ajax({
				method: "POST",
				url: "./ajax/ajax_activity.php",
				data: {type: "activity"}
				})
			.done (function(msg){
				
				$("#table_activity").html(msg);
			});
		});
	
	});
	
	/*
		Suppression d'un utilisateur d'une activité
	*/
	$(".remove_user_act").live("click", function(){
	
		/*
			Rafraichissement du tableau contenant la liste des utilisateurs
		*/
		$.ajax({
			method: "POST",
			url: "./ajax/ajax_activity.php",
			data: {type: "remove_user_act", id : $('#modal_add_users_act').attr("activityid"), userid : $(this).attr("userid")}
		})
		.done (function(msg){
			$.ajax({
				method: "POST",
				url: "./ajax/ajax_activity.php",
				data: {type: "refresh_users_list", activityid : $('#modal_add_users_act').attr("activityid")}
			})
			.done (function(msg){
				$("#table_act_config tbody").html(msg);
			});	
		});	
	
	});

	/*
		Création de l'activité
	*/
	
	$("#btn_next_act").live("click", function() {
		
		$.ajax({
		method: "POST",
		url: "./ajax/ajax_activity.php",
		data: { type: "add_act", name: $("#act_name").val(), classe: $("#act_class").val() }
		})
		.done (function(msg){
			/*
				Rafraichissement du tableau qui affiche les activitées
			*/
			$.ajax({
				method: "POST",
				url: "./ajax/ajax_activity.php",
				data: {type: "activity"}
			})
			.done (function(msg){
				$("#table_activity").html(msg);
			});
			
			$("#modal_add_act").closeModal();
			$('#modal_add_users_act').openModal();
			
			/*
				Rafraichissement du tableau contenant la liste des utilisateurs
			*/
			var activityid = msg;
			$('#modal_add_users_act').attr("activityid", activityid);
			$.ajax({
				method: "POST",
				url: "./ajax/ajax_activity.php",
				data: {type: "refresh_users_list", activityid : activityid}
			})
			.done (function(msg){
				$("#table_act_config tbody").html(msg);
			});	
		});
	});
	
	/*
		Ajout d'utilisateur dans une activitée
	*/
	$("#btn_add_user_act").live("click", function() {
		
		$.ajax({
		method: "POST",
		url: "./ajax/ajax_activity.php",
		data: { type: "add_user", activityid: $('#modal_add_users_act').attr("activityid"), userID: $("#usersSelect").val(), ded: $("#act_ded").val(), dose : $("#act_dose").val(), coeff: $("#act_coeff").val() }
		})
		.done (function(msg){
			$.ajax({
				method: "POST",
				url: "./ajax/ajax_activity.php",
				data: {type: "refresh_users_list", activityid : $('#modal_add_users_act').attr("activityid")}
			})
			.done (function(msg){
				console.log(msg);
				$("#table_act_config tbody").html(msg);	
			});	
		});
		
	});
	
	/*
		Modification d'une activitée
	*/
	$(".modif_act").live("click", function(){
	
		$('#modal_add_users_act').openModal();
		var activityid = $(this).attr("activityid");
		$('#modal_add_users_act').attr("activityid", activityid);
		$.ajax({
				method: "POST",
				url: "./ajax/ajax_activity.php",
				data: {type: "refresh_users_list", activityid : activityid}
		})
		.done (function(msg){
				$("#table_act_config tbody").html(msg);
		});	
		
	});
	
	/*
		Visualisation du RTR
	*/
	$(".view_rtr").live("click", function(){
	
		var activityid = $(this).attr("activityid");
		$.ajax({
				method: "POST",
				url: "./ajax/ajax_activity.php",
				data: {type: "view_rtr", activityid : $('#modal_add_users_act').attr("activityid"), userid : $(this).attr("userid")}
		})
		.done (function(msg){
		
			window.open("./includes/rtr.php?iz="+msg, "blank"); 

				
		});	
		
	});
	
	
	/*
		Ouverture des modals
	*/
	$("#btn_add_act").live("click", function() {
		$('#modal_add_act').openModal();		
	});
	

});