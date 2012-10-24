<?php

$view = new view;
$view->name = 'all_questions';
$view->description = 'List of all questions';
$view->tag = 'Questions';
$view->view_php = '';
$view->base_table = 'node';
$view->is_cacheable = FALSE;
$view->api_version = 2;
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->override_option('fields', array(
  'title' => array(
    'label' => 'Question',
    'alter' => array(
      'alter_text' => 0,
      'text' => '',
      'make_link' => 0,
      'path' => '',
      'link_class' => '',
      'alt' => '',
      'prefix' => '',
      'suffix' => '',
      'target' => '',
      'help' => '',
      'trim' => 0,
      'max_length' => '',
      'word_boundary' => 1,
      'ellipsis' => 1,
      'html' => 0,
      'strip_tags' => 0,
    ),
    'empty' => 'No questions are available',
    'hide_empty' => 0,
    'empty_zero' => 0,
    'link_to_node' => 1,
    'exclude' => 0,
    'id' => 'title',
    'table' => 'node',
    'field' => 'title',
    'relationship' => 'none',
  ),
  'created' => array(
    'label' => 'Post date',
    'alter' => array(
      'alter_text' => 0,
      'text' => '',
      'make_link' => 0,
      'path' => '',
      'link_class' => '',
      'alt' => '',
      'prefix' => '',
      'suffix' => '',
      'target' => '',
      'help' => '',
      'trim' => 0,
      'max_length' => '',
      'word_boundary' => 1,
      'ellipsis' => 1,
      'html' => 0,
      'strip_tags' => 1,
    ),
    'empty' => '',
    'hide_empty' => 0,
    'empty_zero' => 0,
    'date_format' => 'time ago',
    'custom_date_format' => '',
    'exclude' => 0,
    'id' => 'created',
    'table' => 'node',
    'field' => 'created',
    'relationship' => 'none',
  ),
  'field_answer_count_value' => array(
    'label' => 'Answers',
    'alter' => array(
      'alter_text' => 0,
      'text' => '',
      'make_link' => 0,
      'path' => '',
      'link_class' => '',
      'alt' => '',
      'prefix' => '',
      'suffix' => '',
      'target' => '',
      'help' => '',
      'trim' => 0,
      'max_length' => '',
      'word_boundary' => 1,
      'ellipsis' => 1,
      'html' => 0,
      'strip_tags' => 0,
    ),
    'empty' => 'none',
    'hide_empty' => 0,
    'empty_zero' => 1,
    'link_to_node' => 0,
    'label_type' => 'custom',
    'format' => 'default',
    'multiple' => array(
      'group' => TRUE,
      'multiple_number' => '',
      'multiple_from' => '',
      'multiple_reversed' => FALSE,
    ),
    'exclude' => 0,
    'id' => 'field_answer_count_value',
    'table' => 'node_data_field_answer_count',
    'field' => 'field_answer_count_value',
    'relationship' => 'none',
    'override' => array(
      'button' => 'Override',
    ),
  ),
));
$handler->override_option('sorts', array(
  'created' => array(
    'id' => 'created',
    'table' => 'node',
    'field' => 'created',
  ),
));
$handler->override_option('filters', array(
  'type' => array(
    'operator' => 'in',
    'value' => array(
      'question' => 'question',
    ),
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
      'operator' => FALSE,
      'label' => '',
    ),
    'id' => 'type',
    'table' => 'node',
    'field' => 'type',
    'relationship' => 'none',
  ),
  'status' => array(
    'operator' => '=',
    'value' => '1',
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
      'operator' => FALSE,
      'label' => '',
    ),
    'id' => 'status',
    'table' => 'node',
    'field' => 'status',
    'relationship' => 'none',
  ),
  'keys' => array(
    'id' => 'keys',
    'table' => 'search_index',
    'field' => 'keys',
  ),
));
$handler->override_option('access', array(
  'type' => 'none',
));
$handler->override_option('cache', array(
  'type' => 'none',
));
$handler->override_option('header_format', '1');
$handler->override_option('header_empty', 1);
$handler->override_option('empty_format', '1');
$handler->override_option('style_plugin', 'table');
$handler->override_option('style_options', array(
  'grouping' => '',
  'override' => 1,
  'sticky' => 1,
  'order' => 'desc',
  'columns' => array(
    'title' => 'title',
    'created' => 'created',
    'field_answer_count_value' => 'field_answer_count_value',
  ),
  'info' => array(
    'title' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'created' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'field_answer_count_value' => array(
      'sortable' => 1,
      'separator' => '',
    ),
  ),
  'default' => 'created',
));
$handler = $view->new_display('page', 'Default Page', 'page_1');
$handler->override_option('path', 'questions');
$handler->override_option('menu', array(
  'type' => 'none',
  'title' => '',
  'description' => '',
  'weight' => 0,
  'name' => 'navigation',
));
$handler->override_option('tab_options', array(
  'type' => 'none',
  'title' => '',
  'description' => '',
  'weight' => 0,
  'name' => 'navigation',
));
$handler = $view->new_display('page', 'All Questions', 'page_2');
$handler->override_option('empty', 'No questions yet ....');
$handler->override_option('path', 'questions/all');
$handler->override_option('menu', array(
  'type' => 'default tab',
  'title' => 'All Questions',
  'description' => 'All questions',
  'weight' => '0',
  'name' => 'navigation',
));
$handler->override_option('tab_options', array(
  'type' => 'none',
  'title' => '',
  'description' => '',
  'weight' => '0',
  'name' => 'navigation',
));
$handler = $view->new_display('page', 'Unanswered Questions', 'page_3');
$handler->override_option('filters', array(
  'status_extra' => array(
    'operator' => '=',
    'value' => '',
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
      'operator' => FALSE,
      'label' => '',
    ),
    'id' => 'status_extra',
    'table' => 'node',
    'field' => 'status_extra',
    'relationship' => 'none',
  ),
  'type' => array(
    'operator' => 'in',
    'value' => array(
      'question' => 'question',
    ),
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
      'operator' => FALSE,
      'label' => '',
    ),
    'id' => 'type',
    'table' => 'node',
    'field' => 'type',
    'relationship' => 'none',
  ),
  'field_answer_count_value' => array(
    'operator' => '<',
    'value' => array(
      'value' => '1',
      'min' => '',
      'max' => '',
    ),
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
      'operator' => FALSE,
      'label' => '',
    ),
    'id' => 'field_answer_count_value',
    'table' => 'node_data_field_answer_count',
    'field' => 'field_answer_count_value',
    'override' => array(
      'button' => 'Use default',
    ),
    'relationship' => 'none',
  ),
));
$handler->override_option('header', '<em>"Unanswered questions" have no answers yet.</em> ');
$handler->override_option('empty', '<em>All questions have been answered ....</em>');
$handler->override_option('path', 'questions/unanswered');
$handler->override_option('menu', array(
  'type' => 'tab',
  'title' => 'Unanswered Questions',
  'description' => '"Unanswered questions" have no answers yet',
  'weight' => '30',
  'name' => 'navigation',
));
$handler->override_option('tab_options', array(
  'type' => 'none',
  'title' => '',
  'description' => '',
  'weight' => 0,
  'name' => 'navigation',
));
$handler = $view->new_display('page', 'Search Page', 'page_4');
$handler->override_option('filters', array(
  'type' => array(
    'operator' => 'in',
    'value' => array(
      'question' => 'question',
    ),
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
      'operator' => FALSE,
      'label' => '',
    ),
    'id' => 'type',
    'table' => 'node',
    'field' => 'type',
    'relationship' => 'none',
  ),
  'status' => array(
    'operator' => '=',
    'value' => '1',
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
      'operator' => FALSE,
      'label' => '',
    ),
    'id' => 'status',
    'table' => 'node',
    'field' => 'status',
    'relationship' => 'none',
  ),
  'keys' => array(
    'operator' => 'optional',
    'value' => '',
    'group' => '0',
    'exposed' => TRUE,
    'expose' => array(
      'use_operator' => 0,
      'operator' => 'keys_op',
      'identifier' => 'keys',
      'label' => '',
      'optional' => 1,
      'remember' => 0,
    ),
    'id' => 'keys',
    'table' => 'search_index',
    'field' => 'keys',
    'override' => array(
      'button' => 'Use default',
    ),
    'relationship' => 'none',
  ),
));
$handler->override_option('header', '<h2 =\'answers-search-title\'>What question would you like to search for?</h2>');
$handler->override_option('header_format', '2');
$handler->override_option('empty', '<em>No questions match ...</em>');
$handler->override_option('path', 'questions/search');
$handler->override_option('menu', array(
  'type' => 'none',
  'title' => '',
  'description' => '',
  'weight' => 0,
  'name' => 'navigation',
));
$handler->override_option('tab_options', array(
  'type' => 'none',
  'title' => '',
  'description' => '',
  'weight' => 0,
  'name' => 'navigation',
));
$handler = $view->new_display('page', 'Resolved Questions', 'page_5');
$handler->override_option('filters', array(
  'type' => array(
    'operator' => 'in',
    'value' => array(
      'question' => 'question',
    ),
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
      'operator' => FALSE,
      'label' => '',
    ),
    'id' => 'type',
    'table' => 'node',
    'field' => 'type',
    'relationship' => 'none',
  ),
  'status' => array(
    'operator' => '=',
    'value' => '1',
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
      'operator' => FALSE,
      'label' => '',
    ),
    'id' => 'status',
    'table' => 'node',
    'field' => 'status',
    'relationship' => 'none',
  ),
  'field_best_answer_nid' => array(
    'operator' => 'not empty',
    'value' => array(),
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
      'operator' => FALSE,
      'label' => '',
    ),
    'id' => 'field_best_answer_nid',
    'table' => 'node_data_field_best_answer',
    'field' => 'field_best_answer_nid',
    'override' => array(
      'button' => 'Use default',
    ),
    'relationship' => 'none',
    'reduce_duplicates' => 0,
  ),
));
$handler->override_option('header', '<em>"Resolved questions" have a "Best Answer" selected</em>');
$handler->override_option('header_empty', 0);
$handler->override_option('empty', '<em>No questions have been resolved yet ....</em>');
$handler->override_option('path', 'questions/resolved');
$handler->override_option('menu', array(
  'type' => 'tab',
  'title' => 'Resolved Questions',
  'description' => 'Questions for which a "Best Answer" has been selected',
  'weight' => '10',
  'name' => 'navigation',
));
$handler->override_option('tab_options', array(
  'type' => 'none',
  'title' => '',
  'description' => '',
  'weight' => 0,
  'name' => 'navigation',
));
$handler = $view->new_display('page', 'Unresolved Questions', 'page_6');
$handler->override_option('filters', array(
  'type' => array(
    'operator' => 'in',
    'value' => array(
      'question' => 'question',
    ),
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
      'operator' => FALSE,
      'label' => '',
    ),
    'id' => 'type',
    'table' => 'node',
    'field' => 'type',
    'relationship' => 'none',
  ),
  'status' => array(
    'operator' => '=',
    'value' => '1',
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
      'operator' => FALSE,
      'label' => '',
    ),
    'id' => 'status',
    'table' => 'node',
    'field' => 'status',
    'relationship' => 'none',
  ),
  'field_best_answer_nid' => array(
    'operator' => 'empty',
    'value' => array(),
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
      'operator' => FALSE,
      'label' => '',
    ),
    'id' => 'field_best_answer_nid',
    'table' => 'node_data_field_best_answer',
    'field' => 'field_best_answer_nid',
    'override' => array(
      'button' => 'Use default',
    ),
    'relationship' => 'none',
    'reduce_duplicates' => 0,
  ),
  'field_answer_count_value' => array(
    'operator' => '>',
    'value' => array(
      'value' => '0',
      'min' => '',
      'max' => '',
    ),
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
      'operator' => FALSE,
      'label' => '',
    ),
    'id' => 'field_answer_count_value',
    'table' => 'node_data_field_answer_count',
    'field' => 'field_answer_count_value',
    'override' => array(
      'button' => 'Use default',
    ),
    'relationship' => 'none',
  ),
));
$handler->override_option('header', '<em>"Unresolved questions" have answers but no "Best Answer" selected yet</em>');
$handler->override_option('empty', '<em>Currently, no questions are unresolved ....</em>');
$handler->override_option('path', 'questions/unresolved');
$handler->override_option('menu', array(
  'type' => 'tab',
  'title' => 'Unresolved Questions',
  'description' => '"Unresolved Questions" are those which have answers but for which no "Best Answer" has been selected',
  'weight' => '20',
  'name' => 'navigation',
));
$handler->override_option('tab_options', array(
  'type' => 'none',
  'title' => '',
  'description' => '',
  'weight' => 0,
  'name' => 'navigation',
));
