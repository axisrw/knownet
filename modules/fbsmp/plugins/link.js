// $Id: link.js,v 1.1.2.2 2010/05/16 12:43:09 publicmind Exp $
Drupal.behaviors.fbsmpLink = function (context) {
  $link_previous_button = $('.fbsmp-link-previous-button');
  $link_next_button = $('.fbsmp-link-next-button');

  $link_previous_button.click(function() {
      var current = $(this).parent().prev().val();
      var max = $(this).parent().prev().prev().val();
      current--;
      if (current>0){
          $(this).parent().prev().val(current);
          fbsmp_link_print_count(current, max, $(this).next().next());
          var change = ".fbsmp-link-thumbnail-" + current;
          var new_thumb = $(this).parent().parent().find(change).val();
          $(this).prev().attr('src', new_thumb);
      }   
    } 
  );

  $link_next_button.click(function() {
      var current = $(this).parent().prev().val();
      var max = $(this).parent().prev().prev().val();
      current++;
      if (current<=max){
          $(this).parent().prev().val(current);
          fbsmp_link_print_count(current, max, $(this).next());
          var change = ".fbsmp-link-thumbnail-" + current;
          var new_thumb = $(this).parent().parent().find(change).val();
          $(this).prev().prev().attr('src', new_thumb);
      }  
    } 
  );
}

function fbsmp_link_print_count(current, max, where) {
  var link_translations = new Array();
  link_translations['%current'] = current;
  link_translations['%max'] = max;
  where.html(Drupal.t('%current of %max', link_translations));
}