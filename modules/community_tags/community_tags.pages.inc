<?php

/**
 * @file
 * community_tags.pages.inc
 *
 * Page handlers of Community Tags.
 *
 * @defgroup community_tags_pages Community Tags page handlers.
 * @{
 */

/**
 * Quick tag form.
 */
function community_tags_form($form_state, $edit, $title = NULL) {
  $form['#attributes'] = array('class' => 'community-tags-form');
  $vocabulary = taxonomy_vocabulary_load($edit['vid']);

  if($edit['cloud']) {
    if($edit['multiple'] > 1) {
      // displaying more than 1 form so need to differentiate in headings
      $all_title = t('All !name tags', array('!name' => $vocabulary->name));
    }
    else {
      $all_title = t('All tags');
    }

    // attributes doesn't apply to type of item so wrap in a div to set class
    $form['cloud'] = array(
      '#prefix' => '<div class="ct-all-tags">',
      '#type' => 'item',
      '#title' => $all_title,
      '#value' => '<div class="ct-cloud">'. $edit['cloud'] .'</div>',
      '#suffix' => '</div>',
    );
  }

  // anonymous tagging not supported
  $access = user_is_logged_in() && user_access('tag content');

  if($edit['multiple'] > 1) {
    // displaying more than 1 form so need to differentiate in headings
    $my_title = t('My !name tags', array('!name' => $vocabulary->name));
  }
  else {
    $my_title = t('My tags');
  }


  $form['tags'] = array(
    '#prefix' => '<div class="ct-my-tags">',
    '#type' => 'textfield',
    '#title' => $my_title,
    '#maxlength' => 100,
    '#default_value' => $edit['tags'],
    '#required' => FALSE,
    '#autocomplete_path' => 'taxonomy/autocomplete/'. $edit['vid'],
    '#attributes' => array('class' => 'form-tags form-tags-'. $edit['vid']),
    '#access' => $access,
    '#suffix' => '</div>',
  );

  if ($edit['inline']) {
    $form['tags']['#size'] = 20;
  }

  if (!$access) {
    $destination = drupal_get_destination();
    $form['login'] = array('#type' => 'markup');
    if(variable_get('user_register', true)) {
      $form['login']['#value'] = '<div>'. t('<a href="@login">Login</a> or <a href="@register">register</a> to tag items', array('@login' => url('user/login', array('query' => $destination)), '@register' => url('user/register', array('query' => $destination)))) .'</div>';
    }
    else {
      $form['login']['#value'] = '<div>'. t('<a href="@login">Login</a> to tag items', array('@login' => url('user/login', array('query' => $destination)))) .'</div>';
    }
  }
  else {
    // supply this as we
    $form['tags_refs'] = array(
      '#type' => 'hidden',
      '#value' => $edit['tags'],
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#access' => $access,
  );

  $form['node'] = array(
    '#type' => 'value',
    '#value' => $edit['node'],
  );

  $form['nid'] = array(
    '#type' => 'hidden',
    '#title' => t('NID'),
    '#value' => $edit['nid'],
  );

  $form['vid'] = array(
    '#type' => 'hidden',
    '#default_value' => $edit['vid'],
  );

  if($edit['links_container_class']) {
    $form['links_container_class'] = array(
      '#type' => 'hidden',
      '#value' => $edit['links_container_class'],
    );
  }

  if($edit['source']) {
    $form['source'] = array(
      '#type' => 'hidden',
      '#value' => $edit['source'],
    );
  }

  if(!empty($edit['extra'])) {
    foreach($edit['extra'] as $name => $value) {
      $form[$name] = array(
        '#type' => 'hidden',
        '#value' => $value,
      );
    }
  }

  return $form;
  // drupal_add_js(array('communityTags' => array('n_'. $node->nid => array('v_'. $vid => array('tags' => $names, 'url' => url('community-tags/js/'. $node->nid .'/'. $vid), 'add' => t('Add'), 'token' => drupal_get_token('community_tags_form'))))), 'setting');

}

/**
 * Submit callback for quick tag form.
 */
function community_tags_form_submit($form, &$form_state) {
  global $user;
  community_tags_taxonomy_node_save($form_state['values']['node'], array('tags' => array($form_state['values']['vid'] => $form_state['values']['tags'])), FALSE, $user->uid);

  $form_state['redirect'] = 'node/'. $form_state['values']['nid'];
}

/**
 * Theme the quick tag form.
 * @ingroup themeable
 */
function theme_community_tags_form($form) {
  $output .= drupal_render($form);

  _community_tags_add_js();
  _community_tags_add_css();

  return $output;
}

/**
 * Menu callback:
 */
function community_tags_mypage($uid = NULL) {
  $uid = isset($uid) ? $uid : $GLOBALS['uid'];

  _community_tags_get_tag_result('user', 100, $uid);
}

/**
 * Retrieve list of tags for a given node.
 *
 * @return
 *  Array of objects {tid, name, tag_count} keyed on tid.
 */
function _community_tags_get_node_tags_for_mgmt($nid, $sort_clause, $pager_limit = 50, $vids = NULL) {
  $tags = array();
  if($vids) {
    $args = $vids;
    array_unshift($args, $nid);
    $sql = "SELECT t.tid, t.vid, t.name, count(t.tid) tag_count, min(c.date) earliest_tag, max(c.date) latest_tag FROM {term_data} t INNER JOIN {community_tags} c ON c.tid = t.tid WHERE c.nid = %d AND t.vid IN (".db_placeholders($vids, 'varchar').") GROUP BY t.tid ";
    $count_sql = "SELECT count(disctinct t.tid) FROM {term_data} t INNER JOIN {community_tags} c ON c.tid = t.tid WHERE c.nid = %d AND t.vid IN (".db_placeholders($vids, 'varchar').") ";
  }
  else {
    $args = array($nid);
    $sql = "SELECT t.tid, t.vid, t.name, count(t.tid) tag_count, min(c.date) earliest_tag, max(c.date) latest_tag FROM {term_data} t INNER JOIN {community_tags} c ON c.tid = t.tid WHERE c.nid = %d GROUP BY t.tid ";
    $count_sql = "SELECT count(distinct t.tid) FROM {term_data} t INNER JOIN {community_tags} c ON c.tid = t.tid WHERE c.nid = %d ";
  }

  // $result = db_query($sql . $sort_clause, $args);
  $result = pager_query($sql . $sort_clause, $pager_limit, 0, $count_sql, $args);

  while ($term = db_fetch_object($result)) {
    $tags[$term->tid] = $term;
  }

  return $tags;
}


/*****************************************************************************
 * Tag management page.
 ****************************************************************************/
/**
 * Tag management page view.
 */
function community_tags_node_tag_management($node) {
  $output .= '<p>'.t("The tag management operations below apply to all users' tags. ").'</p>';

  $output .= drupal_get_form('community_tags_node_tag_mgmt_form', $node);

  return $output;
}

/**
 * Build tag management form.
 */
function community_tags_node_tag_mgmt_form(&$form_state, $node) {
  if (isset($form_state['values']['operation']) && $form_state['values']['confirm_operation']) {
    return community_tags_node_tag_mgmt_confirm($form_state, $form_state['values']['node'], array_filter($form_state['values']['tags']), $form_state['values']['operation']);
  }

  $form = array();

  $form['#table_header'] = array(
    array('data' => t('Term'), 'field' => 'name'),
    array('data' => t('Count'), 'field' => 'tag_count'),
    array('data' => t('Operations')),
    array('data' => t('First tagged'), 'field' => 'earliest_tag'),
    array('data' => t('Last tagged'), 'field' => 'latest_tag', 'sort' => 'desc'),
  );

  $form['#node'] = $node;

  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Tag options'),
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );
  $options = array('remove' => t('Remove'));
  $confirm_required = array('remove' => TRUE);
  $form['options']['operation'] = array(
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => 'remove',
    '#confirm_required' => $confirm_required,
  );
  $form['options']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
    '#submit' => array('community_tags_node_tag_mgmt_form_submit'),
  );

  $tablesort = tablesort_sql($form['#table_header']);
  $ctags = _community_tags_get_node_tags_for_mgmt($node->nid, $tablesort, 50, null);

  $now = time();

  foreach($ctags as $ctag) {
    $first_tagged = $now-$ctag->earliest_tag;
    $last_tagged = $now-$ctag->latest_tag;
    $operations = array(
      array(
        'title' => t('Remove'),
        'href' => 'community-tags/tagmgmtops/remove/'.$node->nid.'/'.$ctag->tid,
        'query' => drupal_get_destination()
      ),
    );


    $tags[$ctag->tid] = '';
    $form['name'][$ctag->tid] = array('#value' => l($ctag->name, 'taxonomy/term/'.$ctag->tid));
    $form['count'][$ctag->tid] =  array('#value' => $ctag->tag_count);
    $form['operations'][$ctag->tid] = array('#value' => theme_links($operations), '#operations' => $operations);
    $form['first_tagged'][$ctag->tid] =  array('#value' => format_interval($first_tagged));
    $form['last_tagged'][$ctag->tid] =  array('#value' => format_interval($last_tagged));
  }
  $form['tags'] = array('#type' => 'checkboxes', '#options' => $tags);

  $form['pager'] = array('#value' => theme('pager', NULL, 50, 0));
  $form['#theme'] = 'community_tags_tag_mgmt';

  return $form;
}

