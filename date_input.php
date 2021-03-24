<?php
/**
 * Created by PhpStorm.
 * User: Tim Smith
 * Date: 1/15/21
 * Time: 1:38 PM
 */

class date_input {

  public function returnThis($view) {
    $return_data_array = array();

    $temp_date_month = new DateTime('1 month ago');
    $one_month_ago = $temp_date_month->format('Y-m-d');

    $temp_date_year = new DateTime('1 year ago');
    $one_year_ago = $temp_date_year->format('Y-m-d');

    $default_min_date = $view->display_handler->get_option('trafsys_start_date') ?: $one_month_ago;
    $default_max_date = $view->display_handler->get_option('trafsys_end_date') ?: date('Y-m-d');
    if ($view->display_handler->handlers['filter']['periodending']) {
      $type = $view->display_handler->handlers['filter']['periodending']->operator;
      switch ($type) {
        case 'between': // has a MIN and a MAX date
          $temp_min_date = new DateTime($view->display_handler->handlers['filter']['periodending']->value['min']);
          $temp_max_date = new DateTime($view->display_handler->handlers['filter']['periodending']->value['max']);
          $formatted_min_date = $temp_min_date->format('Y-m-d');
          $formatted_max_date = $temp_max_date->format('Y-m-d');
          $return_data_array['date_from'] = $formatted_min_date;
          $return_data_array['date_to'] = $formatted_max_date > $formatted_min_date ? $formatted_max_date : date('Y-m-d');
          break;
        case '<': // only has a MAX date
          $temp_date = new DateTime($view->display_handler->handlers['filter']['periodending']->value['value']);
          $formatted_value_max = $temp_date->format('Y-m-d');
          $one_year_ago = $temp_date->sub(new DateInterval('P1Y'));
          $formatted_value_min = $one_year_ago->format('Y-m-d');
          $return_data_array['date_from'] = $formatted_value_min;
          $return_data_array['date_to'] = $formatted_value_max;
          break;
        case '=': // only has an EXACT date
          $temp_date = new DateTime($view->display_handler->handlers['filter']['periodending']->value['value']);
          $formatted_value = $temp_date->format('Y-m-d');
          $return_data_array['date_from'] = $formatted_value;
          $return_data_array['date_to'] = $formatted_value;
          break;
        case '>': // only has a MIN date
          $temp_date = new DateTime($view->display_handler->handlers['filter']['periodending']->value['value']);
          $formatted_value = $temp_date->format('Y-m-d');
          $return_data_array['date_from'] = $formatted_value;
          $plus_one_year = $temp_date->add(new DateInterval('P1Y'));
          $formatted_plus_one_year = $plus_one_year->format('Y-m-d');
          $today = date('Y-m-d');
          $return_data_array['date_to'] = $formatted_plus_one_year > $today ? $today: $formatted_plus_one_year;
          break;
        default:
          $return_data_array['date_from'] = $default_min_date;
          $return_data_array['date_to'] = $default_max_date > $default_min_date ?: date('Y-m-d');
          break;
      }
      // the api will only return 30 days if summed by hour
      if ($view->display_handler->get_option('trafsys_sum_by_day') == 'hour' ||
        $view->display_handler->get_option('trafsys_sum_by_day') == '') {
        $temp_compare_date = date('Y-m-d', strtotime("+1 months", strtotime($return_data_array['date_from'])));
        $return_data_array['date_to'] = $return_data_array['date_to'] > $temp_compare_date ? $temp_compare_date : $return_data_array['date_to'];
      }
      return ($return_data_array);
    }
    else {
      // return today's date here... $default_start = date('Y-m-d');
      $return_data_array['date_from'] = $default_min_date;
      $return_data_array['date_to'] = $default_max_date > $default_min_date ? $default_max_date : date('Y-m-d');
      // the api will only return 30 days if summed by hour
      if ($view->display_handler->get_option('trafsys_sum_by_day') == 'hour') {
        $temp_compare_date = date('Y-m-d', strtotime("+1 months", strtotime($return_data_array['date_from'])));
        $return_data_array['date_to'] = $return_data_array['date_to'] > $temp_compare_date ? $temp_compare_date : $return_data_array['date_to'];
      }
      return ($return_data_array);
    }
  }
}