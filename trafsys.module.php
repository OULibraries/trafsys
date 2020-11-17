<?php
/**
 * Created by PhpStorm.
 * User: Tim Smith
 * Date: 11/16/20
 * Time: 10:05 AM
 */


/**
 * Implements hook_views_api().
 */
function trafsys_views_api() {
    return array(
        'api' => 3.0
    );
}

/**
 * Implements hook_menu().
 */
function trafsys_menu() {
    /* proxy settings */
    $items['admin/config/system/trafsys']
        = array(
        'title' => 'Trafsys traffic counting settings',
        'description' => 'Configure settings for Trafsys reports',
        'page callback' => 'drupal_get_form',
        'page arguments' => array('trafsys_settings'),
        'access arguments' => array('administer trafsys settings'),
        'weight' => -10,
    );

    return $items;
}

// set the permissions for this module
function trafsys_permission() {
    $modperms = array(
        'administer trafsys settings' => array(
            'title' => t('Administer Trafsys reports'),
        ),
    );
    return $modperms;
}

// Settings for Trafsys: Username and Password @todo look for different ways to do this
function trafsys_settings() {
    $form = array();
    $form['trafsys_name'] = array(
        '#type' => 'textfield',
        '#title' => t('Trafsys Username'),
        '#default_value' => variable_get('trafsys_name', ""),
        '#description' => t("Email/Username for the Trafsys API"),
        '#required' => TRUE,
    );
    $form['trafsys_pw'] = array(
        '#type' => 'textfield',
        '#title' => t('Trafsys PW'),
        '#default_value' => variable_get('trafsys_pw', ""),
        '#description' => t("Password for the Trafsys API"),
        '#required' => TRUE,
    );

    return system_settings_form($form);
}