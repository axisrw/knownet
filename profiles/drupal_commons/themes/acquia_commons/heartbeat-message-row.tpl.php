<?php
// $Id: heartbeat-message-row.tpl.php,v 1.1.2.10 2010/02/21 22:45:26 stalski Exp $

/**
 * @file
 *   Template file for one row, rendered by heartbeat
 *
 * @var
 * - $message : after it was parsed by heartbeat (grouped)
 * - $time_info : information about the time of activity
 * - $class : extra classes to use on the row
 * - $attachments : attachment on the message id (of the grouped message)
 *
 * @remarks
 *   beat-item-<uaid> is necessairy. The beat item id is used to toggle
 *   visibility of the "number more" messages when grouping exceeded the
 *   maximum allowed grouped property.
 */

?>
<div class="heartbeat-message-block <?php print $message->message_id . ' ' . $zebra; ?>">

  <div class="beat-item" id="beat-item-<?php print $message->uaid ?>">

<div class="beat-item-info">
   <?php if(!empty($message->actor->picture)):?>
   <span class="heartbeat_times"><?php print(theme_imagecache('user_picture_meta',$message->actor->picture, $message->actor->name, $message->actor->name)); ?></span>
   <?php endif; ?>
   <?php if (!empty($message->content['time_info'])): ?>
    <span class="heartbeat_times"><?php print $message->content['time_info']; ?></span>
    <?php endif; ?>
    </div>

   <?php 
   
   if ($message->message_id == "heartbeat_add_own_fbss"){
	print $message->variables['@username']." posted on own wall: <b><em>".$message->variables['@status_message']."</em></b>";  
   }
   elseif ($message->message_id == "heartbeat_add_fbss_to_other"){
		print $message->variables['@userpost']." posted on ".$message->variables['@userowner']."'s wall: <b><em>".$message->variables['@status_message']."</em></b>"; 
   }
   else {
	print $message->content['message'];
   }
   ?>


    <div class="clear"></div>

    <?php if (!empty($message->content['widgets'])) : ?>
	
    <div class="heartbeat-attachments">
    <div class="heartbeat-attachments">
      <?php print $message->content['widgets']; ?>
    </div>
    <?php endif; ?>

    <?php if (!empty($message->content['buttons'])) :?>
    <div class="heartbeat-buttons">
      <?php print $message->content['buttons']; ?>
    </div>
    <?php endif; ?> 

	<div>
      <?php print '<HR style="color:#DAEBFD;background-color:#DAEBFD;height:1px;border:none;">' ?>
    </div>

    <br class="clearfix" />

  </div>

</div>