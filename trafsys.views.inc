<?php
/**
 * Created by PhpStorm.
 * User: Tim Smith
 * Date: 11/16/20
 * Time: 10:06 AM
 */

/**
 * Implements hook_views_plugins().
 */
function trafsys_views_plugins() {
    $plugin = array();
    $plugin['query']['trafsys_plugin_query'] = array(
        'title' => t('Trafsys Query'),
        'help' => t('Trafsys query object.'),
        'handler' => 'trafsys_plugin_query',
    );
    return $plugin;
}

/**
 * Implements hook_views_data().
 *
 * the actual fields from which the user can choose in the view
 */
function trafsys_views_data() {
    $data = array();

    //Base data
    $data['trafsys']['table']['group'] = t('Trafsys');
    $data['trafsys']['table']['base'] = array(
        'title' => t('Trafsys'),
        'help' => t('Query Trafsys'),
        'query class' => 'trafsys_plugin_query'
    );

    //Fields
    $data['trafsys']['site_code'] = array(
        'title' => t('Site Code'),
        'help' => t('String containing the alphanumeric code that identifies the site within your organization.'),
        'field' => array(
            'handler' => 'trafsys_handler_field',
        ),
    );
    $data['trafsys']['location'] = array(
        'title' => t('Location'),
        'help' => t('A string containing the name of the location where the sensors are counting.'),
        'field' => array(
            'handler' => 'trafsys_handler_field',
        ),
    );
    $data['trafsys']['isinternal'] = array(
        'title' => t('Is Internal'),
        'help' => t('A boolean flag indicating if the location is internal (true) or belongs to the perimeter (false.) Internal sensors should not count towards the sites total traffic for the day, as those counts should be recorded at the perimeter sensors.'),
        'field' => array(
            'handler' => 'trafsys_handler_field',
        ),
    );
    $data['trafsys']['periodending'] = array(
        'title' => t('Period Ending'),
        'help' => t('A date/time containing the time that marks the end of the counting period. For hourly periods, these will be [date] [time]. For daily periods, the time will be zero, and the traffic should be considered to be counted on that day.'),
        'field' => array(
            'handler' => 'trafsys_handler_field',
        ),
    );
    $data['trafsys']['ins'] = array(
        'title' => t('Ins'),
        'help' => t('An integer recording the in counts for that time period and location.'),
        'field' => array(
            'handler' => 'trafsys_handler_field_date',
        ),
    );
    $data['trafsys']['outs'] = array(
        'title' => t('Outs'),
        'help' => t('An integer recording the out counts for that time period and location.'),
        'field' => array(
            'handler' => 'trafsys_field_option',
        ),
    );

    return $data;
}