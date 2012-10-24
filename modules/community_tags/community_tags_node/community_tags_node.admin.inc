<?php
// $Id$
/**
 * "Private" implementation of hook_form_FORM_ID_alter() for community_tags_sub_settings.
 */
function _community_tags_node_sub_settings(&$form, &$form_state) {
  $vid = $form['#vid'];
  $content_type = $form['#content_type'];

  $status = _community_tags_node_get_status($vid, $content_type);
  $settings = _community_tags_node_get_settings($vid, $content_type);

  $form['plugins']['nodeterm'] = array(
    '#type' => 'fieldset',
    '#title' => t('Node term synchronisation'),
  );

  $form['plugins']['nodeterm']['policy'] = array(
    '#type' => 'item',
    '#title' => t('Synchronisation Policy'),
  );

  $form['plugins']['nodeterm']['policy']['strict'] = array(
    '#title' => t('Strict synchronisation of node terms and community tags'),
    '#type' => 'radio',
    '#default_value' => $settings['policy'] == 'strict' ? 'strict' : FALSE,
    '#parents' => array('plugins', 'nodeterm', 'policy'),
    '#return_value' => 'strict',
    '#description' => t("When checked, the node is community tagged for all new terms added to the node (e.g. via the node edit form). Likewise, community tags are removed for any terms removed from the node. New community tags are attributed to the current user. Node terms are added for new community tags, and removed when the last community tag for the term is removed."),
  );

  $form['plugins']['nodeterm']['policy']['editor'] = array(
    '#title' => t('Synchronisation of node terms and community tags added or removed by node editors'),
    '#type' => 'radio',
    '#default_value' => $settings['policy'] == 'editor' ? 'editor' : FALSE,
    '#parents' => array('plugins', 'nodeterm', 'policy'),
    '#return_value' => 'editor',
    '#description' => t("As above but node terms are added for new community tags when tagged by a node editor, and removed when the last node editor's community tag for the term is removed. Also, when a node term is removed only the current user's matching community tag is removed. This option combines user friendliness (users community tags will not be deleted by node term removal), with convenience (node editors can use the CT interface to add and in most cases remove node terms), with security in that it prevents users without node edit permission to add or remove node terms. <b>Recommended</b>."),
  );

  $form['plugins']['nodeterm']['policy']['minimal'] = array(
    '#title' => t('Minimal synchronisation'),
    '#type' => 'radio',
    '#default_value' => $settings['policy'] == 'minimal' ? 'minimal' : FALSE,
    '#parents' => array('plugins', 'nodeterm', 'policy'),
    '#return_value' => 'minimal',
    '#description' => t("When a node term is added, a community tag is added. When a node term is removed, the current user's community tag is removed."),
  );

  $form['plugins']['nodeterm']['policy']['none'] = array(
    '#title' => t('No synchronisation'),
    '#type' => 'radio',
    '#default_value' => $settings['policy'] == 'none' ? 'none' : FALSE,
    '#parents' => array('plugins', 'nodeterm', 'policy'),
    '#return_value' => 'none',
    '#description' => t("No synchronisation between a node's terms and it's community tags. Tags may be manually 'promoted' to node terms via the tag management tab."),
  );



  $form['#submit'][] = 'community_tags_node_sub_settings_submit';

}

/**
 * Submit handler to handle CT_node part of sub settings form.
 */
function community_tags_node_sub_settings_submit($form, &$form_state) {
  $vid = $form['#vid'];
  $content_type = $form['#content_type'];

  $values = $form_state['values']['plugins']['nodeterm'];

  $settings = _community_tags_node_get_settings($vid, $content_type);
  $settings['policy'] = $values['policy'];

  _community_tags_node_set_settings($vid, $content_type, $settings);
}

/**
 * "Private" implementation of hook_community_tags_admin_operations(). Return a rebuild
 * link if tags and node terms are not in sync.
 */
