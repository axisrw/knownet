<?php
// $Id: fbsmp_views_handler_field_themed_attachment.inc,v 1.1.2.2 2010/05/16 12:43:09 publicmind Exp $

/**
 * @file
 *   Formats FBSMP attachment in Views.
 */

/**
 * Field handler to provide the themed attachment for FBSS status.
 */
class fbsmp_views_handler_field_themed_attachment extends views_handler_field {
  
  function init(&$view, $options) {
    parent::init($view, $options);
    $this->additional_fields['sid'] = array('table' => 'fbsmp', 'field' => 'sid');
    $this->additional_fields['type'] = array('table' => 'fbsmp', 'field' => 'type');
    $this->additional_fields['data'] = array('table' => 'fbsmp', 'field' => 'data');
  }
  
  function query() {
    $this->add_additional_fields();
  }
  
  function render($values) {
    $attachment = new stdClass();
    $attachment->sid = $values->{$this->aliases['sid']};
    $attachment->type = $values->{$this->aliases['type']};
    $attachment->data = unserialize($values->{$this->aliases['data']});
    $function = fbsmp_load_function($attachment->type, 'themed attachment');
    if ($function) {
      $themed_attachment = $function($attachment);
      return '<span class="fbss_attachment">'. $themed_attachment . '</span>';
    }
    return '';
  }
}