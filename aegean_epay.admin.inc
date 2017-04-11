<?php

/**
 * @file
 * Code for the Aegean ePay admin section.
 */

/**
 * Form constructor for the aegean epay settings form.
 *
 * @see aegean_epay_settings_validate()
 * @see aegean_epay_settings_submit()
 *
 * @ingroup forms
 */
function aegean_epay_settings($form, &$form_state) {
  $form['aegean_epay_endpoint'] = array(
    '#type' => 'textfield',
    '#title' => t('Service endpoint. Provided by the service.'),
    '#description' => t('Must be a URL.'),
    '#default_value' => variable_get('aegean_epay_endpoint', ''),
    '#required' => TRUE,
    '#weight' => 0,
  );

  $form['aegean_epay_project_shortcode'] = array(
    '#type' => 'textfield',
    '#title' => t('Project shortcode'),
    '#description' => t('The identifying shortcode for the aegean/project. Provided by the service.'),
    '#default_value' => variable_get('aegean_epay_project_shortcode', ''),
    '#required' => TRUE,
    '#weight' => 1,
  );

  $form['aegean_epay_project_salt'] = array(
    '#type' => 'textfield',
    '#title' => t('Project salt'),
    '#description' => t('The salt to be used by the hash function. Provided by the service.'),
    '#default_value' => variable_get('aegean_epay_project_salt', ''),
    '#required' => TRUE,
    '#weight' => 1,
  );

  $payment_description = "One entry per line. Each entry should be consisted "
    . "of four values split by '|'. The first value is the value 'Participant "
    . "el', the second value is the 'Participant en', the third value is the "
    . "amount of Euros, and the fourth value is the date in the format "
    . " YYYY-MM-DD."
    . "\ne.g. 'Μαθητης|Student|320.50|2017-03-27' to allow an option of 320.50 "
    . "Euros until 27th of March, 2017 to be recorded as students."
    . "\nNote that only one participant value will be saved depending on the "
    . "language that the user will set.";

  $form['aegean_epay_payment_options'] = array(
    '#type' => 'textarea',
    '#title' => t('Payment options'),
    '#description' => $payment_description,
    '#default_value' => aegean_epay_admin_get_payment_options(),
    '#required' => TRUE,
    '#weight' => 2,
  );

  $form['aegean_epay_help'] = array(
    '#type' => 'textarea',
    '#title' => t('Help text'),
    '#description' => t('This text will appear on the screen before the user is redirected to the Aegean epay system.'),
    '#default_value' => variable_get('aegean_epay_help', ''),
    '#required' => FALSE,
    '#weight' => 3,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 10,
  );

  return $form;
}

/**
 * Form validation handler for aegean_epay_settings().
 *
 * @see aegean_epay_settings()
 */
function aegean_epay_settings_validate($form, &$form_state) {
  $payment_options = aegean_epay_parse_payment_options($form_state['values']['aegean_epay_payment_options']);
  foreach ($payment_options as $option) {
    if (empty($option['participant_el']) || !is_string($option['participant_el'])) {
      form_set_error('aegean_epay_payment_options', 'The "Participant el" value must be a valid string.');
    }
    if (empty($option['participant_en']) || !is_string($option['participant_en'])) {
      form_set_error('aegean_epay_payment_options', 'The "Participant en" value must be a valid string.');
    }
    if (empty($option['amount']) || !is_numeric($option['amount'])) {
      form_set_error('aegean_epay_payment_options', 'The "Amount" value must be numeric.');
    }
    if (empty($option['deadline']) || !preg_match("/^[0-9]{4}-(0[1-9]|1[0-2])-(0[1-9]|[1-2][0-9]|3[0-1])$/", $option['deadline'])) {
      form_set_error('aegean_epay_payment_options', 'The "Deadline" value must be in the format of YYYY-MM-DD.');
    }
  }
}

/**
 * Form submission handler for aegean_epay_settings().
 *
 * @see aegean_epay_settings()
 */
function aegean_epay_settings_submit($form, $form_state) {
  variable_set('aegean_epay_endpoint', $form_state['values']['aegean_epay_endpoint']);
  variable_set('aegean_epay_project_salt', $form_state['values']['aegean_epay_project_salt']);
  variable_set('aegean_epay_project_shortcode', $form_state['values']['aegean_epay_project_shortcode']);
  $payment_options = aegean_epay_parse_payment_options($form_state['values']['aegean_epay_payment_options']);
  // Options will be turned into objects.
  variable_set('aegean_epay_payment_options', json_encode($payment_options));
  variable_set('aegean_epay_help', $form_state['values']['aegean_epay_help']);
  drupal_set_message("Settings saved.");
}

/**
 * Returns the payment options as options for a textarea form element.
 *
 * @return string
 *   The string version of the options.
 */
function aegean_epay_admin_get_payment_options() {
  $return = array();
  $options = aegean_epay_load_payment_options();
  foreach ($options as $option) {
    $return[] = implode('|', $option);
  }

  return implode("\n", $return);
}
