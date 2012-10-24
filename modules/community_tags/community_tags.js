// $Id$

Drupal.communityTags = {};

Drupal.communityTags.ctSequence = 0;

Drupal.communityTags.checkPlain = function (text) {
  text = Drupal.checkPlain(text);
  return text.replace(/^\s+/g, '')
             .replace(/\s+$/g, '')
             .replace('\n', '<br />');
}

Drupal.communityTags.serialize = function (data, prefix) {
  prefix = prefix || '';
  var out = '';
  for (i in data) {
    var name = prefix.length ? (prefix +'[' + i +']') : i;
    if (out.length) out += '&';
    if (typeof data[i] == 'object') {
      out += Drupal.communityTags.serialize(data[i], name);
    }
    else {
      out += name +'=';
      out += encodeURIComponent(data[i]);
    }
  }
  return out;
}

/**
 * Add checkboxes to standard CT links display or elements with class 'ct-tag'
 * within a container with class supplied in 'links_container_class' setting.
 */
Drupal.communityTags.addCheckboxes = function (form) {
  var links = $('ul.links li a', form);
  var o = $(form).data('o');

  if (typeof o.links_container_class != "undefined") {
    links = $('.'+o.links_container_class+' .ct-tag');
  }

  $(links).filter(':not(.ct-cbprocessed)').addClass('ct-cbprocessed').each(function() {
    var tag = $(this).text();
    var is_checked = $.inArray(tag, o.tags) > -1;
    $(this).before($(document.createElement("input"))
    .attr({type: 'checkbox', checked: is_checked, value: tag})
    .change(function() {
      var o = $(form).data('o');
      if($(this).attr('checked')) {
        Drupal.communityTags.sendTags(form, o, $(this).val());
      }
      else {
        var i = $.inArray($(this).val(), o.tags);
        o.tags.splice(i, 1);
        Drupal.communityTags.sendTags(form, o, '');
      }
    }));
  });
}

/**
 * Helper function to parse a querystring. Copied from views base.js.
 */
Drupal.communityTags.parseQueryString = function (query) {
  var args = {};
  var pos = query.indexOf('?');
  if (pos != -1) {
    query = query.substring(pos + 1);
  }
  var pairs = query.split('&');
  for(var i in pairs) {
    var pair = pairs[i].split('=');
    // Ignore the 'q' path argument, if present.
    if (pair[0] != 'q' && pair[1]) {
      args[pair[0]] = decodeURIComponent(pair[1].replace(/\+/g, ' '));
    }
  }
  return args;
}

Drupal.communityTags.sendTags = function (form, o, tag_to_add) {
  // Send existing tags and new tag string.
  var token = $('input[name=form_token]', form).val();

  var params = { sequence: ++Drupal.communityTags.ctSequence, tags: o.tags, add: tag_to_add, token: token, extra: o.extra, source: o.source, formId: form.id};
  var o = o;

  // add any query parameters from the form action url - if this form has been ajax
  // loaded by another process this will propogate the settings - well at least it works for views paging both ajax and non-ajax...
  // parse query parameters in order to remove q and possibly other undesirables
  var url = o.url;

  var queryParams = Drupal.communityTags.parseQueryString(url);
  var queryString = Drupal.communityTags.serialize(queryParams);
  if (queryString.length > 0) {
    url += (url.match(/\?/) ? "&" : "?") + queryString;
  }

  $.post(url, Drupal.communityTags.serialize(params), function (data) {
    data = Drupal.parseJson(data);
    if (data.status && Drupal.communityTags.ctSequence == data.sequence) {
      o.tags = data.tags;
      $(form).data('o', o);

      // notify that CT's have been updated
      if(data.update_tags) {
        $(document).trigger('CT_UPDATE_TAGS', [form, data]);
      }
      $(document).trigger('CT_SEND_TAGS_COMPLETE', [form, data]);
    }
  });
}

// Prepare the add Ajax handler and add the button.
Drupal.communityTags.addHandler = function (form, textfield) {
  var o = $(form).data('o');

  // Send existing tags and new tag string.
  Drupal.communityTags.sendTags(form, o, textfield[0].value);

  // Add tag to local list
  o.tags.push(textfield[0].value);

  // store "o" where it's easy to get at
  $(form).data('o', o);

  Drupal.communityTags.updateList(form);

  // Clear field and focus it.
  textfield.val('').focus();
}

