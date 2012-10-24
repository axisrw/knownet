/* $Id: game.js,v 1.5.2.3 2008/12/24 17:47:48 morbus Exp $ */

/**
 * Drupal.jsEnabled is a generic wrapper for all Drupal JavaScript.
 */
if (Drupal.jsEnabled) {
  $(document).ready(function() {
    $('.activity-comments-click-to-show').click(function(){
      $(this).parent().children('.activity-comments-form-hidden').toggleClass('activity-comments-form-shown');
    });
  });
};
