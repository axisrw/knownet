<?php
// $Id: simplereservation.inc,v 1.1 2009/07/09 12:25:25 jwendebaum Exp $

/**
 * simplereservation.module : simplereservation.inc
 * Base functions
 */
 
function simplereservation_get_topmenu($title="")
{
  $content .= '<center><b><a href="?q=simplereservation">&Uuml;bersicht</a> | <a href="?q=simplereservation/cal">Kalender</a> | <a href="?q=simplereservation/aed/a">neue Reservierung eintragen</a></b></center>';
  
  if (!empty($title))
  $content .= '<h2>'.$title.'</h2>';
 
  return $content;
}
 
function simplereservation_get_planes()
{
  $a = mysql_query('SELECT * from '.simplereservation_TAB_PLANE);
  $planes = array();
  while($b = mysql_fetch_row($a)) {
     
     $planes[] = $b[2];
  }
  
  return $planes;
}
 
 
// liefert eine Tabelle mit allen Reservierungen in einer Woche, angefangen mit dem übergebenen Tag (timestamp)
function simplereservation_get_week($stamp) 
{  
  global $user;
  $ST = "";
$path = drupal_get_path('module', 'simplereservation');

$planes = simplereservation_get_planes();
  
  $tage = array("Sonntag","Montag","Dienstag","Mittwoch","Donnerstag","Freitag","Samstag");
   
  $ST .= "<table width=100% border=0 cellpadding=0 cellspacing=0 style=\"empty-cells:show;border=0px #000 solid\">";
  
  $tstoday = mktime(0, 0, 0, date("m")  , date("d"), date("Y"));

  for ($i = 0; $i < 7; $i++)
  { 
    $thisday_stamp = mktime(0, 0, 0, date("m", $stamp ), date("d", $stamp )+$i, date("Y", $stamp ));
    $thisday = date("Y-m-d", $thisday_stamp);

  $day_in_year = date("z", $thisday_stamp);
  
    $Sunrise =  calcSunset($thisday_stamp, 50.4046, 8.1022, 0, 0);
    $Sunset =  calcSunset($thisday_stamp, 50.4046, 8.1022, 1, 0);

//  $Sunrise = date_sunrise($thisday_stamp, SUNFUNCS_RET_STRING, 50.4046, 8.1022, 90, 0);
//  $Sunset = date_sunset($thisday_stamp, SUNFUNCS_RET_STRING, 50.4046, 8.1022, 90, 0);


    if ($thisday_stamp == $tstoday)
       $ST .= "<tr bgcolor=\"#A0A0A0\">";
    else
       $ST .= "<tr bgcolor=\"#D0D0D0\">";

    $ST .= "<th align=left>" . date("d", $thisday_stamp)." ".$tage[date("w", $thisday_stamp)];
    
    $ST .= " (SR: ".$Sunrise.", SS: ".$Sunset." UTC)";
    
    $ST .= "<th align=right>";
    $ST .= "<a href=\"".simplereservation_LINK_PATH."aed/a/".$thisday."\"><img src=\"".$path ."/pic/new.png\" align=middle alt=\"neu\"></a>&nbsp;";
    $ST .= "<tr><td colspan=2>";

    $CONT = "";
    $j = 0;

    // trickreiche Abfrage, liefert die Pilotennamen mit deren Reservierungen für einen Tag
    $query  = 'SELECT '.simplereservation_TAB_RESERVIERUNG.'.plane, '.simplereservation_TAB_RESERVIERUNG.'.start, '.simplereservation_TAB_RESERVIERUNG.'.end, ';
    $query .= simplereservation_TAB_RESERVIERUNG.'.comment, '.simplereservation_TAB_RESERVIERUNG.'.id, ';
    $query .= simplereservation_TAB_RESERVIERUNG.'.pilot, '.simplereservation_TAB_RESERVIERUNG.'.bookfor, ';
    $query .= simplereservation_TAB_USERS.'.name, '.simplereservation_TAB_USERS.'.mail ';
    $query .= ' FROM '.simplereservation_TAB_RESERVIERUNG.' INNER JOIN '.simplereservation_TAB_USERS;
    $query .= ' WHERE '.simplereservation_TAB_RESERVIERUNG.'.pilot = '.simplereservation_TAB_USERS.'.uid ';
    $query .= ' AND ((LEFT('.simplereservation_TAB_RESERVIERUNG.'.start, 10)="'.$thisday.'" OR LEFT('.simplereservation_TAB_RESERVIERUNG.'.end, 10)="'.$thisday.'")';
    $query .= ' OR (LEFT('.simplereservation_TAB_RESERVIERUNG.'.start, 10)<"'.$thisday.'" AND LEFT('.simplereservation_TAB_RESERVIERUNG.'.end, 10)>"'.$thisday.'"))';
    $query .= ' ORDER BY '.simplereservation_TAB_RESERVIERUNG.'.start ASC';
    $result= mysql_query($query) or die ("#wk1: Anfrage fehlgeschlagen: " . mysql_error());

    while($flight = mysql_fetch_assoc($result)) 
    {
/*     $k=0;
      while ($k < count ($planes) && $planes[$k][0] != $flight["plane"])
      {
        $k++;
      }
/**/
      $sday = substr($flight["start"],0,10);
      $stime = substr($flight["start"],11,5);
      $eday = substr($flight["end"],0,10);
      $etime = substr($flight["end"],11,5);
      
      if ((($sday != $thisday) && ( $eday != $thisday)) || (($sday == $eday) && ($stime == "00:00") && ($etime == "23:59") ))
        $CONT .= "ganzt&auml;gig";
      else
      {          
        // heute oder gestern gestartet?
        if ($sday != $thisday)
          $CONT .= "...";
        else  
          $CONT .= $stime;
        
        $CONT .= "-";
        
        // heute oder morgen gelandet?
        if ($eday != $thisday)
          $CONT .= "...";
        else  
          $CONT .= $etime ;
      }

      $CONT .= " ". $planes[$flight["plane"]];

  
    if (empty($flight["bookfor"])) 
      $name =$flight["name"];
    else
      $name =$flight["bookfor"];
    
    if (empty($flight["bookfor"])) // selbst erstellt
    {     
      $CONT .= " <a href=\"?q=user/".$flight["pilot"]."\">".$name."</a>";
      //$CONT .= " <a href=\"mailto:". $flight["mail"]."\">".$name."</a>";
    }
    else // erstellt von turmfralken
    { 
      $CONT .= "&nbsp;".$flight["bookfor"]."<img src=\"".$path ."/pic/star.png\" alt=\"gebucht von ".$flight["name"]."\" title=\"gebucht von ".$flight["name"]."\">";
    }
    
  
    if ($flight["comment"] != "") 
      $CONT .= " (".$flight["comment"].")";

    if (($user->uid == $flight["pilot"]) || is_simplereservation_mod())
    {
      $CONT .= "&nbsp;<a href=\"".simplereservation_LINK_PATH."aed/e/".$flight["id"]."\">";
      $CONT .= "<img src=\"".$path ."/pic/edit.png\" align=middle alt=\"ändern\" title=\"ändern\"></a>";
    }
    else 
      $restable .= "<td>&nbsp;";
  
    $CONT .= "<br>";

    } // while

    $ST .= $CONT;
    $ST .= "&nbsp;";

  } //  for ($i = 0; $i < 7; $i++)
  
  $ST .= "</table>";

  return $ST;
}

