<?php

/**
 * @file
 * community_tags.admin.inc
 *
 * Admin forms and supporting functions.
 */

/**
 * Main settings page.
 */
function community_tags_settings() {

  // community_tags_rehash();
  if($broken = _community_tags_get_count_of_broken_tags()) {
    drupal_set_message(t('There are @count broken community tags that reference missing terms, nodes, or users. Click <a href="@url">here</a> to delete them.', array('@count' => $broken, '@url' => url('admin/settings/community-tags/ops/broken'))), 'warning');
  }

   // Build list of available free-tagging vocabularies
  $vocabularies = _community_tags_get_settings();

  // stat query for all ctags even for invalid combos - e.g. unassigned content type
  $counts = _community_tags_get_tag_counts_by_type_and_vocabulary();

  // put tag counts into vocabularies array. If a new type is found for a vocabulary
  // then tags are left over from a previous configuration - i.e. content type was assigned for tagging but now is not.
  foreach($counts as $vid => $counts_by_type) {
    foreach($counts_by_type as $type => $count) {
      if(!isset($vocabularies[$vid]['types'][$type])) {
        $content_type = node_get_types('type', $type);
        $vocabularies[$vid]['types'][$type] = array('type_name' => $content_type->name, 'tag_count' => $count->tag_count, 'assigned' => FALSE);
      }
      else {
        $vocabularies[$vid]['types'][$type]['tag_count'] = $count->tag_count;
      }
    }
  }

  return theme('community_tags_settings', $vocabularies);
}

/**
 * Theme function for community tags settings.
 */
function theme_community_tags_settings($vocabularies) {
  $rows = array();
  $header_rows = array();


  $first_row = TRUE;
  $first_type_row = TRUE;

  $vocab_index = 0;
  $type_column_count = 1;

  $header_rows[] = array(
    array('data' => t('Vocabulary'), 'rowspan' => 2),
    array('data' => t('Free-tagging?'), 'rowspan' => 2),
    array('data' => t('Assigned content types'), 'colspan' => 6),
  );
  $header_rows[] = array(
    array('data' => t('Type')),
    array('data' => t('Enabled?')),
    array('data' => t('Tag count')),
    array('data' => t('Status')),
    array('data' => t('Operations')),
    array('data' => t('Settings')),
  );

  foreach ($vocabularies as $vid => $vocabulary) {
    // content types enabled for this vocabulary - may not be any
    $types = $vocabulary['types'];
    $content_type_count = count($types);
    $rowspan = $content_type_count == 0 ? 1 : $content_type_count;

    // stripe per vocabulary
    $row_class = ($vocab_index++ % 2) == 0 ? 'odd' : 'even';

    // First column 'vocabulary' - if first pass process header - get description if any
    $row_data = array();
    $row_data[] = array(
      'data' => l($vocabulary['name'], 'admin/content/taxonomy/edit/vocabulary/'.$vid, array('query' => drupal_get_destination())),
      'rowspan' => $rowspan,
    );
    $row_data[] = array(
      'data' => $vocabulary['tagging'] ? t('yes') : t('no'),
      'rowspan' => $rowspan,
    );

    // Vocabulary level columns - if first pass process header - get descriptions if any
    if(!empty($vocabulary['types'])) {
      foreach($vocabulary['types'] as $type_id => $type) {
        $row_data[] = array('data' => l($type['type_name'], 'admin/content/node-type/'.$type_id, array('query' => drupal_get_destination())));
        $row_data[] = array('data' => $type['enabled'] ? t('yes') : t('no'));
        $row_data[] = array('data' => $type['tag_count']);

        // status
        $statuses = array();

        if($type['enabled']) {
          $statuses = module_invoke_all('community_tags_admin_status', $vid, $type_id);
        }

        $row_data[] = empty($statuses) ? t('OK') : array('data' => implode('; ', $statuses));

        // operations
        $operations = array();

        if($type['enabled']) {
          $operations = module_invoke_all('community_tags_admin_operations', $vid, $type_id);
        }

        if (isset($type['tag_count']) && $type['tag_count'] > 0) {
          // operations for any CT vocabulary that has tags
          $operations[] = array('title' => t('purge'), 'href' => "admin/settings/community-tags/ops/purge/$vid/$type_id", 'query' => drupal_get_destination());
        }

        $row_data[] = array('data' => theme('links', $operations, array('class' => 'links inline')));
        $row_data[] = array('data' => l(t('edit'), "admin/settings/community-tags/$vid/$type_id", array('query' => drupal_get_destination())));

        $rows[] = array('data' => $row_data, 'class' => $row_class);
        $row_data = array();
      }
    }
    else {
      $row_data[] = array('data' => t('No assigned content types.'), 'colspan' => 6);
      $rows[] = array('data' => $row_data, 'class' => $row_class);
    }
  }

  // output the table - don't use standard theme because want bespoke striping
  $output .= '<table><thead>';
  foreach($header_rows as $header_row) {
    $output .= '<tr>';
    foreach($header_row as $header_cell) {
      $output .= _theme_table_cell($header_cell, TRUE);
    }
    $output .= '</tr>';
  }
  $output .= '</thead><tbody>';
  foreach($rows as $row) {
    $output .= '<tr class="'.$row['class'].'">';
    foreach($row['data'] as $data_cell) {
      $output .= _theme_table_cell($data_cell);
    }
    $output .= '</tr>';
  }
  $output .= '</tbody></table>';

  return $output;
}

