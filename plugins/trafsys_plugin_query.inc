<?php
/**
 * Created by PhpStorm.
 * User: Tim Smith
 * Date: 11/16/20
 * Time: 10:02 AM
 */

class trafsys_plugin_query extends views_plugin_query {

  function execute(&$view) {

    if (!is_object($view)) { return; }

    //setting server variable
    $trafsysEmail = variable_get('trafsys_name', "");
    $trafsysPassword = variable_get('trafsys_pw', "");
    $data_array = array();
    $dates = new date_input;
    $data_array['dates'] = $dates->returnThis($view);

    // This should default to hour...which would be false
    if ($view->display_handler->get_option('trafsys_sum_by_day') == 'day') {
      $data_array['sum_by'] = 'true';
    }
    else {
      $data_array['sum_by'] = 'false';
    }

    // leaving like this in case this is used with PHP < 7
    if ($view->display_handler->handlers['filter']['site_code']->value) {
      $data_array['site_code'] = $view->display_handler->handlers['filter']['site_code']->value;
    }
    else {
      $data_array['site_code'] = '';
    }

    // Parse through multiple locations entered into the location filter
    if ($view->display_handler->handlers['filter']['location']->value) {
      $temp_location = $view->display_handler->handlers['filter']['location']->value;
      $data_array['location'] = array_map('trim', explode(',', $temp_location));
    }
    else {
      $data_array['location'] = '';
    }

//    if (!array_key_exists('isinternal', $view->display_handler->handlers['filter'])) { return; }
//    if (!is_object($view->display_handler->handlers['filter']['isinternal'])) { return; }
    // Some sensors are not exterior doors...they are internal
    if ($view->display_handler->handlers['filter']['isinternal']->value == 0) {
      $data_array['show_interior'] = 'false';
    }
    else {
      $data_array['show_interior'] = 'true';
    }
    // throw a watchdog error if there is no Trafsys email or password
    if ($trafsysEmail == '' || $trafsysPassword == '') {
      watchdog('trafsys_data', 'No Trafsys email and/or password entered in configuration.', []);
      return;
    }

    // Get the Trafsys Token
    $num_of_attempts = 5;
    $attempts = 0;

    do {
      try {
        $trafsysTokenData = $this->GetToken($trafsysEmail, $trafsysPassword);
        if ($trafsysTokenData->code != 200) {
          throw new Exception("Message: " . $trafsysTokenData->data);
        }

        $trafsysJson = json_decode($trafsysTokenData->data, TRUE);
        $data_array['trafsys_access_token'] = $trafsysJson['access_token'];
        $trafsysTrafficData = $this->getTrafficData($data_array);

        if ($trafsysTrafficData->code != 200) {
          throw new Exception("Message: " . $trafsysTrafficData->data);
        }

        $trafsysReturnedJson = json_decode($trafsysTrafficData->data);
        $this->processData($trafsysReturnedJson, $data_array, $view);

      } catch (Exception $e) {
//        drupal_set_message($e->getMessage()); uncomment this to debug
        error_log($e->getMessage());
        if (strpos($e->getMessage(), 'API calls quota exceeded') !== FALSE) {
          $attempts ++;
          sleep(1);
          continue;
        }
        else {
          $attempts = 5;
        }
      }
      $attempts = 5;
    } while($attempts < $num_of_attempts);
  }

  // Hit the Trafsys API the first time to get the Token
  function GetToken($email, $pw) {
    $data = [
      'grant_type' => 'password',
      'username' => $email,
      'password' => $pw,
    ];

    $options = [
      'method' => 'POST',
      'data' => drupal_http_build_query($data),
      'timeout' => 15,
      'headers' => ['Content-Type' => 'application/x-www-form-urlencoded'],
    ];
    // If Trafsys ever changes their API URL we will need to change it here
    $result = drupal_http_request('https://portal.trafnet.com/rest/token', $options);
    return ($result);
  }

  // Get all the data from the Trafsys API
  function getTrafficData($data) {
    $headers = [
      'Authorization'=> 'Bearer ' . $data['trafsys_access_token'],
      'Content-Type' => 'application/json',
    ];

    $parameters = [
      'SiteCode' => $data['site_code'],
      'DateFrom' => $data['dates']['date_from'],
      'DateTo' => $data['dates']['date_to'],
      'IncludeInternalLocations' => $data['show_interior'],
      'DataSummedByDay' => $data['sum_by'],
    ];

    $options = [
      'method' => 'GET',
      'data' => drupal_http_build_query($parameters),
      'timeout' => 15,
      'headers' => $headers,
    ];
    $url = url('https://portal.trafnet.com/rest/api/traffic', array('query' => $parameters));
    $result = drupal_http_request($url, $options);
    return ($result);
  }

  // loop through the data and assign it to fields in the View
  function processData($data, $data_array, $view) {
    $total = 0;
    foreach ($data as $d) {
      if (($data_array['show_interior'] == 'false') && ($d->IsInternal == 'TRUE')) { continue; } // do I need this if it's included in the params?
      // this little diddy compares (in lowercase) the returned "location" to the array of filter "locations"
      if ($data_array['location'] != '' && !in_array(strtolower($d->Location), array_map('strtolower', $data_array['location']))) { continue; }
      $row = new stdClass();
      $row->site_code = $d->SiteCode;
      $row->location = $d->Location;
      $row->isinternal = $d->IsInternal;
      $row->periodending = $d->PeriodEnding;
      $row->ins = $d->Ins;
      $row->outs = $d->Outs;
      $total = $total + $d->Ins;
      if ($d === end($data)) {
        $row->total = $total;
      }
      $view->result[] = $row;
    }
  }
}