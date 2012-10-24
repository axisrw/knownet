; $Id$

OVERVIEW
===========

This sub-module adds Authcache support to all community tag forms - even those attached to views. It works by stripping users tags from any CT forms in the page that is to be cached and then loading the users tags via AJAX. It makes good use of the browser cache so that AJAX calls are kept to a minumum.


USAGE
=======

To work you need the Authcache module installed and the following code added to ajax/authcache_custom.php in the authcache module folder (or along side settings.php?).

<?php
function _authcache_community_tags($vars) {
  include_once dirname(drupal_get_filename('module', 'community_tags_authcache')) . '/community_tags_authcache_callback.php';
  return community_tags_authcache_callback($vars);
}
?>


LIMITATIONS
=============

Known to work on community tag forms included in node pages. It could be adapted to work with multiple node displays...


NOTE
=======

For those not familiar with Authcache, it's basically an authenticated user page cache. Pages render in a few milliseconds (typically sub 10) with any user-dependent content (e.g. the users' Community Tags) loaded via ajax. It works well for anonymous users as well and if anonymous user support gets added to Community Tags will be nigh on indispensable.

