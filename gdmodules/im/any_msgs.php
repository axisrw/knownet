<?php
// $Id: any_msgs.php,v 1.1.2.1 2009/04/13 04:55:13 pahariwalla Exp $

/**
 * @file
 * checks for new msgs and returns either 'y' or 'n'
 */

require_once './includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_DATABASE, 2);

$ruid = $_GET['ruid'] ? $_GET['ruid'] : 0;
$suid = $_GET['suid'] ? $_GET['suid'] : 0;

$result = db_result(db_query("SELECT count(*) FROM {im_msg} WHERE suid = %d AND ruid = %d AND received_time = '0000-00-00 00:00:00'", $suid, $ruid));
if ($result > 0) {
  echo 'y';
}
else {
  echo 'n';
}

?>