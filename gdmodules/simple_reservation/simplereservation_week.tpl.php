<?php
$imgpath = base_path() . drupal_get_path('module', 'simplereservation').'/images/';
$week_day = date_week_days_ordered(date_week_days(TRUE));

  if (user_access('access simple reservations')) { ?>

  <p><!-- Kopfzeilen -->
  <table width="100%" border="0"><tr>
  <td>  <?php print( l( '<img src="'. $imgpath.'prev.png" style="float:left;margin-right:5px;">'.t("previous week"), $path .'simplereservation/'. ($config["prev_week"]) ."/".$config["prev_week_year"], array('html' => TRUE))) ?></td>

  <td><?php print( l( '<img src="'. $imgpath.'prev.png"  style="float:left;margin-right:5px;">'.t("previous month"), $path .'simplereservation/'. ($config["prev_4week"]) ."/".$config["prev_4week_year"], array('html' => TRUE))) ?>

  <td  style="text-align:center;"><b>&nbsp;&nbsp;<?php  print( $config["month"] ."&nbsp;". $config["year"].", ". t("week"). "&nbsp;");  print $config["week"]; ?>&nbsp;&nbsp;</b></td>

  <td style="text-align:right;">
  <?php print( l( '<img src="'. $imgpath.'next.png" style="float:right;margin-left:5px;">'.
  t("next month"), $path .'simplereservation/'. ($config["next_4week"]) ."/".$config["next_4week_year"], array('html' => TRUE))) ?></td>

  <td style="text-align:right;">
  <?php print( l('<img src="'. $imgpath.'next.png" style="float:right;margin-left:5px;">'.
  t("next week"), $path .'simplereservation/'. ($config["next_week"]) ."/".$config["next_week_year"], array('html' => TRUE))) ?></td>
  </table>
  <br>

  <?php  $oddeven = 0; ?>
  <table>
  <tbody>
    <?php for ($i = 0; $i < 7; $i++) { ?>
    <tr class="odd"><td  valign="top" width="25%" bgcolor="#FFFFCC"><strong>
    <?php
    print $week_day[$i].',&nbsp;';
    print date_format_date($config["calendar"][$i], 'custom', 'd.m.Y');
    ?></strong></td>

    <td valign="top" align="right"><!-- Plus-Image -->
    <?php
    if (user_access('add simple reservations') || user_access('add simple reservations for others') ) {
      print( l('<img src="'. $imgpath.'new.png" alt='.t("Add reservation").' title="'.t("Add reservation").'"></a>', $path .'simplereservation/add/'. date_format_date($config["calendar"][$i], 'custom', 'Y')."/". date_format_date($config["calendar"][$i], 'custom', 'm')."/". date_format_date($config["calendar"][$i], 'custom', 'd')."/", array('html' => TRUE))) ;
    }
    else
    print ('&nbsp;');
    ?>
    </td></tr>

    <tr class="even"><!-- Reservierungen fuer diesen Tag -->
    <td valign="top" colspan="2">
    <?php
    foreach ($reservations[$i] as $reservation)  {
      if ($reservation["rid"] > 0)  {
          $from=user_load($reservation["uid"]);
          $for=user_load($reservation["for_uid"]);
          // von Beginn
          if ( $reservation["begin"] <= date_format($config["calendar"][$i],"U") ) print( "..." );
          else print( date("H:i", $reservation["begin"]));
          print (" - ");
          // bis Ende
          if ( $reservation["ending"] >= date_format($config["calendar"][$i+1],"U") )  print( "..." );
          else print( date("H:i", $reservation["ending"]));

          print( "&nbsp;&nbsp;<b><a  title=\"".$reservation["description"]."\">". $reservation["name"]."</a></b>");

          // nur anzeigen wenn angemeldet und eigene Reservierung
          if($user->uid && (($user->uid == $reservation["uid"]) || ($user->uid == $reservation["for_uid"]))) {
            print (" ". t("booked by")." ".l($from->name, $path .'user/'. $from->uid) );
            if (($reservation["for_uid"] > 0) && ($reservation["uid"] != $reservation["for_uid"])) {
              print(" ". t("for")." ".l($for->name, $path .'user/'. $for->uid) );
            }

            if ($reservation["rcomment"] != "")
            print (" (".$reservation["rcomment"].")");

            if ((user_access('edit own simple reservations')) || (user_access('edit simple reservations of others'))) {
              print(" ");
              print( l('<img src="'. $imgpath.'edit.png" title="Bearbeiten">', $path .'simplereservation/edit/'.$reservation["rid"], array('html' => TRUE)));
            }
          }
          print "<br>";
      }
    }
    ?>
    </td>

  <?php } ?>

  </tbody>
  </table>
<?php } ?>
</p>