/**
 * Validate node_admin_nodes form submissions.
 *
 * Check if any nodes have been selected to perform the chosen
 * 'Update option' on.
 */
function community_tags_node_tag_mgmt_form_validate($form, &$form_state) {
  if($form_state['values']['tags'] && is_array($form_state['values']['tags'])) {
    $tags = array_filter($form_state['values']['tags']);
    if (count($tags) == 0) {
      form_set_error('', t('No tags selected.'));
    }
  }
}

/**
 * Process node_admin_nodes form submissions.
 *
 * Execute the chosen 'Update option' on the selected nodes.
 */
function community_tags_node_tag_mgmt_form_submit($form, &$form_state) {
  $operation = $form_state['values']['operation'];

  $form_state['values']['node'] = $form['#node'];
  // if(isset($form['options']['operation']['#confirm_required'][$operation])) {
    $form_state['values']['confirm_operation'] = TRUE;
    $form_state['rebuild'] = TRUE;
  // }
}


/**
 * Theming for tag management table.
 */
function theme_community_tags_tag_mgmt($form) {
  $has_tags = isset($form['name']) && is_array($form['name']);
  $select_header = $has_tags ? theme('table_select_header_cell') : '';
  $table_header = $form['#table_header'];
  array_unshift($table_header, $select_header);

  $output .= drupal_render($form['options']);

  if($has_tags) {
    foreach (element_children($form['name']) as $key) {
      $row = array();
      $row[] = drupal_render($form['tags'][$key]);
      $row[] = drupal_render($form['name'][$key]);
      $row[] = drupal_render($form['count'][$key]);
      $row[] = drupal_render($form['operations'][$key]);
      $row[] = drupal_render($form['first_tagged'][$key]);
      $row[] = drupal_render($form['last_tagged'][$key]);
      $rows[] = $row;
    }
  }
  else {
    $rows[] = array(array('data' => t('No tags.'), 'colspan' => '6'));
  }

  $output .= theme('table', $table_header, $rows);

  if ($form['pager']['#value']) {
    $output .= drupal_render($form['pager']);
  }

  $output .= drupal_render($form);


  return $output;
}

