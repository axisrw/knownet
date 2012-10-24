<?php
// $Id$
class community_tags_views_handler_sort_tag_count extends views_handler_sort {
  function query() {
    $this->ensure_my_table();
    // Add the field.
    $aggregate_field = 'COUNT('.$this->table_alias.'.tid)';
    $this->query->add_orderby(NULL, $aggregate_field, $this->options['order'], $this->field);
    $this->query->distinct = TRUE;
  }
}
