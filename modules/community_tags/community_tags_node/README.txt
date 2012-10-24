; $Id$

OVERVIEW
========

This sub module synchronises community tags with node terms. This behaviour has
previously been included in the main community tags module. It now lives here because
one doesn't always want node terms to be synchronised with community tags. Or you may want any
terms added on the node edit form (i.e. node terms) to added as community tags, but not
vice versa. This module supports different synchronisation strategies via a number of
options.


OPTIONS
=======

There are currently three synchronisation options which affect how node terms are
synchronised with community tags and vice-versa. These settings are accessed by
following the "settings" link for an enabled community tags configuration on
the community tags settings page. The options are:

1. Synchronise community tags with node terms: When checked, the node is community
tagged for all new terms added to the node (e.g. via the node edit form). Likewise,
community tags are removed for any terms removed from the node. New community tags
are attributed to the current user.

2. Synchronise node terms with community tags: When checked, terms are added to
the node whenever the node is (community) tagged with new terms. Likewise, terms
are removed from the node when terms are no longer used to tag the node - i.e.
a term's tag count drops to zero.

3. Synchronise community tags with node terms: remove all user tags. When checked,
all users' community tags are removed for any terms removed from the node. If
not checked only the current user's tag is removed.


REBUILDING
==========

There may be times when the community tags and node terms are out-of-sync according
to the options set. This can happen if synchronisation options are changed but
could possibly be caused by other activity. When this happens the
status text displays the number of node terms and community tags that are out-of-sync and
a "rebuild" link appears which can be used to bring node terms and community tags back in
sync.

TO DO
=====

The most obvious one for me is to promote a tag to a node term only if the user has
a certain role or belongs to a certain group. Could be implemented with a permission
but should consider the need to vary between vocabularies/node types.


