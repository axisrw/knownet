$(document).ready(
function() {
$('#progress-bar').progressBar({ barImage: window.parent.Drupal.settings.cdn2_bar_image, boxImage: window.parent.Drupal.settings.cdn2_box_image, width: 298 });
$('#progress-bar').hide();
    $('form#upload').submit(
    function() {
  $('#progress-bar').show();
      if($('#uploadFile').val() == '') {
		alert('Please select a file to upload and then click upload.');
      	return false;
      }
      $(window.parent.document).find('input#edit-submit').attr('disabled', 'true');
		$('#progress-bar').show();
		$('#progress-info').show();
		$("div.form-item").hide();
      window.parent.updateProgressBar(sCdn2BasePath);
    }
  );
}
);
