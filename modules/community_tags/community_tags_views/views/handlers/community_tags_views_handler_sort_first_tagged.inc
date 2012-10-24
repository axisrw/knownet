<?php
// $Id$
class community_tags_views_handler_sort_first_tagged extends views_handler_sort_date {
  /**
   * Called to add the sort to a query.
   */
  function query() {
    $this->ensure_my_table();
    $aggregate_field = 'MIN('.$this->table_alias.'.date)';

    switch ($this->options['granularity']) {
      case 'second':
      default:
        $this->query->add_orderby(NULL, $aggregate_field, $this->options['order'], $this->field);
        return;
      case 'minute':
        $formula = views_date_sql_format('YmdHi', $aggregate_field);
        break;
      case 'hour':
        $formula = views_date_sql_format('YmdH', $aggregate_field);
        break;
      case 'day':
        $formula = views_date_sql_format('Ymd', $aggregate_field);
        break;
      case 'month':
        $formula = views_date_sql_format('Ym', $aggregate_field);
        break;
      case 'year':
        $formula = views_date_sql_format('Y', $aggregate_field);
        break;
    }

    // Add the field.
    $this->query->add_orderby(NULL, $formula, $this->options['order'], $this->table_alias . '_' . $this->field . '_' . $this->options['granularity']);
  }
}
