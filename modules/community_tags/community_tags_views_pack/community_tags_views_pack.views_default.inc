<?php
// $Id$

/**
 * Implementation of hook_views_default_views().
 */
function community_tags_views_pack_views_default_views() {
  /*
   * View 'ct_tags_by_node'
   */
  $view = new view;
  $view->name = 'ct_tags_by_node';
  $view->description = 'List of tagged nodes with counts etc for user';
  $view->tag = 'Community Tags';
  $view->view_php = '';
  $view->base_table = 'node';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('fields', array(
    'title' => array(
      'label' => 'Content',
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
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_node' => 1,
      'exclude' => 0,
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'relationship' => 'none',
    ),
    'ct_tag_count' => array(
      'label' => 'Tag Count',
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
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'set_precision' => FALSE,
      'precision' => 0,
      'decimal' => '.',
      'separator' => ',',
      'prefix' => '',
      'suffix' => '',
      'exclude' => 0,
      'id' => 'ct_tag_count',
      'table' => 'community_tags_aggregates',
      'field' => 'ct_tag_count',
      'relationship' => 'none',
      'override' => array(
        'button' => 'Override',
      ),
    ),
    'ct_first_tagged' => array(
      'label' => 'First Tagged',
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
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'date_format' => 'raw time ago',
      'custom_date_format' => '',
      'exclude' => 0,
      'id' => 'ct_first_tagged',
      'table' => 'community_tags_aggregates',
      'field' => 'ct_first_tagged',
      'relationship' => 'none',
    ),
    'ct_last_tagged' => array(
      'label' => 'Last Tagged',
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
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'date_format' => 'raw time ago',
      'custom_date_format' => '',
      'exclude' => 0,
      'id' => 'ct_last_tagged',
      'table' => 'community_tags_aggregates',
      'field' => 'ct_last_tagged',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('sorts', array(
    'ct_first_tagged' => array(
      'order' => 'ASC',
      'granularity' => 'hour',
      'id' => 'ct_first_tagged',
      'table' => 'community_tags_aggregates',
      'field' => 'ct_first_tagged',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('arguments', array(
    'uid' => array(
      'default_action' => 'ignore',
      'style_plugin' => 'default_summary',
      'style_options' => array(),
      'wildcard' => 'all',
      'wildcard_substitution' => 'All',
      'title' => '',
      'breadcrumb' => '',
      'default_argument_type' => 'fixed',
      'default_argument' => '',
      'validate_type' => 'none',
      'validate_fail' => 'not found',
      'break_phrase' => 0,
      'not' => 0,
      'id' => 'uid',
      'table' => 'community_tags_aggregates',
      'field' => 'uid',
      'validate_user_argument_type' => 'uid',
      'validate_user_roles' => array(
        '2' => 0,
      ),
      'relationship' => 'none',
      'default_options_div_prefix' => '',
      'default_argument_fixed' => '',
      'default_argument_user' => 0,
      'default_argument_php' => '',
      'validate_argument_node_type' => array(
        'story' => 0,
      ),
      'validate_argument_node_access' => 0,
      'validate_argument_nid_type' => 'nid',
      'validate_argument_vocabulary' => array(
        '2' => 0,
        '7' => 0,
        '3' => 0,
        '4' => 0,
        '1' => 0,
        '5' => 0,
        '6' => 0,
      ),
      'validate_argument_type' => 'tid',
      'validate_argument_transform' => 0,
      'validate_user_restrict_roles' => 0,
      'validate_argument_php' => '',
      'override' => array(
        'button' => 'Override',
      ),
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'none',
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));
  $handler->override_option('title', 'Your tagged content');
  $handler->override_option('items_per_page', 30);
  $handler->override_option('use_pager', '1');
  $handler->override_option('style_plugin', 'table');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'override' => 1,
    'sticky' => 1,
    'order' => 'desc',
    'columns' => array(
      'title' => 'title',
      'ct_tag_count' => 'ct_tag_count',
      'ct_first_tagged' => 'ct_first_tagged',
      'ct_last_tagged' => 'ct_last_tagged',
    ),
    'info' => array(
      'title' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'ct_tag_count' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'ct_first_tagged' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'ct_last_tagged' => array(
        'sortable' => 1,
        'separator' => '',
      ),
    ),
    'default' => 'ct_tag_count',
  ));
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->override_option('path', 'user/%/tags/content');
  $handler->override_option('menu', array(
    'type' => 'tab',
    'title' => 'Tagged Content',
    'description' => '',
    'weight' => '0',
    'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'tab',
    'title' => 'Tags',
    'description' => '',
    'weight' => '0',
    'name' => 'navigation',
  ));
  $views[$view->name] = $view;

  /*
   * View 'ct_tags_by_term'
   */
  $view = new view;
  $view->name = 'ct_tags_by_term';
  $view->description = 'List of terms used for tagging with counts etc for user';
  $view->tag = 'Community Tags';
  $view->view_php = '';
  $view->base_table = 'term_data';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('fields', array(
    'name' => array(
      'label' => 'Term',
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
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_taxonomy' => 1,
      'exclude' => 0,
      'id' => 'name',
      'table' => 'term_data',
      'field' => 'name',
      'relationship' => 'none',
    ),
    'ct_tag_count' => array(
      'id' => 'ct_tag_count',
      'table' => 'community_tags_aggregates',
      'field' => 'ct_tag_count',
    ),
    'ct_last_tagged' => array(
      'label' => 'Last Tagged',
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
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'date_format' => 'raw time ago',
      'custom_date_format' => '',
      'exclude' => 0,
      'id' => 'ct_last_tagged',
      'table' => 'community_tags_aggregates',
      'field' => 'ct_last_tagged',
      'relationship' => 'none',
    ),
    'ct_first_tagged' => array(
      'label' => 'First Tagged',
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
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'date_format' => 'raw time ago',
      'custom_date_format' => '',
      'exclude' => 0,
      'id' => 'ct_first_tagged',
      'table' => 'community_tags_aggregates',
      'field' => 'ct_first_tagged',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('sorts', array(
    'ct_tag_count' => array(
      'order' => 'DESC',
      'id' => 'ct_tag_count',
      'table' => 'community_tags_aggregates',
      'field' => 'ct_tag_count',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('arguments', array(
    'uid' => array(
      'default_action' => 'not found',
      'style_plugin' => 'default_summary',
      'style_options' => array(),
      'wildcard' => 'all',
      'wildcard_substitution' => 'All',
      'title' => '',
      'breadcrumb' => '',
      'default_argument_type' => 'fixed',
      'default_argument' => '',
      'validate_type' => 'user',
      'validate_fail' => 'not found',
      'break_phrase' => 0,
      'not' => 0,
      'id' => 'uid',
      'table' => 'community_tags_aggregates',
      'field' => 'uid',
      'validate_user_argument_type' => 'uid',
      'validate_user_roles' => array(
        '2' => 0,
      ),
      'relationship' => 'none',
      'default_options_div_prefix' => '',
      'default_argument_fixed' => '',
      'default_argument_user' => 0,
      'default_argument_php' => '',
      'validate_argument_node_type' => array(
        'story' => 0,
      ),
      'validate_argument_node_access' => 0,
      'validate_argument_nid_type' => 'nid',
      'validate_argument_vocabulary' => array(
        '2' => 0,
        '7' => 0,
        '3' => 0,
        '4' => 0,
        '1' => 0,
        '5' => 0,
        '6' => 0,
      ),
      'validate_argument_type' => 'tid',
      'validate_argument_transform' => 0,
      'validate_user_restrict_roles' => 0,
      'validate_argument_php' => '',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'none',
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));
  $handler->override_option('title', 'Your Tags');
  $handler->override_option('items_per_page', 30);
  $handler->override_option('use_pager', '1');
  $handler->override_option('style_plugin', 'table');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'override' => 1,
    'sticky' => 0,
    'order' => 'desc',
    'columns' => array(
      'name' => 'name',
      'ct_last_tagged' => 'ct_last_tagged',
      'ct_first_tagged' => 'ct_first_tagged',
      'ct_tag_count' => 'ct_tag_count',
    ),
    'info' => array(
      'name' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'ct_last_tagged' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'ct_first_tagged' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'ct_tag_count' => array(
        'sortable' => 1,
        'separator' => '',
      ),
    ),
    'default' => 'ct_tag_count',
  ));
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->override_option('path', 'user/%/tags/terms');
  $handler->override_option('menu', array(
    'type' => 'default tab',
    'title' => 'Tag Terms',
    'description' => '',
    'weight' => '-1',
    'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'tab',
    'title' => 'Tags',
    'description' => '',
    'weight' => '0',
    'name' => 'navigation',
  ));
  $views[$view->name] = $view;

  return $views;
}


