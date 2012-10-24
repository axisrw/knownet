<?php
// $Id$

/**
 * @file
 * community_tags_node.batch.inc
 *
 * Batch process functions.
 */

/**
 * Batch process for rebuilding missing tags from node terms.
 */
function community_tags_node_rebuild_tags_batch_process($vid, $content_type, $count, &$context) {
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['current_node'] = 0;
    $context['sandbox']['max'] = $count;
  }

  // get all the next lot of nodes to update. Note the range starts at 0 because the processed ones don't get returned by the query. duh.
  $result = db_query_range(
    "SELECT n.nid, n.uid, count(distinct tn.tid) missing_ctag_count, group_concat(tn.tid) tids
     FROM {term_node} tn
     INNER JOIN {term_data} td ON td.tid = tn.tid AND td.vid = %d
     INNER JOIN {node} n ON n.nid = tn.nid AND n.type = '%s'
     LEFT JOIN {community_tags} ct ON ct.nid = tn.nid AND ct.tid = tn.tid
     WHERE ct.nid IS NULL
     GROUP BY n.nid", $vid, $content_type, 0, 100);

  $initial_progress = $context['sandbox']['progress'];

  // don't let this module hook implementations do anything whilst doing batch processing
  _community_tags_node_is_processing(TRUE);

  while($row = db_fetch_object($result)) {
    // get the tids from the query results
    $tids = explode(',', $row->tids);

    // add - uid is node author
    $tags = array();
    foreach($tids as $tid) {
      $tags[$tid] = (object)array('tid' => $tid, 'vid' => $vid);
    }

    $node = (object)array('nid' => $row->nid, 'type' => $content_type);
    $user = (object)array('uid' => $row->uid);

    community_tags_add_tags($node, $user, $tags);

    $context['sandbox']['progress'] += 1;
    $context['sandbox']['tag_count'] += $row->missing_ctag_count;
  }

  // Inform the batch engine that we are not finished,
  // and provide an estimation of the completion level we reached.
  // guard against not reaching the max!
  // nothing added - something changed whilst processing - finish early
  if ($context['sandbox']['progress'] != $context['sandbox']['max'] && $initial_progress != $context['sandbox']['progress']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
  else {
    $context['sandbox']['progress'] = $context['sandbox']['max'];
    $context['results'][] = t('Added %tag_count community tags to %node_count nodes.', array('%tag_count' => $context['sandbox']['tag_count'], '%node_count' => $context['sandbox']['progress']));
  }
}

/**
 * Batch process for rebuilding missing term nodes from community tags.
 */
function community_tags_node_rebuild_nodeterms_batch_process($vid, $content_type, $count, $mode, &$context) {
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['current_node'] = 0;
    $context['sandbox']['max'] = $count;
  }

  // number of modes to process in single operation step depends on the mode. Term adding
  // takes much longer - so lower limit
  $limit = $mode == 1 ? 20 : 100;

  // get all the next lot of nodes to update. Note the range starts at 0 because the processed ones don't get returned by the query. duh.
  $result = db_query_range(
    "SELECT n.nid, count(distinct ct.tid) missing_nodeterm_count, group_concat(distinct ct.tid) tids
     FROM {community_tags} ct
     INNER JOIN {term_data} td ON td.tid = ct.tid AND td.vid = %d
     INNER JOIN {node} n ON n.nid = ct.nid AND n.type = '%s'
     LEFT JOIN {term_node} tn ON tn.nid = ct.nid AND tn.tid = ct.tid
     WHERE tn.nid IS NULL
     GROUP BY n.nid", $vid, $content_type, 0, $limit);

  $initial_progress = $context['sandbox']['progress'];

  // don't let this module hook implementations do anything whilst doing batch processing
  _community_tags_node_is_processing(TRUE);

  while($row = db_fetch_object($result)) {
    // get the tids from the query results
    $tids = explode(',', $row->tids);

    if($mode == 1) {
      // load node but instruct not to put in static array
      $node = node_load($row->nid, NULL, TRUE);

      // add all these to the node
      foreach($tids as $tid) {
        $node->taxonomy[$tid] = taxonomy_get_term($tid);
      }

      // add save the node
      node_save($node);
    }
    else {
      // delete community tags
      $tags = array();
      foreach($tids as $tid) {
        $tags[$tid] = (object)array('tid' => $tid, 'vid' => $vid);
      }
      $node = (object)array('nid' => $row->nid, 'type' => $content_type);
      community_tags_remove_tags($node, NULL, $tags);
    }

    $context['sandbox']['progress'] += 1;
    $context['sandbox']['nt_count'] += $row->missing_nodeterm_count;
  }

  // Inform the batch engine that we are not finished,
  // and provide an estimation of the completion level we reached.
  // guard against not reaching the max!
  // nothing added - something changed whilst processing - finish early
  if ($context['sandbox']['progress'] != $context['sandbox']['max'] && $initial_progress != $context['sandbox']['progress']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
  else {
    $context['sandbox']['progress'] = $context['sandbox']['max'];
    if($mode == 1) {
      $context['results'][] = t('Added %nt_count terms to %node_count nodes.', array('%nt_count' => $context['sandbox']['nt_count'], '%node_count' => $context['sandbox']['progress']));
    }
    else {
      $context['results'][] = t('Deleted %nt_count community tags from %node_count nodes.', array('%nt_count' => $context['sandbox']['nt_count'], '%node_count' => $context['sandbox']['progress']));
    }
  }
}

/**
 * Batch 'finished' callback
 */
function community_tags_node_batch_finished($success, $results, $operations) {
  if ($success) {
    // Here we do something meaningful with the results.
    $message = count($results) .' processed.';
    $message .= theme('item_list', $results);
  }
  else {
    // An error occurred.
    // $operations contains the operations that remained unprocessed.
    $error_operation = reset($operations);
    $message = t('An error occurred while processing %error_operation with arguments: @arguments', array('%error_operation' => $error_operation[0], '@arguments' => print_r($error_operation[1], TRUE)));
  }
  drupal_set_message($message);
}

