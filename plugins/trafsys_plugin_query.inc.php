<?php
/**
 * Created by PhpStorm.
 * User: Tim Smith
 * Date: 11/16/20
 * Time: 10:02 AM
 */

class trafsys_plugin_query extends views_plugin_query
{
    function execute(&$view)
    {
        //setting server variable
        $trafsysEmail = variable_get('trafsys_email', "");
        $trafsysPassword = variable_get('trafsys_email', "");
        if ($trafsysEmail == '' || $trafsysPassword == '') {
            watchdog('trafsys_data', 'No Trafsys email and/or password entered in configuration.', []);
            return;
        }

        // todo set the variables in the gui and retrieve them here
//        $someVariable = $this->options['somevariable'];

        $trafsysData = GetTrafsysData();

        $trafsysDataArray = array();

        /**
         * looping through the returned data to set some values
         */
        foreach ($trafsysData as $data) {
            if (isset($location->counts)) {
                dpm("loop" . $data);
            }
        }
//        foreach ($suma_ag_data as $coords => $count) {
//            $row = new stdClass();
//            $temp_row = explode("|", $coords);
//            array_push($temp_row, $count);
//            $row->date = strtotime($temp_row[2]);
//            $row->title = $temp_row[0];
//            $row->id = $temp_row[1];
//            $row->count = $temp_row[4];
//            $row->activity = $temp_row[3];
//            $view->result[] = $row;
//        }
    }

    function GetTrafsysData() {
        dpm("the get trafsys data function");
    }
}