/**
 * Confirmation form for tag mgmt operation.
 */
function community_tags_node_tag_mgmt_confirm(&$form_state, $node, $tid, $operation) {
  $tids = is_array($tid) ? $tid : array($tid);
  if($operation == 'remove') {
    $question = t('Remove the following tags from %node?', array('%node' => $node->title));
  }
  elseif($operation) {
    $question = t('Perform "%op" operation on the following tags?', array('%op' => $operation));
  }
  else {
    return FALSE;
  }

  $term_names = array();
  foreach($tids as $tid) {
    $term = taxonomy_get_term($tid);
    $term_names[] = $term->name;
  }

  $description = theme('item_list', $term_names);

  $form = array('#ct_op' => $operation, '#node' => $node, '#tids' => drupal_map_assoc($tids));
  $form['#submit'][] = 'community_tags_node_tag_mgmt_confirm_submit';

  $path = 'node/'.$node->nid.'/tagmgmt';
  $confirm_form = confirm_form($form, $question, $path, $description);
  return $confirm_form;
}

/**
 * Submit handler for tag mgmt operation confirm form.
 */
function community_tags_node_tag_mgmt_confirm_submit($form, &$form_state) {
  if($form['#ct_op'] == 'remove') {
    // do full service tag removal - update node if necesssary etc
    $tags_deleted = community_tags_remove_tags($form['#node'], NULL, $form['#tids']);
    drupal_set_message(t('%count tags removed.', array('%count' => $tags_deleted)));
  }
}

