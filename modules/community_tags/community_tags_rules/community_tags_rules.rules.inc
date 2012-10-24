<?php
// $Id$

/**
 * @file
 * community_tags_rules.rules.inc
 *
 * Rules integration functions.
 */

/**
* Implementation of hook_rules_event_info().
* @ingroup rules
*/
function community_tags_rules_rules_event_info() {
  return array(
    'community_tags_tagged' => array(
      'label' => t('User tags node with term'),
      'module' => 'Community Tags',
      'arguments' => array(
        'user' => array('type' => 'user', 'label' => t('The user who tags the node with a term.')),
        'node' => array('type' => 'node', 'label' => t('The node which the user tags with a term.')),
        'term' => array('type' => 'taxonomy_term', 'label' => t('The term with which the user tags the node.')),
        'ctag' => array('type' => 'community_tag', 'label' => t('The community tag'), 'handler' => 'community_tags_rules_argument_handler_ctag'),
      ),
    ),
    'community_tags_untagged' => array(
      'label' => t('User removes their tag from node'),
      'module' => 'Community Tags',
      'arguments' => array(
        'user' => array('type' => 'user', 'label' => t('The user who removes their tag from the node.')),
        'node' => array('type' => 'node', 'label' => t('The node from which the user removes their tag.')),
        'term' => array('type' => 'taxonomy_term', 'label' => t('The term of the tag the user removes from the node.')),
      ),
    ),
  );
}

function community_tags_rules_argument_handler_ctag($user, $node, $term) {
  return _community_tags_get_tag($node->nid, $term->tid, $user->uid);
}

/**
 * Implementation of hook_rules_data_type_info().
 */
function community_tags_rules_rules_data_type_info() {
  return array(
    'community_tag' => array(
      'label' => t('Community Tag'),
      'class' => 'rules_data_type_community_tag',
      'savable' => FALSE,
      'identifiable' => TRUE,
      'uses_input_form' => FALSE,
      'module' => 'Community Tags',
    ),
  );
}

/**
 * Defines the rules node data type.
 */
class rules_data_type_community_tag extends rules_data_type {

  function load($identifier) {
    return _community_tags_get_tag($identifier['nid'], $identifier['tid'], $identifier['uid']);
  }

  function get_identifier() {
    $ctag = &$this->get();
    return array('nid' => $ctag->nid, 'tid' => $ctag->tid, 'uid' => $ctag->uid);
  }
}

/**
 * Implementation of hook_rules_condition_info().
 * @ingroup rules
 */
function community_tags_rules_rules_condition_info() {
  return array(
    'community_tags_condition_is_first_tag' => array(
      'label' => t('Test if first tag of node with term'),
      'arguments' => array(
        'ctag' => array('type' => 'community_tag', 'label' => t('Community tag')),
      ),
      'module' => 'Community Tags',
    ),
    'community_tags_condition_tagger_is_author' => array(
      'label' => t('Test if the user tagging is also the node author'),
      'arguments' => array(
        'node' => array('type' => 'node', 'label' => t('Node being tagged')),
        'user' => array('type' => 'user', 'label' => t('User doing the tagging')),
      ),
      'module' => 'Community Tags',
    ),
  );
}

/**
 * Rules condition that returns true if the node has not been tagged with the
 * tag term before.
 */
function community_tags_condition_is_first_tag($ctag) {
  return $ctag->date == $ctag->first_tagged;
}

/**
 * Rules condition that returns true if the user doing the tagging is the node
 * author.
 */
function community_tags_condition_tagger_is_author($node, $user, $ctag) {
  return $node->uid == $user->uid;
}

