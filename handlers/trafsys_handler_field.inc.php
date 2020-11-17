<?php
/**
 * Created by PhpStorm.
 * User: Tim Smith
 * Date: 11/16/20
 * Time: 9:51 AM
 */

/**
 * Making sure the field_alias gets set properly, and that
 * none of the sql-specific query functionality gets called.
 */
class trafsys_handler_field extends views_handler_field {
    function query() {
        $this->field_alias = $this->real_field;
    }
}