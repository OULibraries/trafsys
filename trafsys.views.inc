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
  $path = drupal_get_path('module', 'trafsys');
  $plugin = [];
  $plugin['query']['trafsys_plugin_query'] = [
    'title' => t('Trafsys Query'),
    'help' => t('Trafsys query object.'),
    'handler' => 'trafsys_plugin_query',
  ];
  $plugin['display_extender'] = [
    'trafsys' => [
      'title' => t('Sum Total by Location'),
      'help' => t('Select this option if you want the chart to group locations together'),
      'path' => $path,
      'handler' => 'trafsys_views_plugin_display_extender',
    ],
  ];
  return $plugin;
}

/**
 * Implements hook_views_data().
 *
 * the actual fields from which the user can choose in the view
 */
function trafsys_views_data() {
  $data = [];
  //Base data
  $data['trafsys']['table']['group'] = t('Trafsys');
  $data['trafsys']['table']['base'] = [
    'title' => t('Trafsys'),
    'help' => t('Query Trafsys'),
    'query class' => 'trafsys_plugin_query',
  ];

  //Fields
  $data['trafsys']['site_code'] = array(
    'title' => t('Site Code'),
    'help' => t('String containing the alphanumeric code that identifies the site within your organization.'),
    'field' => array(
      'handler' => 'trafsys_handler_field',
    ),
    'filter' => array(
      'handler' => 'trafsys_handler_filter_string',
    ),
  );
  $data['trafsys']['location'] = array(
    'title' => t('Location'),
    'help' => t('A string containing the name of the location where the sensors are counting.'),
    'field'  => array(
      'handler' => 'trafsys_handler_field',
    ),
    'filter' => array(
      'handler' => 'trafsys_handler_filter_string',
    ),
  );
  $data['trafsys']['isinternal'] = array(
    'title' => t('Is Internal'),
    'help' => t('A boolean flag indicating if the location is internal (true) or belongs to the perimeter (false.) Internal sensors should not count towards the sites total traffic for the day, as those counts should be recorded at the perimeter sensors.'),
    'field' => array(
      'handler' => 'trafsys_handler_field',
    ),
    'filter' => array(
      'handler' => 'trafsys_handler_filter_bool',
      'label' => t('Show Internal Locations?'),
      'type' => 'yes-no',
    ),
  );
  $data['trafsys']['periodending'] = array (
    'title' => t('Period Ending'),
    'help' => t('A date/time containing the time that marks the end of the counting period. For hourly periods, these will be [date] [time]. For daily periods, the time will be zero, and the traffic should be considered to be counted on that day.'),
    'field' => array(
      'handler' => 'trafsys_handler_field',
   ),
    'filter' => array(
      'handler' => 'trafsys_handler_filter_date',
    ),
  );
  $data['trafsys']['ins'] = [
    'title' => t('Ins'),
    'help' => t('An integer recording the in counts for that time period and location.'),
    'field' => [
      'handler' => 'trafsys_handler_field',
    ],
  ];
  $data['trafsys']['outs'] = [
    'title' => t('Outs'),
    'help' => t('An integer recording the out counts for that time period and location.'),
    'field' => [
      'handler' => 'trafsys_handler_field',
    ],
  ];

  return $data;
}