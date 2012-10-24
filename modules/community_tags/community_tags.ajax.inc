<?php

/**
 * @file
 * community_tags.ajax.inc
 *
 * Handles the server side Ajax interactions of Community Tags.
 *
 * @defgroup community_tags_ajax Community Tags server side Ajax interactions.
 * @{
 */


/**
 * Callback for the JS tagger.
 */
function community_tags_from_js($node, $vid = NULL) {
  global $user;

  if ($node && isset($_POST['token']) && drupal_valid_token($_POST['token'], 'community_tags_form')) {
    $tags = (isset($_POST['tags']) && is_array($_POST['tags'])) ? $_POST['tags'] : array();

    // Merge in new tag and save.
    $tags = array_unique(array_merge($tags, drupal_explode_tags($_POST['add'])));

    //There should always be $vid, but if not, this will make a best guess.
    if(!$vid){
      $vid = array_shift(community_tags_vids_for_node($node));
    }

    community_tags_taxonomy_node_save($node, array('tags' => array($vid => $tags)), FALSE, $user->uid);

    // Fetch updated list.
    $tags = community_tags_flatten(community_tags_get_user_node_tags($user, $node, $vid));

    $json = array('status' => TRUE, 'tags' => $tags, 'sequence' => $_POST['sequence']);

    $messages = drupal_get_messages('status', TRUE);

    if(!empty($messages['status'])) {
      $messages = implode('; ', $messages['status']);
      $json['messages'] = $messages;
    }

    drupal_alter('community_tags_json', $json, $node, $user, $vid);

    // Output JSON.
    print drupal_json($json);
  }
}

/**
 * Default json handler adds an updated cloud to the response.
 */
function community_tags_community_tags_json_alter(&$json, $node, $user, $vid) {
  if (empty($_POST['source']) || $_POST['source'] == 'undefined') {
    $display_handler = _community_tags_get_display_handler($vid, $node->type);
    $json['cloud'] = call_user_func($display_handler['fn'], $node, $vid);
    $json['update_tags'] = TRUE;
  }
}
