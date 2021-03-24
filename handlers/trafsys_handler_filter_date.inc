<?php
/**
 * Created by PhpStorm.
 * User: smit0015
 * Date: 1/12/21
 * Time: 12:19 PM
 */

/**
 * Making sure the field_alias gets set properly, and that
 * none of the sql-specific query functionality gets called.
 */
class trafsys_handler_filter_bool extends views_handler_filter_boolean_operator {
  function query() {
    $this->field_alias = $this->real_field;
  }
}