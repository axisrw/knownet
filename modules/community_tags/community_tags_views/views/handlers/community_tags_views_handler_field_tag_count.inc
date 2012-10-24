<?php
// $Id$
class community_tags_views_handler_field_tag_count extends views_handler_field_numeric {
  /**
   * Called to add the field to a query.
   */
  function query() {
    $this->ensure_my_table();
    // Add the field.
    // dpm($this);
    $aggregate_field = 'COUNT('.$this->table_alias.'.tid)';
    $this->field_alias = $this->query->add_field(NULL, $aggregate_field, $this->field, array('aggregate' => TRUE));
    // need this for the summary query to behave
    $this->query->distinct = TRUE;
  }
}