/**
 * Form builder; Builds the sub settings form.
 *
 * @ingroup forms
 */
function community_tags_sub_settings(&$form_state, $vocabulary, $content_type) {
  $form = array();
  $type_obj = node_get_types('type', $content_type);

  $form['#vid'] = $vid = $vocabulary->vid;
  $form['#content_type'] = $content_type;

  $settings = _community_tags_get_settings($vocabulary->vid, $content_type);

  // stat query for all ctags even for invalid combos - e.g. unassigned content type
  $counts = _community_tags_get_tag_counts_by_type_and_vocabulary();
  $settings['tag_count'] = $counts[$vid][$content_type];

  $display_handler_options = _community_tags_get_display_handler_options();

  $form['core'] = array(
    '#tree' => TRUE,
  );

  $form['core']['enabled'] = array(
    '#title' => t('Enable community tagging'),
    '#type' => 'checkbox',
    '#default_value' => $settings['enabled'],
  );

  $form['core']['display_handler'] = array(
    '#title' => t('Display'),
    '#type' => 'select',
    '#default_value' => isset($display_handler_options[$settings['display_handler']]) ? $settings['display_handler'] : 'links',
    '#options' => $display_handler_options,
  );
  $form['core']['redundant_terms'] = array(
    '#title' => t('Delete redundant terms'),
    '#type' => 'checkbox',
    '#default_value' => $settings['opmode'] & COMMUNITY_TAGS_OPMODE_DELETE_ORPHAN_TERMS,
    '#description' => t('When checked, terms that are no longer used as a result of a community tag being removed are deleted.'),
  );
  $form['core']['tag_links'] = array(
    '#title' => t('Show "Tag this" link on'),
    '#type' => 'checkboxes',
    '#options' => array('teaser' => t('Node teasers'), 'full' => t('Full nodes')),
    '#default_value' => $settings['tag_links'] ? $settings['tag_links'] : array(),
  );

  $form['plugins'] = array(
    '#tree' => TRUE,
  );

  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Title callback for sub-settings configuration page.
 */
function _community_tags_sub_settings_title($arg1, $arg2) {
  $type = node_get_types('type', $arg2);
  return t('Community tags settings for @vocabulary on content of type @content_type', array('@vocabulary' => $arg1->name, '@content_type' => $type->name));
}

/**
 * Submit handler for sub settings form.
 */
function community_tags_sub_settings_submit($form, &$form_state) {
  $settings = _community_tags_get_settings(NULL, NULL);

  $type_settings = $form_state['values']['core'];

  $type_settings['opmode'] = 0x00;
  $type_settings['opmode'] |= $type_settings['redundant_terms'] ? COMMUNITY_TAGS_OPMODE_DELETE_ORPHAN_TERMS : 0x00;
  unset($type_settings['redundant_terms']);

  $settings[$form['#vid']]['types'][$form['#content_type']] = $type_settings;

  variable_set('community_tags_vocabularies', $settings);
}

/**
 * "Private" implementation of hook_form_alter().
 */
function _community_tags_node_type_form_alter(&$form, &$form_state, $form_id) {
  // only show if content type is mapped to tagging vocabulary
  $supported_vids = _community_tags_vids_for_node_type($form['#node_type']->type);
  $type_settings = community_tags_get_content_type_settings($form['#node_type']->type);
  if(!empty($supported_vids)) {
    $form['community_tags'] = array(
      '#type' => 'fieldset',
      '#title' => 'Community tag settings',
      '#tree' => TRUE,
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $modes = array(
      COMMUNITY_TAGS_MODE_BLOCK => t('Block'),
      COMMUNITY_TAGS_MODE_TAB => t('Tab'),
      COMMUNITY_TAGS_MODE_INLINE => t('Inline'),
      COMMUNITY_TAGS_MODE_NONE => t('None'),
    );
    $form['community_tags']['community_tags_display'] = array(
      '#type' => 'radios',
      '#title' => t('Community tagging form'),
      '#default_value' => variable_get('community_tags_display_'. $form['#node_type']->type, COMMUNITY_TAGS_MODE_TAB),
      '#options' => $modes,
      '#description' => t('How should users be allowed to tag content?')
    );
    $hide_terms_modes = array(
      'none' => t('No override'),
      'hide' => t('Hide terms'),
      'override' => t('Replace with community tags'),
    );
  // COMMUNITY_TAGS_MODE_HIDE_TERMS_PAGE => t('Page'),
  // COMMUNITY_TAGS_MODE_HIDE_TERMS_TEASER => t('Teaser'),

    $form['community_tags']['community_tags_terms_page'] = array(
      '#type' => 'select',
      '#title' => t('Node page term links display override'),
      '#options' => $hide_terms_modes,
      '#default_value' => $type_settings['community_tags_terms_page'],
      // '#description' => t('Hide standard taxonomy term display?')
    );
    $form['community_tags']['community_tags_terms_teaser'] = array(
      '#type' => 'select',
      '#title' => t('Node teaser term links display override'),
      '#options' => $hide_terms_modes,
      '#default_value' => $type_settings['community_tags_terms_teaser'],
      // '#description' => t('Hide standard taxonomy term display?')
    );
    array_unshift($form['#submit'], 'community_tags_node_type_form_submit');
  }
}

function community_tags_node_type_form_submit($form, &$form_state) {
  $settings = community_tags_get_content_type_settings();
  $settings[$form['#node_type']->type] = $form_state['values']['community_tags'];
  unset($form_state['values']['community_tags']);
  variable_set('community_tags_content_types', $settings);
}

/**
 * Get tag counts by vocabulary and type.
 */
function _community_tags_get_tag_counts_by_type_and_vocabulary() {
  $counts = array();

  // stat query for all ctags even for invalid combos - e.g. unassigned content type
  $tag_count_result = db_query(
    'SELECT td.vid, n.type, count(*) tag_count, count(distinct ct.nid) node_count, count(distinct ct.tid) term_count, count(distinct ct.uid) user_count
     FROM {community_tags} ct
     INNER JOIN {term_data} td ON td.tid = ct.tid
     INNER JOIN {node} n ON n.nid = ct.nid
     GROUP BY td.vid, n.type');

  // put tag counts into vocabularies array. If a new type is found for a vocabulary
  // then tags are left over from a previous configuration - i.e. content type was assigned for tagging but now is not.
  while ($row = db_fetch_object($tag_count_result)) {
    $counts[$row->vid][$row->type] = $row;
  }

  return $counts;
}

/**
 * Get the number of broken tags.
 *
 * @return
 *  The number of broken tags.
 */
function _community_tags_get_count_of_broken_tags() {
  $result = db_query(
    'SELECT count(*) FROM {community_tags} ct
     LEFT JOIN {term_data} td ON td.tid = ct.tid
     LEFT JOIN {users} u ON u.uid = ct.uid
     LEFT JOIN {node} n ON n.nid = ct.nid
     WHERE td.tid IS NULL
     OR u.uid IS NULL
     OR n.nid IS NULL');

  return db_result($result);
}

/**
 * Confirmation for deleting broken tags.
 */
function community_tags_delete_broken_tags_form() {
  $form = array();
  $question = t('Delete %count broken community tags?', array('%count' => $broken_ctag_count));
  $path = 'admin/settings/community-tags';
  $confirm_form = confirm_form($form, $question, $path);
  return $confirm_form;
}

/**
 * Submit handler for broken tag deletion form.
 */
function community_tags_delete_broken_tags_form_submit($form, &$form_state) {
  db_query(
    'DELETE FROM ct
     USING {community_tags} ct
     LEFT JOIN {term_data} td ON td.tid = ct.tid
     LEFT JOIN {users} u ON u.uid = ct.uid
     LEFT JOIN {node} n ON n.nid = ct.nid
     WHERE td.tid IS NULL
     OR u.uid IS NULL
     OR n.nid IS NULL'
  );

  $deleted = db_affected_rows();

  drupal_set_message(t('@count broken community tags deleted.', array('@count' => $deleted)));
  $form_state['redirect'] = "admin/settings/community-tags";
}

/**
 * Confirmation form for tag purge. Assumes defaults.
 */
function community_tags_delete_all_form(&$form_state, $vocabulary, $content_type) {
  $form = array();

  $type_obj = node_get_types('type', $content_type);

  // get the number of tags that will be deleted
  $counts = _community_tags_get_tag_counts_by_type_and_vocabulary();
  if(empty($counts[$vocabulary->vid][$content_type])) {
    drupal_set_message(t('There are no community tags to delete.'));
  }
  else {
    $counts = $counts[$vocabulary->vid][$content_type];
  }


  // get settings if valid
  $settings = _community_tags_get_settings($vocabulary->vid, $content_type, TRUE);

  $form['#delete_ops'] = array();
  $form['#ct_settings'] = $settings;
  $form['#vocabulary'] = $vocabulary;
  $form['#content_type_obj'] = $type_obj;
  $form['#counts'] = $counts;

  $question = t('Delete %count community tags from the %vocabulary vocabulary on nodes of type %type?.', array('%count' => $counts->tag_count, '%vocabulary' => $vocabulary->name, '%type' => $type_obj->name));

  // @todo 2.x get the number of node terms that will be deleted
  // @todo 2.x get the number of orphaned terms that will be deleted

  $description = '';
  if($settings) {
    $form['#delete_ops']['delete_tags'] = TRUE;
    $description .= '<p>'.t('WARNING: Community tagging is currently enabled for this combination of content type and vocabulary. ').'</p>';
    $description = '<p>'.t('Community tags will be deleted from the community tags database table.').'</p>';

    if($settings['opmode'] & COMMUNITY_TAGS_OPMODE_DELETE_ORPHAN_TERMS) {
      $form['delete_policy'] = array(
        '#type' => 'checkbox',
        '#title' => t('Delete redundant terms'),
        '#weight' => 1,
        '#default_value' => COMMUNITY_TAGS_OPMODE_DELETE_ORPHAN_TERMS,
        '#description' => t('"Delete redundant term" mode is set. All redundant terms will be deleted i.e. all terms matching deleted tags that are no longer used. If you do not want to delete redundant terms as part of this purge operation, un-check this below.'),
      );
    }

  }
  else {
    $form['#delete_ops']['#quick_delete'] = TRUE;
    $description = '<p>'.t('Community tagging is currently disabled for this combination of content type and vocabulary. ').'</p>';
    $description = '<p>'.t('Community tags will be deleted from the community tags database table and nothing else. ').'</p>';
  }

  $path = 'admin/settings/community-tags';

  $confirm_form = confirm_form($form, $question, $path, $description);
  $confirm_form['actions']['#weight'] = 5;
  return $confirm_form;
}

/**
 * Submit handler for community_tags_delete_all_form().
 */
function community_tags_delete_all_form_submit($form, &$form_state) {
  $vid = $form['#vocabulary']->vid;
  $content_type = $form['#content_type_obj']->type;
  $settings = $form['#ct_settings'];

  if(isset($form['#delete_ops']['#quick_delete'])) {
    db_query(
      "DELETE FROM ct
       USING {community_tags} ct, {term_data} AS td, {node} AS n
       WHERE ct.tid = td.tid
       AND td.vid = %d
       AND ct.nid = n.nid
       AND n.type = '%s'", $vid, $content_type);

    drupal_set_message(t('@delete_count community tags deleted.', array('@delete_count' => db_affected_rows())));
  }
  else {
    // do batched delete
    $operations = array();

    $mode = $form['#ct_settings']['opmode'];

    // set up the tag delete operations
    // remove mode flags for unchecked mode options
    if($settings['opmode'] & COMMUNITY_TAGS_OPMODE_DELETE_ORPHAN_TERMS && empty($form_state['values']['delete_policy'])) {
      // unset
      $mode = $settings['opmode'] &= ~COMMUNITY_TAGS_OPMODE_DELETE_ORPHAN_TERMS;
    }

    $config = array('ct_settings' => array('opmode' => $mode));
    $operations[] = array('community_tags_delete_tags_batch_process', array($vid, $content_type, $form['#counts'], $mode));
    $batch = array(
      'operations' => $operations,
      'finished' => 'community_tags_batch_finished',
      'title' => t('Processing Community Tags Delete Batch'),
      'init_message' => t('Community Tags Delete Batch is starting.'),
      // 'progress_message' => t('Processed @current out of @total.'),
      'error_message' => t('Community Tags Delete Batch has encountered an error.'),
      'file' => drupal_get_path('module', 'community_tags') . '/community_tags.batch.inc',
    );
    batch_set($batch);
  }

}
