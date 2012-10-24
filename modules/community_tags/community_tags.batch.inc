<?php

/**
 * @file
 * community_tags.batch.inc
 *
 * Batch process functions.
 */

/**
 * Batch process for deleting tags and optionally their corresponding node terms.
 */
function community_tags_delete_tags_batch_process($vid, $content_type, $counts, $mode, &$context) {
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['current_node'] = 0;
    $context['sandbox']['max'] = $counts->node_count;
  }

  // get all the next lot of nodes to update. Note the range starts at 0 because the processed ones don't get returned by the query. duh.
  $result = db_query_range(
    "SELECT n.nid, n.uid, count(distinct ct.tid) term_count, group_concat(ct.tid) tids, count(*) tag_count
     FROM {community_tags} ct
     INNER JOIN {term_data} td ON td.tid = ct.tid AND td.vid = %d
     INNER JOIN {node} n ON n.nid = ct.nid AND n.type = '%s'
     LEFT JOIN {term_node} tn ON tn.nid = ct.nid AND tn.tid = ct.tid
     GROUP BY n.nid", $vid, $content_type, 0, 100);

  $initial_progress = $context['sandbox']['progress'];

  $source = $mode & COMMUNITY_TAGS_OPMODE_DELETE_ORPHAN_TERMS ? 'community_tags:purge:sync' : 'community_tags:purge:no_sync';

  while($row = db_fetch_object($result)) {
    // get the tids from the query results
    $tids = explode(',', $row->tids);

    // delete community tags
    $tags = array();
    foreach($tids as $tid) {
      $tags[$tid] = (object)array('tid' => $tid, 'vid' => $vid);
    }
    $node = (object)array('nid' => $row->nid, 'type' => $content_type);
    community_tags_remove_tags($node, NULL, $tags, $source);

    $context['sandbox']['progress'] += 1;
    $context['sandbox']['tag_count'] += $row->tag_count;
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
    $context['results'][] = t('Deleted %tag_count community tags from %node_count nodes.', array('%tag_count' => $context['sandbox']['tag_count'], '%node_count' => $context['sandbox']['progress']));
  }
}

/**
 * Batch 'finished' callback
 */
function community_tags_batch_finished($success, $results, $operations) {
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

