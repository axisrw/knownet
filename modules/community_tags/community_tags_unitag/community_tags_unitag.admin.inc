<?php
// $Id$
/**
 * @file
 * community_tags_unitag.admin.inc
 *
 * All admin level code for the Community Tags Unitag integration module.
 *
 * @todo Make suggestion saving more efficient so it calls community_tags_add_tag() once per node for all approved terms
 */

/**
 * "Private" implementation of hook_form_FORM_ID_alter() for community_tags_sub_settings.
 */
function _community_tags_unitag_sub_settings(&$form, &$form_state) {
  $vid = $form['#vid'];
  $content_type = $form['#content_type'];

  // $status = _community_tags_node_get_status($vid, $content_type);
  $settings = _community_tags_unitag_get_settings($vid, $content_type);

  $form['plugins']['unitag'] = array(
    '#type' => 'fieldset',
    '#title' => t('Unitag moderation'),
  );

  $form['plugins']['unitag']['enabled'] = array(
    '#title' => t('Process tags with Unitag'),
    '#type' => 'checkbox',
    '#default_value' => $settings['enabled'],
    '#description' => t("When checked, community tags are processed by Unitag."),
  );

  $form['plugins']['unitag']['queue'] = array(
    '#title' => t('Queue new tags for Unitag term moderation'),
    '#type' => 'radios',
    '#options' => array('all' => "Always", 'untrusted' => "Untrusted user tags", 'never' => t("Never")),
    '#default_value' => $settings['queue'],
    '#description' => t("If the Unitag configuration for this vocabulary is NOT read-only then select if or when new terms should be added to the Unitag moderation queue. Untrusted users are those without node edit or tag management permissions."),
  );

  $form['#submit'][] = 'community_tags_unitag_sub_settings_submit';
}

/**
 * "Private" implementation of extra submit handler for form community_tags_sub_settings.
 */
function community_tags_unitag_sub_settings_submit($form, &$form_state) {
  $vid = $form['#vid'];
  $content_type = $form['#content_type'];

  $values = $form_state['values']['plugins']['unitag'];

  $settings = _community_tags_unitag_get_settings($vid, $content_type);
  $settings['enabled'] = $values['enabled'];
  $settings['disabled_on_node_form'] = $values['disabled_on_node_form'];

  _community_tags_unitag_set_settings($vid, $content_type, $settings);
}

/**
 * Save settings. Module maintains it's own settings.
 */
function _community_tags_unitag_set_settings($vid, $content_type, $settings) {
  $all_settings = variable_get('community_tags_unitag_settings', array());
  $all_settings[$vid]['types'][$content_type] = $settings;
  variable_set('community_tags_unitag_settings', $all_settings);
}

/**
 * Process manage suggestions form submission for community tags. All suggestions
 * that are handled by this handler are deleted after processing and so will
 * not be handled by the standard unitag submit handler that runs after this one.
 */
function _community_tags_unitag_unitag_manage_suggestions_form_submit($form, &$form_state) {
  $utids = array_filter($form_state['values']['utids']);

  // only process suggestions that are CT suggestions
  foreach ($utids as $index => $utid) {
    // a utid is unique for a combination of nid, vid, basename, (and name)
    if ($suggestion = unitag_suggestion_load($utid)) {
      $suggesters = _community_tags_unitag_suggestion_users($utid);
      if(!empty($suggesters)) {
        // as we are handling this here - remove from form_state values so unitag submit handler doesn't handle it also.
        switch ($form_state['values']['operation']) {
          case 'new-term':
            // Save as a new term.
            $term = unitag_suggestion_save_term($suggestion);
            $term = is_array($term) ? (object) $term : $term;

            // Add term community tags for all tags
            _community_tags_unitag_save_suggestion_as_community_tags($suggestion, $term);
            break;
          case 'new-synonym':
            $term = unitag_taxonomy_get_term_by_name($form_state['values']['root'][$utid], $suggestion->vid);
            if ($term) {
              unitag_suggestion_save_synonym($suggestion, $term);

              // Cast term as an object as _save_term functions return an array.
              $term = is_array($term) ? (object) $term : $term;

              _community_tags_unitag_save_suggestion_as_community_tags($suggestion, $term);
            }
            else {
              drupal_set_message(t('Root term %term not found.', array('%term' => $form_state['values']['root'][$utid])), 'error');
            }
            break;
          case 'new-child':
            $parents = array();
            foreach ($form_state['values']['root'][$utid] as $tag) {
              $parent = unitag_taxonomy_get_term_by_name($tag, $suggestion->vid);
              if ($parent) {
                $parents[$parent->tid] = $parent->tid;
              }
              else {
                drupal_set_message(t('Parent term %term not found.', array('%term' => $tag)), 'error');
              }
            }
            $term = unitag_suggestion_save_term($suggestion, $parents);
            _community_tags_unitag_save_suggestion_as_community_tags($suggestion, $term);
            break;
          case 'deny':
            _community_tags_unitag_delete_suggestion($suggestion);
            unitag_suggestion_delete($suggestion);
            break;
          case 'blacklist-current':
            _community_tags_unitag_delete_suggestion($suggestion);
            unitag_suggestion_blacklist($suggestion);
            break;
          case 'blacklist-all':
            _community_tags_unitag_delete_suggestion($suggestion, TRUE);
            unitag_suggestion_blacklist($suggestion, TRUE);
            break;
        }
      }
    }
  }

  drupal_set_message(t('Community tag suggestions updated successfully.'));
}

