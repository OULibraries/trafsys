<?php

class trafsys_views_plugin_display_extender extends views_plugin_display_extender {

  /**
   * Provide a form to edit options for trafsys plugin
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
// todo this may need to be $form_state['view']->base_table === 'trafsys'
    if ($form_state['section'] === 'trafsys') {

      // I'm setting the defaults here, because setting them the proper way isn't working
      // set the start date to today, unless it has already been set
      if ($form_state['view']->display_handler->options['trafsys_start_date']) {
        $default_start = $form_state['view']->display_handler->options['trafsys_start_date'];
      }
      else {
        $temp_date = new DateTime('1 week ago');
        $default_start = $temp_date->format('Y-m-d');
      }
      // set the end date to today, unless it has already been set
      if ($form_state['view']->display_handler->options['trafsys_end_date'] && $form_state['view']->display_handler->options['trafsys_end_date'] > $default_start) {
        $default_end = $form_state['view']->display_handler->options['trafsys_end_date'];
      }
      else {
        $default_end = date('Y-m-d');
      }
      // set the sum by to hour, unless it has already been set
      if ($form_state['view']->display_handler->options['trafsys_sum_by_day']) {
        $default_sum_by = $form_state['view']->display_handler->options['trafsys_sum_by_day'];
      }
      else {
        $default_sum_by = 'hour';
      }

      // User selects to have the totals grouped by the hour or by the day (hour is default)
      $form['trafsys_sum_by_day'] = array(
        '#type' => 'radios',
        '#title' => t('Summed Data'),
        '#description' => t('Select whether you want the data summed by Day or Hour.'),
        '#default_value' => $default_sum_by,
        '#options' => array(
          'hour' => t('Sum by Hour'),
          'day' => t('Sum by Day'),
        ),
      );
      $form['markup'] = array(
        '#markup' => '<br><hr>',
      );
      $form['markup1'] = array(
        '#markup' => '<p>These are the default date settings. If the Period Ending Filter (in Filter Criteria) is added, it will override what is set here.</p>',
      );
      $form['trafsys_start_date'] = array(
        '#type' => 'date_popup', //if you change this to date, you need to change the default on line 15
        '#date_format' => 'Y-m-d',
        '#title' => t('Trafsys Start Date'),
        '#description' => t('Enter the start date for this View\'s trafsys report'),
        '#default_value' => $default_start,
      );
      $form['markup2'] = array(
        '#markup' => '<br>',
      );
      $form['trafsys_end_date'] = array(
        '#type' => 'date_popup', //if you change this to date, you need to change the default on line 15
        '#date_format' => 'Y-m-d',
        '#title' => t('Trafsys End Date'),
        '#description' => t('Enter the end date for this View\'s trafsys report. If \'Sum By Hour\' is selected above, the date range must be one month or less.'),
//        '#default_value' => $this->display->get_option('trafsys_end_date'),
        '#default_value' => $default_end,
      );
    }
  }

  /**
   * Handle any special handling on the validate form.
   * If you don't set these, the defaults will overwrite themselves
   */
  function options_submit(&$form, &$form_state) {
    // It is very important to call the parent function here:
    parent::options_submit($form, $form_state);
    switch ($form_state['section']) {
      case 'trafsys':
        $this->display->set_option('trafsys_start_date', $form_state['values']['trafsys_start_date']);
        $this->display->set_option('trafsys_end_date', $form_state['values']['trafsys_end_date']);
        $this->display->set_option('trafsys_sum_by_day', $form_state['values']['trafsys_sum_by_day']);
        break;
    }
  }

  /**
   * Provide the default summary for options in the views UI.
   *
   * This output is returned as an array.
   */
  function options_summary(&$categories, &$options) {
    // It is very important to call the parent function here:
    parent::options_summary($categories, $options);

    $options['trafsys'] = array(
      'category' => 'other',
      'title' => t('Trafsys Options'),
      'value' => t('Options'),
      'desc' => t('This is the settings for Trafsys'),
    );
  }

}