function _community_tags_node_community_tags_admin_operations($vid, $content_type) {
  $status = _community_tags_node_get_status($vid, $content_type);
  $settings = _community_tags_node_get_settings($vid, $content_type);

  $operations = array();
  // operations for valid enabled CT vocabulary content type combinations
  // add in deleted orphaned terms - need to encapsulate all this
  if ($status['rebuild_required'] && $settings['opmode'] & (COMMUNITY_TAGS_NODE_OPMODE_SYNC_NT_TO_CT | COMMUNITY_TAGS_NODE_OPMODE_SYNC_CT_TO_NT)) {
    $operations[] = array('title' => t('rebuild'), 'href' => "admin/settings/community-tags/ops/rebuild/$vid/$content_type", 'query' => drupal_get_destination());
  }

  return $operations;
}

/**
 * "Private" implementation of hook_community_tags_admin_status(). Return the any
 * out of sync messages.
 */
function _community_tags_node_community_tags_admin_status($vid, $content_type) {
  $status = _community_tags_node_get_status($vid, $content_type);
  $settings = _community_tags_node_get_settings($vid, $content_type);

  if(!empty($status['messages'])) {
    return $status['messages'];
  }
}

/**
 * Save settings for this module.
 */
function _community_tags_node_set_settings($vid, $content_type, $settings) {
  $all_settings = variable_get('community_tags_node_settings', array());
  $all_settings[$vid]['types'][$content_type] = $settings;
  variable_set('community_tags_node_settings', $all_settings);
}

/**
 * Gets the status of node term synchronisation.
 */
function _community_tags_node_get_status($vid, $content_type) {
  $status = array();

  $settings = _community_tags_node_get_settings($vid, $content_type);
  $ct_settings = _community_tags_get_settings($vid, $content_type);

  // default status to OK - override if not.
  $status['summary'] = '';
  $status['rebuild_required'] = FALSE;
  $status['messages'] = array();
  if($settings['opmode'] & COMMUNITY_TAGS_NODE_OPMODE_SYNC_NT_TO_CT) {
    $missing_tag_counts = _community_tags_get_out_of_sync_missing_tag_counts($vid, $content_type);
    if($missing_tag_counts->missing_ctag_count > 0) {
      $status['rebuild_required'] = TRUE;
      $status['messages'][] = t('Missing tags: %count', array('%count' => $missing_tag_counts->missing_ctag_count));
    }
  }

  if($settings['opmode'] & COMMUNITY_TAGS_NODE_OPMODE_SYNC_CT_TO_NT) {
    $missing_nodeterm_counts = _community_tags_get_out_of_sync_missing_nodeterm_counts($vid, $content_type);
    if($missing_nodeterm_counts->missing_nodeterm_count > 0) {
      $status['rebuild_required'] = TRUE;
      $status['messages'][] = t('Missing term nodes: %count', array('%count' => $missing_nodeterm_counts->missing_nodeterm_count));
    }
  }

  if(!empty($status['messages'])) {
    $status['summary'] = implode('; ', $status['messages']);
  }

  return $status;
}

/**
 * Confirmation form for tag rebuild.
 */
