<?php
// $Id$
class community_tags_views_handler_field_first_tagged extends views_handler_field_date {
  /**
   * Called to add the field to a query.
   */
  function query() {
    $this->ensure_my_table();
    // Add the field.
    $aggregate_field = 'MIN('.$this->table_alias.'.date)';
    $this->field_alias = $this->query->add_field(NULL, $aggregate_field, $this->field, array('aggregate' => TRUE));
    $this->query->distinct = TRUE;
  }
}
