// $Id: im.js,v 1.1.2.18 2010/03/29 06:43:57 pahariwalla Exp $

var last_msg_in = 0;
var timer_friendlist = 0;
var timer_newmsgs = 0;
var url_prefix = '';
var im_flash_player_listener = new Object();
var is_muted;
var friendlist_array = new Array;
var oldFriendlist;
var newFriendlist;

if (Drupal.jsEnabled) {
  var poking = 0;
  $(document).ready(function() {
    if (Drupal.settings.im_module.has_audio ==1) {    
      im_flash_player_listener.onInit = function() {
        this.position = 0;
      };
      if ($('#im-console-speaker-icon-on').css('display') == 'none') {
        is_muted = 1;
      }
      else {
        is_muted = 0;
      }
      $('#im-console-speaker-icon-on').click(function() {
        set_speaker_state(0);
        is_muted = 1;
        $('#im-console-speaker-icon-on').toggle();
        $('#im-console-speaker-icon-off').toggle();
        });
      $('#im-console-speaker-icon-off').click(function() {
        set_speaker_state(1);
        is_muted = 0;
        $('#im-console-speaker-icon-on').toggle();
        $('#im-console-speaker-icon-off').toggle();
        });
    }
    
    if (Drupal.settings.basePath == '/') {
      url_prefix = '/';
    }
    else {
      url_prefix = Drupal.settings.basePath; // .substring(1);
    }
    var d = new Date();
    last_msg_in = (d.getTime());
    $('#im-refresh-friendlist').click(function() {
      getFriendList();
      return false;
    });

   $('#edit-im-console-commandline').keydown(function(e) {
      if (e.which != 13) {
        return true;
      }
      if (!$('#edit-im-other-uid').val() || $('#edit-im-other-uid').val() ==0) {
        alert (Drupal.t("Please select some someone to talk to"));
        return false;
      }
      if (!$('#edit-im-my-uid').val() || !$('#edit-im-console-commandline').val()) {
        return false;
      }
      if (Drupal.settings.im_module.has_perms ==0) {
        return false;
      }
      var sendText = $('#edit-im-console-commandline').val();
      var newText = $('#im-console-msgs').html()
//       + '<span class = "im-console-msg-name">Me: </span>'
          
      var newText = $('#im-console-msgs').html()
       + '<span class = "im-console-msg-name">' + Drupal.t('Me: ') + '</span>'
       + '<span class="im-console-my-msgtext">'
       + $('#edit-im-console-commandline').val()
       + '</span>'
       + '<br/>';           
          
      $('#im-console-msgs').html(newText);
      $('#im-console-msgs').animate({scrollTop: 99999 }, 1);        
      $('#edit-im-console-commandline').val('')
      
      var afterSend = function(data) {
        $('div.status').html(data.status);
      }
      var sendurl= url_prefix + 'im/send_msg/'
        + $('#edit-im-my-uid').val() + '/'
        + $('#edit-im-other-uid').val() + '/?msg='
        + encodeURI(sendText);
      $.ajax({
        type: 'GET',
        url: sendurl,
        dataType: 'json',
        success: afterSend,
        data: 'js=1'
      });
      return false;
    });
    getFriendList();
    getNewMsgs ('all');
    timer_newmsgs = setTimeout ('getNewMsgs("all")', Drupal.settings.im_module.refresh_rate);    
  });
}

function set_speaker_state($speaker_state) {
  if (Drupal.settings.im_module.has_perms ==0) {
    return;
  }
  var sendurl= url_prefix + 'im/set_speaker_state/' + $speaker_state;
  $.ajax({
    type: 'GET',
    url: sendurl ,
    dataType: 'json',
    data: 'js=1'
  });
  
}

function popstatus(thetext) {
  var newText = $('#im-console-msgs').html() + '<BR>'+ thetext;
  $('#im-console-msgs').html(newText);
  $('#im-console-msgs').animate({scrollTop: 99999 }, 1);        
  $('#edit-im-console-commandline').val(''); 
}

function getFriendList() {
  if (Drupal.settings.im_module.has_perms ==0) {
    return;
  }
  if ($('#edit-im-my-uid').val() > 0) {
    var friendList = function(data) {
      oldFriendlist = jQuery.extend(true, {}, newFriendlist);
      newFriendlist = jQuery.extend(true, {}, data.friendlist_array);
      raiseEventsFriendlist (data.friendlist_array);
      $('#im-friendlist').html(data.friendlist);
    }
    var sendurl= url_prefix + 'im/friendlist';
    $.ajax({
      type: 'POST',
      url: sendurl ,
      dataType: 'json',
      success: friendList,
      data: 'js=1'
    });
  }
  clearTimeout (timer_friendlist);
  timer_friendlist = setTimeout ('getFriendList()', Drupal.settings.im_module.friendlist_refresh_rate);
}