function community_tags_node_rebuild_form(&$form_state, $vocabulary, $content_type) {
  $form = array();
  $type_obj = node_get_types('type', $content_type);

  $settings = _community_tags_node_get_settings($vid, $content_type);

  $missing_tag_counts = _community_tags_get_out_of_sync_missing_tag_counts($vocabulary->vid, $content_type);
  $missing_nodeterm_counts = _community_tags_get_out_of_sync_missing_nodeterm_counts($vocabulary->vid, $content_type);

  $form['#rebuild_ops'] = array();
  $form['#ct_settings'] = $settings;
  $form['#vocabulary'] = $vocabulary;
  $form['#content_type_obj'] = $type_obj;

  $description = '';

  if($settings) {
    $question = t('Rebuild community tags for %vocabulary terms on %type nodes?', array('%vocabulary' => $vocabulary->name, '%type' => $type_obj->name));

    if($settings['opmode'] & COMMUNITY_TAGS_NODE_OPMODE_SYNC_NT_TO_CT && $missing_tag_counts->missing_ctag_count > 0) {
      $description .= '<p>'.t('This operation will rebuild %count missing community tags on %node_count nodes.', array('%count' => $missing_tag_counts->missing_ctag_count, '%node_count' => $missing_tag_counts->node_count)).'</p>';

      $form['#rebuild_ops']['tags'] = $missing_tag_counts;
    }

    if($settings['opmode'] & COMMUNITY_TAGS_NODE_OPMODE_SYNC_CT_TO_NT && $missing_nodeterm_counts->missing_nodeterm_count > 0) {
      $description .= '<p>'.t('There are %count community tags that have no corresponding node terms. Select a syncronistion mode: ', array('%count' => $missing_nodeterm_counts->missing_nodeterm_count)).'</p>';

      // offer option to bring ct's into line with nt or vice versa
      $form['sync_policy'] = array(
        '#type' => 'radios',
        '#title' => t('Synchronisation mode'),
        '#options' => array(
          t('<em>Delete community tags</em> that have no corresponding node term.'),
          t('<em>Add node terms</em> for all community tags that have no corresponding node term.'),
        ),
        '#weight' => 1,
        '#default_value' => 0,
      );
      $form['#rebuild_ops']['nodeterms'] = $missing_nodeterm_counts;
    }

    // don't do orphaned term removal - needs work.
    if(empty($form['#rebuild_ops'])) {
      drupal_set_message(t('Community tags for @vocabulary terms on @type nodes are up to date. Nothing to rebuild. ', array('@vocabulary' => $vocabulary->name, '@type' => $type_obj->name)));
      return;
    }
  }
  else {
    drupal_set_message(t('Community tagging is not active for @vocabulary terms on @type nodes. Will not rebuild. ', array('@vocabulary' => $vocabulary->name, '@type' => $type_obj->name)));
    return;
  }

  $path = 'admin/settings/community-tags/'.$vocabulary->vid.'/'.$content_type;

  $confirm_form = confirm_form($form, $question, $path, $description);
  $confirm_form['actions']['#weight'] = 2;
  return $confirm_form;
}

/**
 * Rebuild confirmation form handler. Starts batched rebuild operation.
 */
function community_tags_node_rebuild_form_submit($form, &$form_state) {
  $vid = $form['#vocabulary']->vid;
  $content_type = $form['#content_type_obj']->type;

  $operations = array();

  // set up the tag rebuild operations
  if(isset($form['#rebuild_ops']['tags'])) {
    $operations[] = array('community_tags_node_rebuild_tags_batch_process', array($vid, $content_type, $form['#rebuild_ops']['tags']->node_count));
  }

  if(isset($form['#rebuild_ops']['nodeterms'])) {
    $mode = $form_state['values']['sync_policy'];
    $operations[] = array('community_tags_node_rebuild_nodeterms_batch_process', array($vid, $content_type, $form['#rebuild_ops']['nodeterms']->node_count, $mode));
  }

  $batch = array(
    'operations' => $operations,
    'finished' => 'community_tags_node_batch_finished',
    'title' => t('Processing Community Tags Rebuild Batch'),
    'init_message' => t('Community Tags Rebuild Batch is starting.'),
    // 'progress_message' => t('Processed @current out of @total.'),
    'error_message' => t('Community Tags Rebuild Batch has encountered an error.'),
    'file' => drupal_get_path('module', 'community_tags_node') . '/community_tags_node.batch.inc',
  );
  batch_set($batch);
}

/**
 * Get number of missing community tags i.e. node terms that don't have community tags. Also return the number of nodes that have missing ctags.
 *  - number of missing term nodes i.e. community tags that don't have a matching node term (sync mode only).
 *
 * @return
 *  Object with properties: {node_count, missing_ctag_count}.
 */
