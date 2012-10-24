<?php
/**
 * Authcache callback function for community tags.
 *
 * @param $vars
 *  This is the value that was included in the authcache ajax object of the cached page by community_tags_authcache_authcache_ajax().
 *
 * @return
 *  JSON containing the user's community tags.
 */
function community_tags_authcache_callback($vars) {
  global $user;
  // Would be nice not to do a full bootstrap but it's the only way to guarantee that
  // all hooks get called.
  drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
  if(!empty($_GET['ct'])) {
    $ct = is_array($_GET['ct']) ? $_GET['ct'] : array($_GET['ct']);
    $tags = array();
    foreach($ct as $nid_vid) {
      list($nid, $vid) = explode('_', $nid_vid);

      $node = node_load($nid);
      $tags_for_node_and_vid = community_tags_get_user_node_tags($user, $node, $vid);
      $tags[$nid_vid] = taxonomy_implode_tags($tags_for_node_and_vid);
    }
    return array('msg' => 'OK', 'tags' => $tags);
  }
}
?>