function simplereservation_make_calendar($starttag) {
$path = drupal_get_path('module', 'simplereservation');

  $monate = array(1=>"Januar", 2=>"Februar", 3=>"M&auml;rz", 4=>"April", 5=>"Mai", 6=>"Juni", 7=>"Juli", 8=>"August", 9=>"September", 10=>"Oktober",  11=>"November", 12=>"Dezember");
  
  $content = "$starttag";
  
  if (isset($starttag))
  {
    $pieces = explode ("-", $starttag);
    $starttag_stamp = mktime(0, 0, 0, $pieces[1]  ,  $pieces[2], $pieces[0]);
  }
  else //if  ($starttag == 0)
  {
    $tsToday = mktime(0, 0, 0, date("m")  , date("d"), date("Y"));
    $starttag_stamp = $tsToday;
     
  }
  
  # count down to monday
  while (strftime('%u', $starttag_stamp) != 1)
         $starttag_stamp -= 60*60*24;

  // Navigationspunkte berechnen:
  $tag = date("d", $starttag_stamp);
  $monat = date("m", $starttag_stamp);
  $jahr = date("Y", $starttag_stamp);
  
  $starttag = date("Y-m-d", $starttag_stamp);
  $lastweek = date("Y-m-d", (mktime(0, 0, 0, $monat  , $tag-7, $jahr)));
  $nextweek = date("Y-m-d", (mktime(0, 0, 0, $monat  , $tag+7, $jahr)));

  $lastmonth = date("Y-m-d", (mktime(0, 0, 0, $monat  , $tag-28, $jahr)));
  $nextmonth = date("Y-m-d", (mktime(0, 0, 0, $monat  , $tag+28, $jahr)));

  $endmonat = date("n", mktime(0, 0, 0, $monat  , $tag+6, $jahr));

  $monat = date("n", $starttag_stamp); 

  $content .= '<table width=100% border=0>';
  $content .= '<tr><td align=left><a href="'.simplereservation_LINK_PATH.'cal/'.$lastweek.'"><img src="'.$path .'/pic/prev.png" align=middle alt="vorherige Woche"> vorherige Woche</a>'; 
  $content .= '<td align=center>';

  $content .= '<a href="'.simplereservation_LINK_PATH.'cal/'.$lastmonth.'"><img src="'.$path .'/pic/prev.png" align=middle alt="vorheriger Monat"></a> '; 

  $content .= '<b>'.$monate[$monat];
  
  if ($endmonat != $monat)
    $content .= "/".$monate[$endmonat];
  
  $content .= ' '.$jahr . ' </b>';
  $content .= ' <a href="'.simplereservation_LINK_PATH.'cal/'.$nextmonth.'"><img src="'.$path .'/pic/next.png" align=middle alt="nächster Monat"></a>'; 
  
  $content .= '<td align=right>';
  $content .= '<a href="'.simplereservation_LINK_PATH.'cal/'.$nextweek.'">n&auml;chste Woche <img src="'.$path .'/pic/next.png" align=middle alt="nächste Woche"></a>';
  $content .= '</table>';

  // und jetzt noch den Hauptteil: die Wochenübersicht (aus functions.php)
  $content .= simplereservation_get_week($starttag_stamp);

  $content .= "<br><br><img src=\"".$path ."/pic/star.png\"\"> Reservierung erstellt von Turmfalken oder "
       .  "Administrator f&uuml;r den jeweiligen Piloten.<br>Mauszeiger &uuml;ber den Stern halten, um anzuzeigen, wer diese Reservierung eingetragen hat.";

  
  
  return $content;
} 
 
 ?>