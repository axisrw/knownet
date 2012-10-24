; $Id$

Community Tags Views
====================

All community tags views support is contained in this sub-module.

Tagging form attachment
------------------------

The tagging form views attachment allows you to add a tagging interface to
a view of community tags for a node. The entire view is refreshed via AJAX
in response to tagging actions.

The view it attaches to must have a node argument, and a vocabulary filter set to
a single vocabulary. If these conditions are not met the attachment will not display.

The term links or names can have checkboxes dynamically added by JS for one-click
tagging/un-tagging of existing tags. To enable this, add the class 'ct-tag' to
the term link or term name field that contains the term name text. You may need
to use field re-write or theming.

Caching Views Block with attachment
-----------------------------------

The only feasible Block Cache policy for block views with the tagging form attachment
is "per page, per user". I'm not sure what value there is in per page/per user
CT block caching - authcache is a much better solution. There is some support to selectively invalidate the
block cache on ajax tagging operations so that the view refreshes correctly.

TODO
----

1. Decide whether to continue support for block cache and if so make invalidation logic apply to non-ajax tagging ops.
2. More docs.



