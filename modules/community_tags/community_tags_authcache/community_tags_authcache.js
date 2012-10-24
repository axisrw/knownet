// $Id$
// Callback function from Ajax request
function _authcache_community_tags(vars) {
  // for each CT "my tags" textfield set the tags from the ajax response and attach CT behaviours.
  $("input.form-tags").each(function(i, el) {
    var vid = $('input[name=vid]', this.form).val();
    var nid = $('input[name=nid]', this.form).val();
    var nid_vid = nid+'_'+vid;
    if(vars['tags'][nid_vid]) {
      // get the tags and set the form field value.
      var tags = vars['tags'][nid_vid];
      $('input[name=tags_refs]', this.form).val(tags);

      // authcached pages with CT forms have ct-processed class preset to prevent JS processing
      // until user tags are loaded. Remove the class and re-attach CT behaviours.
      $(this).removeClass('ct-processed');
      Drupal.attachBehaviors(this.form);
    }
  });
}

// Get the users's mytags via ajax call. Response will be cached by browser but is
// invalidated if the page is new by including Authcache.info.cache_time in request.
// The authcached node page is refreshed on the server when any tags are added to
// or removed from the node so external changes to a users' my tags (e.g. term deleted
// or tags removed via tag management/moderation) will be got.
function authcacheCommunityTagsInit() {
  if($("input.form-tags").length) {
    $("input.form-tags").each(function(i, el) {
      var form_id = this.form.id;
      var vid = $('input[name=vid]', this.form).val();
      var nid = $('input[name=nid]', this.form).val();
      var nid_vid = nid+'_'+vid;
      ajaxJson = {
        'community_tags' : 1,
        'ct' : nid_vid,
        'q' : Authcache.ajax.q,
        'max_age' : 86400,
        'page_time' : Authcache.info.cache_time,
        'uid' : $.cookie('drupal_uid')
      };
      // Perform independent Authcache ajax request
      Authcache.ajaxRequest(ajaxJson);
    });
  }
}

jQuery(function() { authcacheCommunityTagsInit(); })
