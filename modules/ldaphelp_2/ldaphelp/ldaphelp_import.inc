<?php

/**
 * @file
 * import functionality file for ldaphelp module
 *
 */

function ldaphelp_import_form(&$form_state = NULL) {

  $instructions['1'] = t('Step 1 of 3. Paste your LDAP export into the box below.  On the next sceen you will
  be able select which parts of it you would like to import.');

  $instructions['2'] = t('Step 2 of 3. You are encouraged to back up any LDAP settings you have before using this.
  Check the items you would like to import below.');

  $instructions['3'] = t('Step 3 of 3.');

  $step = isset($form_state['values']) ? (int) $form_state['storage']['step'] : 0;
  $form_state['storage']['step'] = $step + 1;

  if ($step) {
    drupal_set_message($instructions[$step]);
  }

  $current_conf_data = _ldaphelp_get_configuration(TRUE, NULL);
  $default_ldap_imports = array();
  $import_admin_settings = 0;

  switch ($step) {
    case 1:

    $form['import_data_serialized'] = array(
      '#type' => 'textarea',
      '#title' => $export_summary,
      '#default_value' => $form_state['values']['import_data_serialized'],
      '#cols' => 80,
      '#rows' => 5,
      '#required' => FALSE,
      '#attributes' => array('WRAP' => 'SOFT'),
      '#description' => NULL,
    );

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Import Settings'),
    );
    break;

    case 2:
      $form_state['storage']['import_data_serialized'] = ldaphelp_stripcomments(trim($form_state['values']['import_data_serialized']));
      $import_data = unserialize($form_state['storage']['import_data_serialized']);

      $form['import_data_serialized'] = array(
        '#type' => 'hidden',
        '#default_value' => $form_state['storage']['import_data_serialized'],
      );

        if ($import_data['admin_settings']) {

          if ($current_conf_data['admin_settings']) {
            $description = t('Your current admin setting will be replaced.' ) .'<br/>'. t('Replace') .': '.
            ldaphelp_arraytohtmllist($current_conf_data['admin_settings']) . t('With') .':'.
            ldaphelp_arraytohtmllist($import_data['admin_settings']);
          }
          else {
            $description = t("Your current admin setting will be set to:") .
            ldaphelp_arraytohtmllist($import_data['admin_settings']);
          }

          $form['import_admin_settings_checkbox'] =  array(
            '#type' => 'checkbox',
            '#title' => t('Import Admin Settings'),
            '#return_value' => 1,
            '#default_value' => $form_state['values']["import_admin_settings_checkbox"],
            '#description' => $description ,
          );
        }
        if ($import_data['ldaps']) {
          foreach ($import_data['ldaps'] as $sid => $ldap) {
            // could be update or addition of server.
            // need to make clear.
            unset($import_data['ldaps'][$sid]['sid']);
            $description = t('This will add another LDAP server.  It will not replace your existing ones.') .
              ldaphelp_arraytohtmllist($import_data['ldaps'][$sid]);

            $title = t("Import LDAP Server <code>") . $ldap['name'] ."</code>";
            $form["importldapcheckbox-$sid"] =  array(
              '#type' => 'checkbox',
              '#title' => $title,
              '#return_value' => 1,
              '#default_value' => $form_state['values']["importldapcheckbox-$sid"],
              '#description' => $description,
            );
          }
        }
        $form['submit'] = array(
          '#type' => 'submit',
          '#value' => t('Import Checked Items'),
        );
      break;

    case 3:

      $import_data = unserialize(trim($form_state['storage']['import_data_serialized']));

      if ($form_state['values']['import_admin_settings_checkbox']) {
        $imports[] = "Admin Settings";
        $import_admin_settings = 1;
      }
      $import_sids = array();
      foreach ($form_state['values'] as $fieldname => $value ) {
        if (drupal_substr($fieldname, 0, 10) == 'importldap' && $value == 1 ) {
          $parts = split('-', $fieldname);
          $import_sid  = $parts[1];
          $import_sids[] = $import_sid ;
          $imports[] =  "LDAP ". $import_data['ldaps'][$export_sid]['name'];
        }
      }
      if (is_array($imports)) {
        $export_summary .= join(', ', $imports) ."</code>";
      }

      if ($import_admin_settings || count($import_sids)) {
        if ($import_admin_settings == 1) {
          $ldapauth_admin_settings_form_state['values'] = $import_data['admin_settings'];
          $ldapauth_admin_settings_form_state['clicked_button']['#value'] = "Save configuration";
          drupal_execute('ldapauth_admin_settings', $ldapauth_admin_settings_form_state);
          if (! form_get_errors()) {
            drupal_set_message('Values:<br/>'. ldaphelp_arraytohtmllist($import_data['admin_settings']) );
          }
          else {
            drupal_set_message(t('Skipped updating Admin Settings because of errors.'), 'error');
          }
        }
        foreach ($import_sids as $sid) {
          $name = $import_data['ldaps'][$sid]['name'];
          $ldapauth_ldap_settings_form_state['values'] = $import_data['ldaps'][$sid];
          unset($ldapauth_ldap_settings_form_state['values']['sid']); // adding, not updating.
          drupal_execute('ldapauth_admin_form',  $ldapauth_ldap_settings_form_state);
          if (! form_get_errors() ) {
            drupal_set_message( ldaphelp_arraytohtmllist($ldapauth_ldap_settings_form_state['values']) );
            if ($ldapauth_ldap_settings_form_state['values']['binddn']) {
              drupal_set_message("Bind DB field of <code>$name</code>imported, but password needs to be set also.", 'warning');
            }
          }
          else {
            drupal_set_message("Skipped Adding LDAP Server: <code>$name</code> because of errors.", 'error');
          }
        }
      }
      else {
        drupal_set_message('Nothing selected to import.  Check something.', 'warning');
        $form_state['storage']['step'] =  $form_state['storage']['step'] - 1;
      }
      break;

  }
  return $form;
}

function ldaphelp_import_form_validate($form, &$form_state) {

  switch ($form_state['storage']['step']) {

    case 2:
      $import_data_serialized = ldaphelp_stripcomments(trim($form_state['values']['import_data_serialized']));
      if (! $import_data_serialized) {
        $haserror = TRUE;
      }
      else {
        $import_data = unserialize($import_data_serialized);
        if (! is_array($import_data)) {
          $haserror = TRUE;
          }
        elseif ( ! ($import_data['ldaps'] ||  $import_data['admin_settings'])) {
          $haserror = TRUE;
        }
      }

      if ($haserror) {
        $err = $note .  t("Input not valid.  Input should be a serialized array from the export function.
        It should look something like: ") .'<code>a:1:{s:5:"ldaps";a:1:{i:1;a:1</code>...';
        drupal_set_message($err, 'error');
        $form_state['storage']['step'] =  $form_state['storage']['step'] - 1;
        return FALSE;
      }
      break;
  }
}

function ldaphelp_stripcomments($text) {
  $parts = split("\*\/", $text);
  $serialized_array = (count($parts) == 0) ? trim($parts[0]) : trim($parts[1]);
  return trim($serialized_array);
}



