<?php

/*
 * Create the field_best_answer to be installed in the 'question' cck content type
 */

 function _best_answer_field_best_answer() {
  $array = array(
  'field_name' => 'field_best_answer',
  'type_name' => 'question',
  'display_settings' => 
  array(
    'weight' => '5',
    'parent' => '',
    'label' => 
    array(
      'format' => 'above',
    ),
    'teaser' => 
    array(
      'format' => 'default',
      'exclude' => 1,
    ),
    'full' => 
    array(
      'format' => 'default',
      'exclude' => 1,
    ),
    4 => 
    array(
      'format' => 'default',
      'exclude' => 0,
    ),
  ),
  'widget_active' => '1',
  'type' => 'nodereference',
  'required' => '0',
  'multiple' => '0',
  'db_storage' => '1',
  'module' => 'nodereference',
  'active' => '1',
  'locked' => '0',
  'columns' => 
  array(
    'nid' => 
    array(
      'type' => 'int',
      'unsigned' => TRUE,
      'not null' => FALSE,
      'index' => TRUE,
    ),
  ),
  'referenceable_types' => 
  array(
    'answer' => 'answer',
    'page' => 0,
    'question' => 0,
    'story' => 0,
  ),
  'advanced_view' => '--',
  'advanced_view_args' => '',
  'widget' => 
  array(
    'autocomplete_match' => 'contains',
    'size' => '60',
    'default_value' => 
    array(
      0 => 
      array(
        'nid' => NULL,
        '_error_element' => 'default_value_widget][field_best_answer][0][nid][nid',
      ),
    ),
    'default_value_php' => NULL,
    'label' => 'Best Answer',
    'weight' => '5',
    'description' => '',
    'type' => 'nodereference_autocomplete',
    'module' => 'nodereference',
  ),
);
return $array;
}

/*
 * Create the field_best_answer_p to be installed in the 'answer' cck content type
 */
function _best_answer_field_best_answer_p() {
  $array = array(
  'field_name' => 'field_best_answer_p',
  'type_name' => 'answer',
  'display_settings' => 
  array(
    'weight' => '12',
    'parent' => '',
    'label' => 
    array(
      'format' => 'above',
    ),
    'teaser' => 
    array(
      'format' => 'default',
      'exclude' => 1,
    ),
    'full' => 
    array(
      'format' => 'default',
      'exclude' => 1,
    ),
    4 => 
    array(
      'format' => 'default',
      'exclude' => 0,
    ),
    2 => 
    array(
      'format' => 'default',
      'exclude' => 0,
    ),
    3 => 
    array(
      'format' => 'default',
      'exclude' => 0,
    ),
  ),
  'widget_active' => '1',
  'type' => 'number_integer',
  'required' => '0',
  'multiple' => '0',
  'db_storage' => '1',
  'module' => 'number',
  'active' => '1',
  'locked' => '0',
  'columns' => 
  array(
    'value' => 
    array(
      'type' => 'int',
      'not null' => FALSE,
      'sortable' => TRUE,
    ),
  ),
  'prefix' => '',
  'suffix' => '',
  'min' => '',
  'max' => '',
  'allowed_values' => '0 | No
1 | Yes
',
  'allowed_values_php' => '',
  'widget' => 
  array(
    'default_value' => 
    array(
      0 => 
      array(
        'value' => '0',
        '_error_element' => 'default_value_widget][field_best_answer_p][0][value',
      ),
    ),
    'default_value_php' => NULL,
    'label' => 'Best Answer',
    'weight' => '12',
    'description' => 'Has the question author selected this answer as the "best answer"?',
    'type' => 'number',
    'module' => 'number',
  ),
);
return $array;
}
