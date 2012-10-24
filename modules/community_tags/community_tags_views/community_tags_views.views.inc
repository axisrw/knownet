<?php
// $Id$

function community_tags_views_data()
{
  $data = array (

    'community_tags'=> array (
      'table'=> array (
        'group'=>t('Community Tags'),
        'base'=> array (
          'field'=>'tid',
          'title'=>t('Community Tags Entries'),
          'help'=>t("Records of who tagged what when (for Community Tag vocabularies)"),
          ),
        'join'=> array (
          'node'=> array (
            'left_field'=>'nid',
            'field'=>'nid',
            ),
          ),
        ),
      'uid'=> array (
        'title'=>t('Tagging User'),
        'help'=>t('The UID of the user who created the tag instance'),
        'field'=> array (
          'handler'=>'views_handler_field_user',
          'click sortable'=>TRUE,
          ),
        'argument'=> array (
          'handler'=>'views_handler_argument_user_uid',
          'name field'=>'name',
          ),
        'filter'=> array (
          'title'=>t('Name'),
          'handler'=>'views_handler_filter_user_name',
           ),
        'sort'=> array (
          'handler'=>'views_handler_sort',
          ),
        'relationship'=> array (
          'base'=>'users',
          'base field'=>'uid',
          'relationship field'=>'uid',
          'label'=>t('Community Taxonomy Tagging User'),
          ),
        ),

      'date'=> array (
        'title'=>t('Tagging Time'),
        'help'=>t('When the tag instance happened'),
        'field'=> array (
          'handler'=>'views_handler_field_date',
          'click sortable'=>TRUE,
          ),
        'sort'=> array (
          'handler'=>'views_handler_sort_date',
          ),
        'filter'=> array (
          'handler'=>'views_handler_filter_date',
          ),
        'argument'=> array (
          'handler'=>'views_handler_argument_date',
           ),
        ),

       'nid'=> array (
        'title'=>t('Tagged Node'),
        'help'=>t('The node that was tagged'),
        'field'=> array (
          'handler'=>'views_handler_field_numeric',
          'click sortable'=>TRUE,
          ),
        'sort'=> array (
          'handler'=>'views_handler_sort',
          ),
        'filter'=> array (
          'handler'=>'views_handler_filter_numeric',
          ),
        'argument'=> array (
          'handler'=>'views_handler_argument_numeric',
        ),
        'relationship'=> array (
          'base' => 'node',
          'base field' => 'nid',
          'relationship field' => 'nid',
          'label' => t('Community Taxonomy Tagged Node'),
        ),
        ),

      'tid'=> array (
        'title'=>t('Tag ID'),
        'help'=>t('The TID of the Tag'),
        'field'=> array (
          'handler'=>'views_handler_field_numeric',
          'click sortable'=>TRUE,
          ),
        'sort'=> array (
          'handler'=>'views_handler_sort_numeric',
          ),
        'filter'=> array (
          'handler'=>'views_handler_filter_numeric',
          ),
        'argument'=> array (
          'handler'=>'views_handler_argument_numeric',
          'skip base'=> array ('node', 'node_revision'),
          ),
        'relationship'=> array (
          'base'=>'term_data',
          'base field'=>'tid',
          'relationship field'=>'tid',
          'label'=>t('Community Taxonomy Tagged Term'),
          ),
        ),

      ),

    'community_tags_aggregates' => array (
      'table' => array(
        'group' => 'Community Tags Aggregates ',
        'join' => array(
          'term_data' => array(
            'table' => 'community_tags',
            'type' => 'INNER',
            'left_field' => 'tid',
            'field' => 'tid',
          ),
          'user' => array(
            'table' => 'community_tags',
            'type' => 'INNER',
            'left_field' => 'uid',
            'field' => 'uid',
          ),
          'node' => array(
            'table' => 'community_tags',
            'type' => 'INNER',
            'left_field' => 'nid',
            'field' => 'nid',
          ),
        ),
      ),

      'ct_tag_count' => array (
        'title' => t('Tag Count'),
        'help' => t('The number of tags.'),
        'field' => array(
          'handler' => 'community_tags_views_handler_field_tag_count',
          'click sortable' => TRUE,
        ),
        'sort'=> array (
          'handler'=>'community_tags_views_handler_sort_tag_count',
        ),

      ),
      'ct_last_tagged' => array (
        'title' => t('Last Tagged'),
        'help' => t('Datetime of the most recent tagging.'),
        'field' => array(
          'handler' => 'community_tags_views_handler_field_last_tagged',
          'click sortable' => TRUE,
        ),
        'sort'=> array (
          'handler'=>'community_tags_views_handler_sort_last_tagged',
        ),
      ),
      'ct_first_tagged' => array (
        'title' => t('First Tagged'),
        'help' => t('Datetime of the least recent tagging.'),
        'field' => array(
          'handler' => 'community_tags_views_handler_field_first_tagged',
          'click sortable' => TRUE,
        ),
        'sort'=> array (
          'handler'=>'community_tags_views_handler_sort_first_tagged',
        ),
      ),
      'ct_tags' => array (
        'title' => t('Tags'),
        'help' => t('List of first n tags.'),
        'field' => array(
          'handler' => 'community_tags_views_handler_field_tags',
          'click sortable' => TRUE,
        ),
      ),
      'ct_tag_op' => array (
        'title' => t('Tag Op'),
        'help' => t('Tag operation.'),
        'field' => array(
          'handler' => 'community_tags_views_handler_field_tag_op',
          // 'click sortable' => TRUE,
        ),
      ),
      'uid'=> array (
        'title'=>t('Tagging User'),
        'help'=>t('The UID of the user who created the tag instance'),
        'argument'=> array (
          'handler'=>'views_handler_argument_user_uid',
          'name field'=>'name',
          ),
        ),
      'nid'=> array (
         'title'=>t('Tagged Node'),
         'help'=>t('The node that was tagged'),
         'argument'=> array (
           'handler'=>'views_handler_argument_node_nid',
         ),
       ),
       'tid'=> array (
         'title'=>t('Tag ID'),
         'help'=>t('The TID of the Tag'),
         'argument'=> array (
           'handler' => 'views_handler_argument_numeric',
           // 'skip base' => array('node', 'node_revision'),
           // 'zero is null' => TRUE,
           ),
         ),
    ),
  );

  return $data;
}

