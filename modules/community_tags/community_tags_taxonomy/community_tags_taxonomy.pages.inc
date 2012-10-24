<?php
// $Id$
/**
 * Menu callback; displays all nodes associated with a term.
 */
function community_tags_taxonomy_term_page($str_tids = '', $depth = 0, $op = 'page') {
  module_load_include('inc', 'taxonomy', 'taxonomy.pages');

  $terms = taxonomy_terms_parse_string($str_tids);
  if ($terms['operator'] != 'and' && $terms['operator'] != 'or') {
    drupal_not_found();
  }

  if ($terms['tids']) {
    $result = db_query(db_rewrite_sql('SELECT t.tid, t.name, t.vid FROM {term_data} t WHERE t.tid IN ('. db_placeholders($terms['tids']) .')', 't', 'tid'), $terms['tids']);
    $tids = array(); // we rebuild the $tids-array so it only contains terms the user has access to.
    $names = array();
    $vids = array();
    while ($term = db_fetch_object($result)) {
      $tids[] = $term->tid;
      $names[] = $term->name;
      $vids[$term->vid] = $term->vid;
    }

    // check whether to override or call the standard taxonomy pages
    $settings = array_intersect_key(_community_tags_taxonomy_get_settings(), $vids);
    $override = FALSE;
    foreach($settings as $vocab_settings) {
      if($vocab_settings['override_term_page']) {
        $override = TRUE;
        break;
      }
    }

    if(!$override) {
      // use the default term page
      return taxonomy_term_page($str_tids, $depth, $op);
    }


    if ($names) {
      $title = implode(', ', $names);
      drupal_set_title(check_plain($title));

      switch ($op) {
        case 'page':
          // Build breadcrumb based on first hierarchy of first term:
          $current->tid = $tids[0];
          $breadcrumb = array();
          while ($parents = taxonomy_get_parents($current->tid)) {
            $current = array_shift($parents);
            $breadcrumb[] = l($current->name, 'taxonomy/term/'. $current->tid);
          }
          $breadcrumb[] = l(t('Home'), NULL);
          $breadcrumb = array_reverse($breadcrumb);
          drupal_set_breadcrumb($breadcrumb);

          $output = theme('taxonomy_term_page', $tids, community_tags_taxonomy_select_nodes($tids, $terms['operator'], $depth, TRUE));
          drupal_add_feed(url('taxonomy/term/'. $str_tids .'/'. $depth .'/feed'), 'RSS - '. $title);
          return $output;
          break;

        case 'feed':
          $channel['link'] = url('taxonomy/term/'. $str_tids .'/'. $depth, array('absolute' => TRUE));
          $channel['title'] = variable_get('site_name', 'Drupal') .' - '. $title;
          // Only display the description if we have a single term, to avoid clutter and confusion.
          if (count($tids) == 1) {
            $term = taxonomy_get_term($tids[0]);
            // HTML will be removed from feed description, so no need to filter here.
            $channel['description'] = $term->description;
          }

          $result = community_tags_taxonomy_select_nodes($tids, $terms['operator'], $depth, FALSE);
          $items = array();
          while ($row = db_fetch_object($result)) {
            $items[] = $row->nid;
          }

          node_feed($items, $channel);
          break;

        default:
          drupal_not_found();
      }
    }
    else {
      drupal_not_found();
    }
  }
}



