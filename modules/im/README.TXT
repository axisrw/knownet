$Id: README.TXT,v 1.1.2.4 2009/07/14 07:28:06 pahariwalla Exp $

Purpose: Enable registered users to instant message each other.

Dependencies: Relies on either User Relationships module or Views to provide a list of online users which current user can IM.

Installation: Enable the module

Database: Enabling creates a database table "im_msgs"

Configuration: Before using IM, administrator must create a User Relationship Type to allow users to establish
lists of "friends" to IM.  Once this User Relationship Type is created, to use this Relationship Type as the basis for building the list of IM friends by selecting that Relationship Type
under the label "User relationship for selecting IM Friends" in admin/settings/im.

Optional configuration settings involving polling frequency and labels can also set here:

Theming: All theming is done via css in file im.css.  Classes and ID's are provided for:
div.im-friendlist             - container for the list fo friends
div.im-friendlist .im-friends - links representing the friends
#im-command-line              - textfield where you type your messages
#im-display-console-msgs      - container for the console displaying the conversation
.im-console-msg-name          - portion of each message containing the name of the sender of the message
.im-console-my-msgtext        - portion of current user's message containing the actualy message text
.im-console-other-msgtext     - portion of other user's message containing the actualy message text
#im-console-wrapper           - div containing the message console
#im-console-caption           - label above the div containing the message console
#im-console-msgs              - the textarea containing the message console


User interface: Provides 2 blocks:
1) Friendlist - lists users with whom a user can IM.  
2) The "command line" for typing in messages and the "console" that displayes the conversation. 

Usage: The Friendlist will provide a link to each user in your list of friends who are online. Determination of
whether a user is online is based on the same algoritm as the Drupal "Who's Online' block, with the added capability
to set the "last accessed" time paramenter in admin/settings/config/.

In order to initiate a conversation, you click on the link for one of your friends.  Then send her a message by typing in the command line
and hitting Enter.   Although you can have multiple simultaneous conversations, you can only view/converse with one at a time.

To switch between conversations, simply click on the user's link in the FriendList block.

When someone sends you a message, you will see a number in parentheses next to the user's link in the Friendlist indicating
how many unread messages you have from that user.

The messages are stored in the database table until both users have logged off, at which point they are deleted. 

Audio Support
------------------------
You need to install the MP3 Flash Player:
1) Download the file (2k) from http://flash-mp3-player.net/medias/player_mp3_js.swf
2) Put it in the im module folder

Configure
-------------------------
1) go to admin/settings/im to pick the sound file for each IM event
2) you can add sound files (mp3's) by putting them in the im/sounds folder

User
-------------------------
4) user can go to their account/edit to set their own sound file preferences
5) user has a small icon on the Console to turn sounds on/off

