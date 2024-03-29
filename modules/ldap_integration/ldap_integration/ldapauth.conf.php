<?php
// $Id: ldapauth.conf.php,v 1.2.4.2 2008/07/19 23:14:10 miglius Exp $

/**
 * @file
 * ldapauth module configuration options.
 */

/**
 * Transform the login name to something understood by the server.
 */
function ldapauth_transform_login_name($login_name) {
  return $login_name;
}

/**
 * Let users in (or not) according to some attributes' values
 * (and maybe some other reasons).
 */
function ldapauth_user_filter(&$attributes) {
  // Uncomment next line to see how the argument array looks like
  // msg_r($attributes);

  // Example: don't allow in users with no homeDirectory set.
  // return isset($attributes['homeDirectory'][0]) && $attributes['homedirectory'][0];

  return TRUE;
}

