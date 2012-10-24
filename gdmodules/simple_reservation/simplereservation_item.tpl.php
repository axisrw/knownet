<?php 

$imgpath = drupal_get_path('module', 'simplereservation').'/images/'; 
$week_day = date_week_days_ordered(date_week_days(TRUE));


?>

<p>
    <strong><?php print $config["name"]; ?>:</strong>
    
Woche <?php print $config["week"]; ?>

<table width=100% border=0>
  <tr><td align=left><a href="cal/'.'"><img src="<?php print $imgpath; ?>prev.png" align=middle alt="vorherige Woche"> vorherige Woche</a>
  <td align=center>

  <a href="'.'cal/'.'"><img src="<?php print $imgpath; ?>prev.png" align=middle alt="vorheriger Monat"></a> '

  <b>gg</b>
   <a href="'.'cal/'.'"><img src="<?php print $imgpath; ?>next.png" align=middle alt="nächster Monat"></a>'
  
  <td align=right>';
  <a href="'.'cal/'.'">n&auml;chste Woche <img src="<?php print $imgpath; ?>next.png" align=middle alt="nächste Woche"></a>
  </table>

<?php $oddeven = 0; ?>
  <table>
  <thead><tr><th>Date</th><th>Description</th> </tr></thead>
<tbody>
  <?php for ($i = 0; $i < 7; $i++) { ?>
  <tr class="
    <?php $oddeven = 1 - $oddeven; if ($oddeven == 0) print("even"); else print ("odd"); ?>
  ">
       <td>
       
          
          <?php print $week_day[$i] ?>, <?php print date_format_date($config["calendar"][$i], 'custom', 'd.m.Y'); ?>
          
      </td>
        <td><?php print ("_".$config["calendar"][$i]) ;?>yz
        </td>
    </tr>
  <?php } ?>

</tbody>
</table>




<?php $oddeven = 0; ?>
  <table>
  <thead><tr><th>Name</th><th>Description</th> </tr></thead>
<tbody>
  <?php foreach ($reservations as $reservation): ?>
  <tr class="
    <?php $oddeven = 1 - $oddeven; if ($oddeven == 0) print("even"); else print ("odd"); ?>
  ">
       <td>
       
          
          <?php print $reservation ?></a>
          
      </td>
        <td>xyz
        </td>
    </tr>
  <?php endforeach; ?>

</tbody>
</table><br><img src="<?php print $imgpath; ?>star.png"> Reservierung erstellt von Turmfalken oder Administrator f&uuml;r den jeweiligen Piloten.<br>Mauszeiger &uuml;ber den Stern halten, um anzuzeigen, wer diese Reservierung eingetragen hat.
  
  
  </p>
