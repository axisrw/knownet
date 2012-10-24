<?php
// $Id$
class community_tags_views_handler_field_tags extends views_handler_field_prerender_list {

  /**
   * Called to add the field to a query.
   */
  function query() {
    $this->ensure_my_table();
    // Add the field.
    $aggregate_field = 'GROUP_CONCAT('.$this->table_alias.'.tid)';
    // dpm($this->field);
    $this->field_alias = $this->query->add_field(NULL, $aggregate_field, $this->field, array('aggregate' => TRUE));
    $this->query->distinct = TRUE;

    // The following would be alot quicker but what about i18n etc?
    // $join = new views_join();
    //
    // $def = array();
    // $def['table'] = 'term_data';
    // $def['field'] = 'tid';
    // $def['left_table'] = 'community_tags_aggregates';
    // $def['left_field'] = 'tid';
    // $def['type'] = 'INNER';
    //
    // $join->definition = $def;
    // $join->construct();
    // $join->adjusted = TRUE;
    //
    // // use a short alias for this:
    // $alias = 'term_data_community_tags_aggregates';
    //
    // $relationship_alias = $this->query->add_relationship($alias, $join, 'term_data');
    //
    // $aggregated_term_name_field = 'GROUP_CONCAT('.$relationship_alias.'.name)';
    // $this->field_alias = $this->query->add_field(NULL, $aggregated_term_name_field, 'ct_tag_names', array('aggregate' => TRUE));
  }

  function get_items($values) {
    $tids_str = $values->{$this->field};
    $tids = explode(',', $tids_str);
    $tid_counts = array_count_values($tids);
    asort($tid_counts, SORT_NUMERIC);
    $n = $this->options['limit'];

    $top_n_tid_counts = !empty($n) ? array_slice($tid_counts, $n*-1, $n, true) : $tid_counts;
    $top_n_tids = array_keys($top_n_tid_counts);

    $result = db_query("SELECT td.* FROM {term_data} td WHERE td.tid IN (".db_placeholders($top_n_tids).")", $top_n_tids);

    $items = array();
    while ($term = db_fetch_object($result)) {
      $item = array();
      $item['name'] = check_plain($term->name);
      $item['tid'] = $term->tid;
      $item['vid'] = $term->vid;
      $item['count'] = $top_n_tid_counts[$term->tid];

      if (!empty($this->options['link_to_taxonomy'])) {
        $item['make_link'] = TRUE;
        $item['path'] = taxonomy_term_path($term);
      }
      $items[] = $item;
    }

    return $items;
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['link_to_taxonomy'] = array('default' => TRUE);
    $options['limit'] = array('default' => FALSE);

    return $options;
  }

  /**
   * Provide "link to term" option.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['link_to_taxonomy'] = array(
      '#title' => t('Link this field to its term page'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_taxonomy']),
    );

    $form['limit'] = array(
      '#type' => 'textfield',
      '#title' => t('Limit terms'),
      '#default_value'=> $this->options['limit'],
    );
  }

  function render_item($count, $item) {
    return $item['name'];
  }

  function document_self_tokens(&$tokens) {
    $tokens['[' . $this->options['id'] . '-tid' . ']'] = t('The taxonomy term ID for the term.');
    $tokens['[' . $this->options['id'] . '-name' . ']'] = t('The taxonomy term name for the term.');
    $tokens['[' . $this->options['id'] . '-vid' . ']'] = t('The vocabulary ID for the vocabulary the term belongs to.');
    $tokens['[' . $this->options['id'] . '-count' . ']'] = t('Tag count.');
    // $tokens['[' . $this->options['id'] . '-vocabulary' . ']'] = t('The name for the vocabulary the term belongs to.');
  }

  function add_self_tokens(&$tokens, $item) {
    $tokens['[' . $this->options['id'] . '-tid' . ']'] = $item['tid'];
    $tokens['[' . $this->options['id'] . '-name' . ']'] = $item['name'];
    $tokens['[' . $this->options['id'] . '-vid' . ']'] = $item['vid'];
    $tokens['[' . $this->options['id'] . '-count' . ']'] = $item['count'];
    // $tokens['[' . $this->options['id'] . '-vocabulary' . ']'] = $item['vocabulary'];
  }
}
