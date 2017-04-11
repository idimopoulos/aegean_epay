<?php

/**
 * @file
 * aegean_epay.features.inc
 */

/**
 * Implements hook_eck_bundle_info().
 */
function aegean_epay_eck_bundle_info() {
  $items = array(
    'registration_registration' => array(
      'machine_name' => 'registration_registration',
      'entity_type' => 'registration',
      'name' => 'registration',
      'label' => 'Registration',
      'config' => array(
        'managed_properties' => array(
          'uid' => 'uid',
          'created' => 'created',
        ),
      ),
    ),
  );
  return $items;
}

/**
 * Implements hook_eck_entity_type_info().
 */
function aegean_epay_eck_entity_type_info() {
  $items = array(
    'registration' => array(
      'name' => 'registration',
      'label' => 'Registration',
      'properties' => array(
        'uid' => array(
          'label' => 'Author',
          'type' => 'integer',
          'behavior' => 'author',
        ),
        'created' => array(
          'label' => 'Created',
          'type' => 'integer',
          'behavior' => 'created',
        ),
      ),
    ),
  );
  return $items;
}
