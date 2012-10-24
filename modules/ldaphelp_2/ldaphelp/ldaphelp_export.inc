<?php
/**
 * @file
 * export functionality file for ldaphelp module
 *
 */
function ldaphelp_export_form() {

  $all_current_data = _ldaphelp_get_configuration(TRUE, NULL);
  $default_ldap_exports = array();

  $export_admin_settings = 0;
  $instructions = t('Check the items you would like to export below.  This will simply create
  a serialized array in a text area. You must cut and paste it somewhere; the export is not stored.
  It can later be imported or shared for debugging and support purposes.');


  if ($_SESSION['ldaphelp_export_last_form_state_values'] ) {
    $exports = array();
    $last_values = unserialize($_SESSION['ldaphelp_export_last_form_state_values']);
    $export_admin_settings = $last_values['export-admin-settings'];

    $export_summary = "Export of:<br/><code class='export-summary'>";
    if ($last_values['export-admin-settings']) {
      $exports[] = "Admin Settings";
    }

    $export_sids = array();
    foreach ($last_values as $fieldname => $value ) {
      if (drupal_substr($fieldname, 0, 10) == 'exportldap' && $value == 1 ) {
        $parts = split('-', $fieldname);
        $export_sid  = $parts[1];
        $export_sids[] = $export_sid ;
        $exports[] =  "LDAP ". $all_current_data['ldaps'][$export_sid]['name'] ." (sid=$export_sid)" ;

        $default_ldap_exports[$export_sid] = 1;
      }
    }
    $export_summary .= join(', ', $exports) ."</code>";
    if ($export_admin_settings || count($export_sids)) {
      $export_conf_data = _ldaphelp_get_configuration($last_values['export-admin-settings'], $export_sids);
      $export_comments = "\r\n/** begin comments \r\n". ldaphelp_get_export_comments($export_conf_data) ."\r\n end comments **/\r\n";
      $form['export'] = array(
        '#type' => 'textarea',
        '#title' => $export_summary,
        '#default_value' => $export_comments . serialize($export_conf_data),
        '#cols' => 80,
        '#rows' => 10,
        '#description' => t('Copy and past this into a text file for future use or to share with others.  The import option takes this as its input.'),
      );
    }
    else {
      drupal_set_message('Nothing selected to export.  Check something.', 'warning');
    }
    unset($_SESSION['ldaphelp_export_last_form_state_values']);

  }
  else {
    drupal_set_message($instructions);
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Export Checked Items Below'),
  );

  $form['export-admin-settings'] =  array(
    '#type' => 'checkbox',
    '#title' => t('Export Admin Settings'),
    '#return_value' => 1,
    '#default_value' => $export_admin_settings,
    '#description' => '',
  );

  foreach ($all_current_data['ldaps'] as $sid => $ldap) {
    $title = t("Export Server: <code>") . $ldap['name'] ."</code> (sid=$sid)";
    $form["exportldap-$sid"] =  array(
      '#type' => 'checkbox',
      '#title' => $title,
      '#return_value' => 1,
      '#default_value' => $default_ldap_exports[$sid],
      '#description' => '',
    );
  }
  return $form;
}

/**
 * Submit hook for the export form.
 */
function ldaphelp_export_form_submit($form, &$form_state) {
  $_SESSION['ldaphelp_export_last_form_state_values'] = serialize($form_state['values']);
}



