/**
 * Implementation of hook_views_handlers() to register all of the basic handlers
 * views uses.
 */
function community_tags_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'community_tags_views') . '/views/handlers',
    ),
    'handlers' => array(
      'community_tags_views_handler_field_tag_count' => array(
        'parent' => 'views_handler_field_numeric',
      ),
      'community_tags_views_handler_field_last_tagged' => array(
        'parent' => 'views_handler_field_date',
      ),
      'community_tags_views_handler_field_first_tagged' => array(
        'parent' => 'views_handler_field_date',
      ),
      'community_tags_views_handler_sort_tag_count' => array(
        'parent' => 'views_handler_sort',
      ),
      'community_tags_views_handler_sort_last_tagged' => array(
        'parent' => 'views_handler_sort_date',
      ),
      'community_tags_views_handler_sort_first_tagged' => array(
        'parent' => 'views_handler_sort_date',
      ),
      'community_tags_views_handler_field_tags' => array(
        'parent' => 'views_handler_field_prerender_list',
      ),
      // 'community_tags_views_handler_field_tag_op' => array(
      //   'parent' => 'views_handler_field',
      // ),
    ),
  );
}

/**
 * Implementation of hook_views_plugins
 */
function community_tags_views_plugins() {
  return array(
    'module' => 'community_tags',
    'display' => array(
      'attachment_tagform' => array(
        'title' => t('Community tagging form attachment'),
        'help' => t('Adds the community tagging form attachment and optionally ajaxifies the tags presented in the view.'),
        'handler' => 'community_tags_views_plugin_attachment_tagform',
        'parent' => 'attachment',
        'theme' => 'views_view',
        'use ajax' => TRUE,
        'help topic' => 'display-attachment',
        'path' => drupal_get_path('module', 'community_tags_views') . '/views/plugins'
      ),
    ),
  );
}

