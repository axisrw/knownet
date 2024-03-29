<?php
// $Id$

/**
 * @file
 * Provide views data and handlers for the Facebook-style Statuses module.
 */

/**
 * Implementation of hook_views_data().
 */
function fbssc_views_data() {
  //Basic table information.
  $data['fbssc']['table']['group']  = t('Facebook-style Statuses Comments');
  $data['fbssc']['table']['join'] = array(
    'users' => array(
      'left_field' => 'uid',
      'field' => 'uid',
      'type' => 'INNER',
    ),
    'facebook_status' => array(
      'left_field' => 'sid',
      'field' => 'sid',
      'type' => 'INNER',
    ),
  );

  //Advertise this table as a possible base table.
  $data['fbssc']['table']['base'] = array(
    'field' => 'cid',
    'title' => t('Facebook-style Statuses Comments'),
    'help' => t('Stores status comments.'),
    'weight' => 10,
  );

  //Alias for the FBSS Status ID to extend its use.
  $data['facebook_status']['comment-box'] = array(
    'title' => t('Status comment box'),
    'help' => t('Shows the list of comments for the relevant status and the form to submit a new status comment.'),
    'field' => array(
      'field' => 'sid',
      'handler' => 'fbssc_views_handler_field_comment_box',
      'click sortable' => FALSE,
    ),
  );


  //Declares the Comment ID column.
  $data['fbssc']['cid'] = array(
    'title' => t('Status Comment ID'),
    'help' => t('The ID of the status comment.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  //Declares the Status ID column.
  $data['fbssc']['sid'] = array(
    'title' => t('Status ID'),
    'help' => t('The ID of the status.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  //Declares the User ID column.
  $data['fbssc']['uid'] = array(
    'title' => t('User ID'),
    'help' => t('The User ID of the status comment author.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  //Declares the status comment timestamp column.
  $data['fbssc']['comment_time'] = array(
    'title' => t('Created time'),
    'help' => t('The time the status comment was posted.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  //Declares the status comment text column.
  $data['fbssc']['comment'] = array(
    'title' => t('Comment text'),
    'help' => t('The text of the status comment.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => FALSE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  return $data;
}

/**
 * Implementation of hook_views_data_alter().
 *
 */
function fbssc_views_data_alter(&$data) {
  $data['fbssc']['users'] = array(
    'group' => t('Facebook-style Statuses Comments'),
    'relationship' => array(
      'title' => t('Users'),
      'label' => t('Users'),
      'help' => t('Add a relationship to gain access to information related to the users who submitted the relevant status comments.'),
      'relationship table' => 'fbssc',
      'relationship field' => 'uid',
      'base' => 'users',
      'field' => 'uid',
      'type' => 'INNER',
      'handler' => 'views_handler_relationship',
    ),
  );
  $data['fbssc']['facebook_status'] = array(
    'group' => t('Facebook-style Statuses Comments'),
    'relationship' => array(
      'title' => t('Statuses'),
      'label' => t('Statuses'),
      'help' => t('Add a relationship to gain access to information related to the statuses themselves.'),
      'relationship table' => 'fbssc',
      'relationship field' => 'sid',
      'base' => 'facebook_status',
      'field' => 'sid',
      'type' => 'INNER',
      'handler' => 'views_handler_relationship',
    ),
  );
}

/**
 * Implementation of hook_views_handlers().
 */
function fbssc_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'fbssc'),
    ),
    'handlers' => array(
      'fbssc_views_handler_field_comment_box' => array(
        'parent' => 'views_handler_field',
      ),
    ),
  );
}