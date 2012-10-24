 function updateProgressBar(sBasePath) {
 	var vidid = $('#cdn2_iframe').contents().find('#videoToken').val();
   $.ajax({
   type: "GET",
   url: sBasePath + "/?q=cdn2/uploadform/status/"+vidid,
   success: function(msg) {
     var results = msg.split(',');
     var progressBar = function() { updateProgressBar(results[1]); };
     $('#cdn2_iframe').contents().find('#progress-bar').progressBar(Math.ceil(results[0]));
     if(results[0] != 100 && typeof(results[1]) != 'undefined' && typeof(vidid) != 'undefined') {
	    setTimeout(progressBar ,500);
     }
   }});
}