function click_on_friend(uid, uname) {
  $('#edit-im-other-uid').val(uid);
  $('#edit-im-other-name').val(uname);
  $('#im-console-caption-name').text(uname);
  getNewMsgs ('all');
  return false;
  
}

function getNewMsgs(getall) {
  if (Drupal.settings.im_module.has_perms ==0) {
    return;
  }
  if (!$('#edit-im-other-uid').val()) {
    return;
  }
  if (getall=='all') {
    get_url = url_prefix + 'im/get_all_messages/' + $('#edit-im-other-uid').val();
     $('#im-console-msgs').html('');
  }
  else {
    get_url = url_prefix +'im/get_new_messages/' + $('#edit-im-other-uid').val();
  }
  $('#edit-im-status').val(status);
  
  var pokeNewMsgs = function(data) {
    //return if called from timeout while already in routine
    if (poking == 1) {
      return;
    }
    var d = new Date();
    
    poking = 1;
    var waiters = data['waiters'];
    var this_msg_in = 0;
    if (data.status == 'got data')  {
      var str = "";
      var msgs = data['msgs'];
      var newText = $('#im-console-msgs').html() + msgs;
      $('#im-console-msgs').html();
      $('#im-console-msgs').html(newText);
      $('#im-console-msgs').animate({scrollTop: 99999 }, 1);
      this_msg_in = (d.getTime());
      if (Drupal.settings.im_module.has_audio == 1 && getall !='all') {
        play_sound(Drupal.settings.im_module.current_convo_msg_in);
      }
      getFriendList();
    }
    poking = 0;
    if (this_msg_in > 0) {
      last_msg_in = this_msg_in;
      clearTimeout(timer_newmsgs);
      timer_newmsgs = setTimeout ('getNewMsgs("new")', Drupal.settings.im_module.refresh_rate);    
    }
    else {
      now = d.getTime();
      if ((now - last_msg_in) < Drupal.settings.im_module.im_refresh_rate_inactive_threshold) {
        clearTimeout(timer_newmsgs);
        timer_newmsgs = setTimeout ('getNewMsgs("new")', Drupal.settings.im_module.refresh_rate);    
      }
      else {
        clearTimeout(timer_newmsgs);
        timer_newmsgs = setTimeout ('getNewMsgs("new")',  Drupal.settings.im_module.im_refresh_rate_inactive);
      }
    }
  }
  $.ajax({
    type: 'POST',
    url: get_url,
    dataType: 'json',
    success: pokeNewMsgs,
    data: 'js=1'
  });
}

//Audio Alert
function getImAudioPlayer()  {
   return document.getElementById("im_flash_player");
}

function raiseEventsFriendlist(newFriendlist) {
  // find new uids

  var oldUids = [];
  var newUids = [];
  var i = 0;

  for (var friend in oldFriendlist) {
    oldUids[i]= oldFriendlist[friend]['uid'];
    i++;
  }
  if (i == 0) {
    return;
  }

  i = 0;
  for (var friend in newFriendlist) {
    newUids[i]= newFriendlist[friend]['uid'];
    i++;
  }

  for (var i=0; i < oldUids.length; i++) {
    if (newUids.indexOf(oldUids[i]) == -1) {
      play_sound (Drupal.settings.im_module.friendpicker_user_left);
    }
  }
  for (var i=0; i < newUids.length; i++) {
    if (oldUids.indexOf(newUids[i]) == -1) {
      play_sound(Drupal.settings.im_module.friendpicker_user_added);
    }
  }

  for (var friend in newFriendlist) {
    if (newFriendlist[friend]['waiters'] > waiters(newFriendlist[friend]['uid'], oldFriendlist)) {
        play_sound (Drupal.settings.im_module.other_msg_in);
        if (!$('#edit-im-other-uid').val() || $('#edit-im-other-uid').val() ==0) { // talking to no one
          click_on_friend(newFriendlist[friend]['uid'], newFriendlist[friend]['name']);
        }
    }
  }
}

function waiters(uid, friendlist) {
  for (var friend in friendlist) {
    if (friendlist[friend]['uid'] == uid) {
      return friendlist[friend]['waiters'];
    }
  }
  return 0;
}


function play_sound(file_name) {
  if (is_muted == 1 || file_name =='/none') {
    return;
  }
  if (im_flash_player_listener.position == 0) {
	try 
	{
	 getImAudioPlayer().SetVariable("method:setUrl", file_name);
	}
	catch(err) {
	}
  }
  try {
    getImAudioPlayer().SetVariable("method:play", "");
  }
	catch(err) {
	}
  try {
    getImAudioPlayer().SetVariable("enabled", "true");
  }
  catch(err) {
  }
}