// Prepare the delete Ajax handler.
Drupal.communityTags.deleteHandler = function (li, form) {
  var textfield = $('input.form-tags', form);

  // keep stored "o" up to date
  var o = $(form).data('o');

  // Remove tag from local list.
  var i = $(li).attr('key');
  o.tags.splice(i, 1);

  // keep stored "o" up to date
  $(form).data('o', o);

  Drupal.communityTags.updateList(form);

  // Send new tag list.
  Drupal.communityTags.sendTags(form, o, '');

  // Clear textfield and focus it.
  textfield.val('').focus();
}

Drupal.communityTags.updateList = function (form) {
  var o = $(form).data('o');
  if(typeof o.tags != "undefined") {
    // maintain consistent order
    o.tags.sort(function (a,b) { a = a.toLowerCase(); b = b.toLowerCase(); return (a>b) ? 1 : (a<b) ? -1 : 0; });
    var list = $('.tag-widget ul', form);
    list.empty();
    for (i in o.tags) {
      list.append('<li key="'+ Drupal.communityTags.checkPlain(i) +'">'+ Drupal.communityTags.checkPlain(o.tags[i]) +'</li>');
    }
    $("li", list).hover(
      function () {
        $(this).addClass('hover');
      },
      function () {
        $(this).removeClass('hover');
      }
    );
    $('li', list).click(function() {
      Drupal.communityTags.deleteHandler(this, form)
    });
  }
}

Drupal.behaviors.communityTags = function(context) {
  var sequence = 0;

  // Note: all tag fields are autocompleted, and have already been initialized at this point.
  $('input.form-tags', context).filter(':not(.ct-processed)').addClass('ct-processed').each(function () {
      // Hide submit buttons.
      $('input[type=submit]', this.form).hide();

      // Fetch settings.
      var nid = $('input[name=nid]', this.form).val();
      var vid = $('input[name=vid]', this.form).val();

      var form = this.form;
      var tags = $('input[name=tags_refs]', this.form).val().split(', ');
      var url = Drupal.settings.basePath + 'community-tags/js/' + nid + '/' + vid;
      var links_container_class = $('input[name=links_container_class]', this.form).val();
      var source = $('input[name=source]', this.form).val();

      var o = {nid: nid, vid: vid, tags: tags, url: url, links_container_class: links_container_class, source: source};
      $(form).data('o', o);

      // Show the textfield and empty its value.
      var textfield = $(this).val('').css('display', 'inline');

      var button = $('<input type="button" class="form-button" value="'+ Drupal.t('Add') +'" />').click(function () { Drupal.communityTags.addHandler(form, textfield); });
      $(this.form).submit(function () { Drupal.communityTags.addHandler(form, textfield); return false; });

      var messages = $('<div class="ct-messages">Enter tags.</div>');

      // Create widget markup.
      // @todo theme this.
      var widget = $('<div class="tag-widget"><ul class="inline-tags clear-block"></ul></div>');
      textfield.before(widget);
      widget.append(textfield).append(button).append(messages);

      Drupal.communityTags.updateList(form);

      Drupal.communityTags.addCheckboxes(this.form);
  });
}

// update the cloud when community tags are updated
$(document).bind('CT_UPDATE_TAGS', function(e, form, data) {
  // only update list if cloud is returned
  Drupal.communityTags.updateList(form);
});

// update the cloud when community tags are updated
$(document).bind('CT_SEND_TAGS_COMPLETE', function(e, form, data) {
  // only update list if cloud is returned
  if(data.cloud) {
    $('.ct-cloud', form).html(data.cloud);
    // put checkboxes back on
    Drupal.communityTags.addCheckboxes(form);
  }

  if(data.messages) {
    $('.ct-messages', form).html(data.messages).fadeTo(0,0).css('visibility','visible').fadeTo(400,1);
    setTimeout(function(){ $('.ct-messages', form).fadeTo(400,0, function() {$(this).css('visibility', 'hidden'); $(this).html('Enter tags.');}) }, 3000);
  }
});


