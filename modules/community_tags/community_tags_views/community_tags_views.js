// $Id$

/**
 * @file community_tags_views.js
 *
 * Handles AJAXiness of CT form attached to a view.
 */

/**
 * Handle the update tags event by refreshing the view response embedded in the CT response.
 */
$(document).bind('CT_SEND_TAGS_COMPLETE', function(e, form, data) {
 if(data.view) {
   // find display element to update
   // Call all callbacks.
   var response = data.view;

   var view_name = $('input[name=view_name]', this.form).val();
   var view_display_id = $('input[name=view_display_id]', this.form).val();

   if (view_name && view_display_id) {
     view = '.view-id-' + view_name + '.view-display-id-' + view_display_id;
     $(view).each(function() {
       var target = this;
       if (response.__callbacks) {
         $.each(response.__callbacks, function(i, callback) {
           eval(callback)(target, response);
         });
       }
     });
     $(view).each(function() {
       var target = this;
       if(data.messages) {
         $('.ct-messages', target).html(data.messages).fadeTo(0,0).css('visibility','visible').fadeTo(400,1);
         setTimeout(function(){ $('.ct-messages', target).fadeTo(400,0, function() {$(this).css('visibility', 'hidden'); $(this).html('Enter tags.');}) }, 3000);
       }
     });
   }
 }
});

Drupal.behaviors.communityTagsViews = function(context) {
  // intercept the community tags ajax form submission and add views query parameters to the POST.
  // this is needed to preserve other views ajax settings.
  $('input.form-tags', context).ajaxSend(function(e, xhr, settings) {
    if(settings.data) {
      var queryParams = Drupal.communityTags.parseQueryString(settings.data);
      // if(this == e.target && settings.type == 'POST') {
      if(this.form.id == queryParams.formId && settings.type == 'POST') {
        var action = this.form.action;

        // parse view parameters from action url to get any added by views ajax ops such as paging or filters
        var view_name = $('input[name=view_name]', this.form).val();
        var view_display_id = $('input[name=view_display_id]', this.form).val();
        var view_args = $('input[name=view_args]', this.form).val();
        var view_path = $('input[name=view_path]', this.form).val();
        var view_dom_id = $('input[name=view_dom_id]', this.form).val();
        var pager_element = $('input[name=pager_element]', this.form).val();

        var viewsQueryParams = {view_name: view_name, view_display_id: view_display_id, view_args: view_args, view_path: view_path, view_dom_id: view_dom_id, pager_element: pager_element};

        var viewsQueryString = Drupal.communityTags.serialize(viewsQueryParams);
        if (viewsQueryString.length > 0) {
          settings.data += "&" + viewsQueryString;
        }
      }
    }
  });
}
