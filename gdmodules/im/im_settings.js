;if (Drupal && Drupal.jsEnabled) {
 
  Drupal.behaviors.im = function(context) {
    var fid_tf = 'edit-im-text-entry-type-textfield';
    var fid_ta = 'edit-im-text-entry-type-textarea';
    var $fcb_tf = $('#'+ fid_tf);
    var $fcb_ta = $('#'+ fid_ta);
    var $fopts_tf = $('#im-textfield-options');
    var $fopts_ta = $('#im-textarea-options');
		
		var $cbs = $('input[@name=im_text_entry_type]');
    if (!$cbs.length) {
      return ;
    }
		
    if (!$("#edit-im-text-entry-type-textarea:checked").length == 0) {
        $('#im-textfield-options').hide();
        $('#im-textarea-options').show();
    }
    else {
        $('#im-textfield-options').show();
        $('#im-textarea-options').hide();
    }

    $fcb_tf.bind('click', function(e) {
				$('#im-textfield-options').show();
        $('#im-textarea-options').hide();
    });
    $fcb_ta.bind('click', function(e) {
        $('#im-textfield-options').hide();
        $('#im-textarea-options').show();
    });

  };

}