function _community_tags_get_out_of_sync_missing_tag_counts($vid, $content_type) {
  $result = db_query(
    "SELECT count(distinct n.nid) node_count, count(distinct n.nid, td.tid) missing_ctag_count
     FROM {term_node} tn
     INNER JOIN {term_data} td ON td.tid = tn.tid AND td.vid = %d
     INNER JOIN {node} n ON n.nid = tn.nid AND n.type = '%s'
     LEFT JOIN {community_tags} ct ON ct.nid = tn.nid AND ct.tid = tn.tid
     WHERE ct.nid IS NULL", $vid, $content_type);

  $counts = db_fetch_object($result);
  return $counts;
}

/**
 * Get out_of_sync_missing_tag_counts counts for all vocabulary and node type combinations.
 */
function _community_tags_get_all_out_of_sync_missing_tag_counts() {
  $counts = array();
  $result = db_query(
    "SELECT td.vid, n.type, count(distinct tn.nid) node_count, count(distinct tn.nid) term_count, count(distinct tn.nid, tn.tid) missing_ctag_count
     FROM {term_node} tn
     INNER JOIN {term_data} td ON td.tid = tn.tid
     INNER JOIN {node} n ON n.nid = tn.nid
     LEFT JOIN {community_tags} ct ON ct.nid = tn.nid AND ct.tid = tn.tid
     WHERE ct.nid IS NULL
     GROUP BY td.vid, n.type");

  while($row = db_fetch_object($result)) {
    $counts[$row->vid][$row->type] = $row;
  }
  return $counts;
}

/**
 * Get out_of_sync_missing_node_terms counts for all vocabulary and node type combinations.
 */
function _community_tags_get_all_out_of_sync_missing_node_terms_counts() {
  $counts = array();
  $result = db_query(
    "SELECT td.vid, n.type, count(distinct ct.nid) node_count, count(distinct ct.nid) term_count, count(distinct ct.nid, ct.tid) missing_termnode_count
     FROM {community_tags} ct
     INNER JOIN {term_data} td ON td.tid = ct.tid
     INNER JOIN {node} n ON n.nid = ct.nid
     LEFT JOIN {term_node} tn ON tn.nid = ct.nid AND tn.tid = ct.tid
     WHERE tn.nid IS NULL
     GROUP BY td.vid, n.type");

  while($row = db_fetch_object($result)) {
    $counts[$row->vid][$row->type] = $row;
  }
  return $counts;
}

/**
 * Get the number of missing term nodes i.e. community tags that don't have a matching node term (sync mode only). Also return the number of nodes that have missing node terms.
 *
 * @return
 *  Object with properties: {node_count, missing_nodeterm_count}.
 */
function _community_tags_get_out_of_sync_missing_nodeterm_counts($vid, $content_type) {
  $result = db_query(
    "SELECT count(distinct n.nid) node_count, count(*) missing_nodeterm_count
     FROM {community_tags} ct
     INNER JOIN {term_data} td ON td.tid = ct.tid AND td.vid = %d
     INNER JOIN {node} n ON n.nid = ct.nid AND n.type = '%s'
     LEFT JOIN {term_node} tn ON tn.nid = ct.nid AND tn.tid = ct.tid
     WHERE tn.nid IS NULL", $vid, $content_type);

  $counts = db_fetch_object($result);
  return $counts;
}

/**
 * "Private" implementation of hook_form_FORM_ID_alter() for community_tags_sub_settings.
 * Add a warning to the form with a link to the settings page.
 */
function _community_tags_node_delete_all(&$form, &$form_state) {
  $vid = $form['#vocabulary']->vid;
  $type = $form['#content_type_obj']->type;

  $settings = _community_tags_node_get_settings($vid, $type);

  if($settings['opmode'] & COMMUNITY_TAGS_NODE_OPMODE_SYNC_CT_TO_NT) {
    // offer option to bring ct's into line with nt or vice versa
    $form['nodeterm_sync_policy'] = array(
      '#type' => 'item',
      '#title' => t('Node term synchronisation'),
      '#weight' => 2,
      '#value' => t('<em>Node terms are synchronised with community tags. This operation will remove terms from nodes for all community tags that are deleted. To change node term synchronisation options go to the <a href="@sub-settings-page">settings page</a>.', array('@sub-settings-page' => url('admin/settings/community-tags/'.$vid.'/'.$type))),
    );
  }
}

?>