/**
 * Find the suggestion in the ct unitags table and for all nid/uid's insert
 * into community tags table with the term. If term is synonym then have to handle possible duplicates
 * i.e. user 1 tags node 1 with tag 1 which is approved. user 1 then tags node 1 with tag 2 which is deemed a synonym of tag 1.
 */

/**
 * Because of the way Unitag works, we have to handle all instances for any suggestion
 * Add a suggested term as a community tag to all nodes of enabled types for all users that have tagged.
 * Delete only suggestions for nodes that have CT unitag processing enabled so that normal Unitag processing
 * can complete on nodes that aren't handled here.
 */
function _community_tags_unitag_save_suggestion_as_community_tags($suggestion, $term) {
  $result = db_query(
    "SELECT ctu.nid, ctu.vid, ctu.uid, ct.date existing_tag_date, ut.utid FROM {community_tags_unitag} ctu
     LEFT JOIN {community_tags} ct ON ct.nid = ctu.nid AND ct.uid = ctu.uid AND ct.tid = %d
     INNER JOIN {unitag} ut ON ut.basename = ctu.basename AND ut.vid = ctu.vid AND ut.nid = ctu.nid
     INNER JOIN {node} n ON n.nid = ctu.nid
     INNER JOIN {users} u ON u.uid = ctu.uid
     WHERE ctu.basename = '%s' AND ctu.vid = %d", $term->tid, $suggestion->basename, $suggestion->vid);

  while ($ct_unitag = db_fetch_object($result)) {
    if(is_null($ct_unitag->existing_tag_date)) {
      $node = node_load($ct_unitag->nid);
      $user = user_load($ct_unitag->uid);
      // create proper CT record for this approved new term.
      if (_community_tags_unitag_is_enabled($suggestion->vid, $node->type)) {
        community_tags_add_tags($node, $user, array($term->tid => $term));
      }
      else {
        // there may be edge cases where terms are not registered - if the same suggestion has been submitted on the node
        // edit page and the community tags form, the community tag will be created but the node term won't as
        // this bit of code will not be reached. If CT to node term synchronisation is enabled the node term will be
        // created that way.
        unitag_association_register($suggestion, $term);
      }
      // Delete all instances of this suggestion
      unitag_suggestion_delete($suggestion, TRUE);
    }
    else {
      // will occur if say user attempts to tag same content with a term that is moderated as a synonmym of
      // a term that this user has already used to tag this content.
    }
  }

  // delete all CT instances of this suggestion - if we've haven't handled it we don't need to keep it.
  _community_tags_unitag_delete_suggestion($suggestion);
}

/**
 * Delete the suggestion from the CT unitag queue.
 */
function _community_tags_unitag_delete_suggestion($suggestion, $all = FALSE) {
  if($all) {
    db_query("DELETE FROM {community_tags_unitag} WHERE basename = '%s' AND vid = %d", $suggestion->basename, $suggestion->vid);
  }
  else {
    db_query("DELETE FROM {community_tags_unitag} WHERE basename = '%s'", $suggestion->basename);
  }
}

/**
 * Load a particular CT suggestion.
 *
 * @param Integer $utid
 *   The utid of the suggestion to load.
 */
function _community_tags_unitag_suggestion_users($utid) {
  $result = db_query(
    "SELECT ctu.uid FROM {unitag} ut
     INNER JOIN {community_tags_unitag} ctu ON ctu.basename = ut.basename AND ctu.vid = ut.vid AND ctu.nid = ut.nid
     WHERE ut.utid = %d", $utid);

  $uids = array();
  while($row = db_result($result)) {
    $uids[] = $row;
  }

  return $uids;
}

/**
 * "Private" implementation of hook_community_tags_admin_operations(). Return a link
 * to Unitag moderation queue if tags are waiting to be moderated.
 */
function _community_tags_unitag_community_tags_admin_operations($vid, $content_type) {
  $status = _community_tags_unitag_community_tags_admin_status($vid, $content_type);
  if($status) {
    return array(array('title' => t('moderate'), 'href' => "admin/content/taxonomy/unitag", 'query' => drupal_get_destination()));
  }
}

/**
 * "Private" implementation of hook_community_tags_admin_status(). Return the number
 * of tags waiting to be moderated.
 */
function _community_tags_unitag_community_tags_admin_status($vid, $content_type) {
  $result = db_query(
   "SELECT count(distinct(ctu.basename)) suggestion_count, count(*) tag_count FROM {community_tags_unitag} ctu
    INNER JOIN {node} n ON n.nid = ctu.nid
    INNER JOIN {unitag} ut ON ut.basename = ctu.basename AND ut.vid = ctu.vid AND ut.nid = ctu.nid
    WHERE ctu.vid = %d AND n.type = '%s'", $vid, $content_type);

  if($row = db_fetch_object($result)) {
    if($row->suggestion_count > 0) {
      return array(t('Tag suggestions awaiting moderation: %suggestion_count ', array('%suggestion_count' => $row->suggestion_count)));
    }
  }
}



?>