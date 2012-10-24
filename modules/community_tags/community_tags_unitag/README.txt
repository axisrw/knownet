; $Id$

OVERVIEW
========

Unitag helps to keep a taxonomy clean, consistent, and relevant. As well as cleaning, de-duplicating, and filtering tags automatically, it can be configured to place all new tags in a moderation queue. It is the perfect partner to community tagging, dealing with most of the issues that are encountered when allowing a large set of users to tag content.

This sub-module integrates Community tags with Unitag.


FEATURES
========

* Synonym replacement. If a user enters a tag which is a synonym of another tag, the tag is replaced with the synonym.
* Moderation of new tags. New tags may be queued for moderation. Whilst queued the user will see their pending tags in the "my tags" list. If approved their new tag will also appear in the "all tags" list.
* Tag moderation can be set for all users/untrusted users/or never and is independent of the Unitag read-only setting.
* Tag sanitisation - remove whitespace and optionally convert all to lower case.
* Tag blacklist. Add inappropriate tags to a blacklist. Tagging is not permitted with blacklisted terms.
* Works independently of node terms.
* Maintains unitag queue referential integrity.
* Handles removal of tags pending moderation. Removed tags are removed from the unitag moderation queue if not suggested by anyone else.


TO USE
======

* Install and set up Unitag as desired.
* Install this module.
* With this module enabled, Unitag processing is enabled by default for every enabled vocabulary/content type pair.
* Unitag processing can be disabled via the settings page for each vocabulary/content type pair.
* Moderation etc is via the Unitag management page.
* Moderation status is provided on the community tags settings page and if required a link to the Unitag moderation queue.


LIMITATIONS
===========

If a taxonomy is set to Unitag read-only mode then terms added via the node edit form that should also be added as a community tag via node term synchronisation will not be added as node terms if approved, unless reverse node term synchronisation is enabled.


TO DO
=====

* Feedback: Via ajax, provide feedback to the tagger if their tag was not directly added. E.g. queued for moderation, tagged with synonym, tagged with synonym and related terms, blacklisted term.
* Notification: For admin, e.g. terms awaiting moderation. For users, your tag was blacklisted or not approved etc. Probably do this via the admin status callback.