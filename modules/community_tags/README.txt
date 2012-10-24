; $Id$

OVERVIEW
========

Community Tags module allows users with the proper permissions to tag other users' content. An AJAX powered UI is provided to make tagging content quick and easy. Tagadelic support can visually highlight the most popular tags.


INSTALLATION
============

1. Download and enable the Community Tags module in the usual way.

2. Community Tags ships with a number of optional sub-modules. Enable these as required.

3. For Tagadelic display of "All Tags", install the Tagadelic module.


CONFIGURATION
=============

1. Create a free tagging vocabulary for your content type(s) by going to:

     Administer > Content managment > Taxonomy

   On the "edit vocabulary" pages of the vocabularies you wish to use for community tagging, select the content type(s) to tag, and check the "Tags" setting.

2. Enable and configure community tagging for each combination of content type and free tagging vocabulary by going to:

     Administer > Site Configuration > Community tags

3. Select how each content type should display the community tagging form by going to:

     Administer > Content > Content Types

   Note: The community tagging settings can be found in the "Workflow settings" section of the content type settings form. They will only be visible if a community tags enabled vocabulary is assigned to the content type. See steps 1 and 2 above!

4. Set permissions for tagging content and editing tags by going to:

     Administer > Access control


BLOCK CACHE
=============

Block caching is supported (per page/per user only) but is not enabled by default.

To enable Community Tags block caching:

1. Use the Block Cache Alter module (http://drupal.org/project/blockcache_alter) or update the "block" database table (block table, cache column value should be 6).

2. Ensure Community Tags javascript and CSS is added to pages where the CT block is displayed. To do this call _community_tags_add_js() and _community_tags_add_css() e.g. from a suitable place in your theme.

3. Check cache invalidation requirements below...


Block cache invalidation
------------------------

If "community tag to node term" node synchronisation is enabled (via the Community Tags Node sub-module) the entire cache (including the CT block cache) will be cleared every time content is tagged or un-tagged.
This may be a good reason not to enable "community tag to node term" synchronisation!

If "community tag to node term" node synchronisation is NOT enabled, the block cache can be selectively invalidated in response to tagging ops by
implementing the hook_community_tags_tag(), and hook_community_tags_untag() hooks and calling _community_tags_community_tags_block_cache_invalidate($node).


NOTES
=====


WORKFLOW
========

This is how Community Tags responds to various workflow events.

Community Tags operations
-------------------------

### Tag removed ###

Remove redundant terms ("Delete redundant terms" mode only)

Node operations
---------------

For synchronisation between community tags and node terms refer to the Community Tags Node sub-module.

### Node delete ###

1. All community tags records are removed for the deleted node.

2. Remove redundant terms ("Delete redundant terms" mode only)


User operations
---------------

### User delete ###

All community tags are removed for the deleted user. Where the deleted user's tags were the only tag for the combination of node/term then:

1. Remove redundant terms ("Delete redundant terms" terms mode only)

(NB thinking of having the option of moving tags to a "dead" user rather than
lose potentially valuable tagging activity).


Taxonomy operations
-------------------

### Term delete ###

All community tags are removed for the deleted term. No synchronisation issues.


Configuration changes
----------------------

Configuration changes may leave invalid community tags in the database. When
these are detected the CT configuration page will display a message with the option to remove them.


AUTHOR INFORMATION
==================

Tagadelic was originally written by Bèr Kessels and more information can be
found on http://www.webschuur.com/modules/tagadelic.

Carpentered in the webschuur.com

Additional modifications including weighted node tagging, the quick tag form and
user tags have been authored by Rob Barreca of Electronic Insight and more
information can be found at http://www.electronicinsight.com.

Cherry-picking from Rob Barreca's work and using it to create the Community Tags
module was done by Angela Byron of Lullabot with support from the Bryght guys.

Steven Wittens of Bryght updated this module to 5.x, cleaned up the code some more
and added more shiny features.

Andy Chapman - Partial rewrite for 1.x. Big rewrite for 2.x...