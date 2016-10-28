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
	
	
	//affichage de la table batterie
	$.ajax({
		method: "POST",
		url: "ajax/ajax_batterie.php",
		data: {type: "batterie"}
		})
	.done (function(msg){
		console.log(msg);
		$("#table_batt").html(msg);
	});
	
});