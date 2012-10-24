<?php
// $Id: cdn2_field.inc,v 1.3.2.20 2009/11/30 23:41:27 ocyrus Exp $

//
// @file cdn2_field.inc configuration settings for the cck content type
//

/**
 * Declare information about the cdn2 field type
 */
function cdn2_field_info() {
  return array(
    'cdn2' => array(
      'label' => t('CDN2 Video'),
      'description' => t('Allows passthrough video upload to your node type.'),
      'callbacks' => array(
	'tables' => CONTENT_CALLBACK_DEFAULT,
	'arguments' => CONTENT_CALLBACK_DEFAULT,
      ),
    ),
  );
}


function cdn2_widget_info() {
  return array(
    'cdn2' => array(
      'label' => 'CDN2 Field',
      'field types' => array('cdn2'),
      'multiple values' => CONTENT_HANDLE_CORE,
      'callbacks' => array(
        'view' => CONTENT_CALLBACK_CUSTOM
      ),
    ),
  );
}

/**
 * Handle the parameters for the cck field. (used in the add/modify field form)
 */
function cdn2_field_settings($op, $field) {
  switch ($op) {
    // case 'form': (not needed.. no additional options for this field type)
    // case 'validate':
     //case 'save':
    case 'database columns':
      $columns = array(
	'video_token' => array(
	  'mysql_type' => 'varchar(36)',
          'type' => 'text',
          'size' => 36,
          'not null' => TRUE,
        )
      );
      return $columns;
    case 'callbacks':
      // use our own view callback when rendering the field.
      /*       return array(
	'view' => CONTENT_CALLBACK_CUSTOM,
        ); */
    // case 'tables':
    // case 'arguments':
    // case 'filters':
  }
}

function cdn2_content_is_empty($item, $field) {
  return (empty($item['video_token']));
}

/**
 * Define the behavior of the cdn2 field type
 */
function cdn2_field($op, &$node, $field, &$node_field, $teaser, $page) {
  switch ($op) {
    case 'validate':
      break;
    case 'sanitize':
      break;
    case 'load':
      $video_token = $node_field[0]['video_token'];

      $query = 'SELECT cv.video_token, cv.preset_name, cv.status, cv.asset_fetch_url, cv.file_size, cv.video_length FROM {cdn2_videos} cv WHERE cv.nid=\'%s\'';
      $resultset = db_query($query, $node->nid);
      $field_name = $field['field_name'];
      $additions = array();
      $additions[$field_name] = array();
      $additions[$field_name][0] = array();
      $additions[$field_name][0]['assets'] = array();
      $additions[$field_name][0]['video_token'] = $video_token;
      while ($row = db_fetch_array($resultset)) {
        $additions[$field_name][0]['assets'][] = $row;
	// TODO: see if this is handled by the node load directly, otherwise
	// fetch it from the content table.
        $additions[$field_name][0]['video_token'] = $video_token;
      }
      $additions[$field_name][0]['splash_image_number'] = _cdn2_get_splash_image_number($node->nid);
      $query = "SELECT nid from {cdn2_video_node} cvn WHERE video_token='%s'";
      $cvn_nid = db_result(db_query($query, $additions[$field_name][0]['video_token']));
      
      if ($cvn_nid != 0 && !count($additions[$field_name][0]['assets'])) {
        $additions[$field_name][0]['processing'] = TRUE;
      } 
      return $additions;
    case 'view':

    case 'insert':
      // reassociate the video back with the node
      //
      foreach ($node->{$field['field_name']} as $cdn2_video) {
	$video_token = $cdn2_video['video_token'];
	// update the db records to reassociate the node ids with any videos that have been processed.
	// this will address a potential race condition vis a vis xmlrpc callback is invoked before the node
	// has been saved.
	//
      
	$query = "UPDATE {cdn2_video_node} SET nid=%d WHERE video_token='%s'";
	db_query($query, $node->nid, $video_token);
	
	$query = "UPDATE {cdn2_videos} SET nid=%d where video_token='%s'";
	db_query($query, $node->nid, $video_token);
      }
      
      drupal_set_message('Your video is being processed.  It will appear here once it has finished processing.  You may ' . l('refresh this page', 'node/'. $node->nid) .' to see progress.', 'info');
      break;
    case 'update':
      //print_r($node);
  }
}


/**
 * Implementation of hook_field_formatter_info()
 */
function cdn2_field_formatter_info() {
  return array(
    'default' => array(
      'label' => t('Default'),
      'field types' => array('cdn2'),
      'multiple values' => CONTENT_HANDLE_CORE
    ),
    'thumbnail' => array(
      'label' => t('Thumbnail'),
      'field types' => array('cdn2'),
      'multiple values' => CONTENT_HANDLE_CORE
    ),
    'thumbnail_linked' => array(
      'label' => t('Thumbnail linked to node'),
      'field types' => array('cdn2'),
      'multiple values' => CONTENT_HANDLE_CORE
    ),
  );
}

function theme_cdn2_formatter_thumbnail($element) {
  $node = $element['#node'];
  $node = node_load($node->nid);
  $splash_image_number = _cdn2_get_splash_image_number($node->nid);
  switch($splash_image_number) {
    case '0':
        $suffix = "_thumb_001.jpg";
    break;
    case '10':
        $suffix = "_thumb_010.jpg";
    break;
    default:
      $suffix = '_thumb_00'. $splash_image_number .'.jpg';
  }
  $cdn2_default_preset = variable_get('cdn2_default_preset', null);
  $size = theme('cdn2_preset_sizes', $cdn2_default_preset);
  $field_info = _cdn2_get_field_by_content_type($node->type);
  $name = $field_info['field_name'];
  $assets = $node->{$name}[0]['assets'];
  $video_url = $assets[0]['asset_fetch_url']; 
  return  '<img width='.$size['width'].'" height="'. $size['height'] .'" src="'. preg_replace("%\.[^\.]*$%", $suffix, $video_url) .'">';
}
function theme_cdn2_formatter_thumbnail_linked($element) {
  $node = $element['#node'];
  $node = node_load($node->nid);
  $splash_image_number = _cdn2_get_splash_image_number($node->nid);
  switch($splash_image_number) {
    case '0':
        $suffix = "_thumb_001.jpg";
    break;
    case '10':
        $suffix = "_thumb_010.jpg";
    break;
    default:
      $suffix = '_thumb_00'. $splash_image_number .'.jpg';
  }
  $cdn2_default_preset = variable_get('cdn2_default_preset', null);
  $size = theme('cdn2_preset_sizes', $cdn2_default_preset);
  $field_info = _cdn2_get_field_by_content_type($node->type);
  $name = $field_info['field_name'];
  $assets = $node->{$name}[0]['assets'];
  $video_url = $assets[0]['asset_fetch_url']; 
  $path = drupal_get_path_alias('node/'. $node->nid);
  return  l('<img width='.$size['width'].'" height="'. $size['height'] .'" src="'. preg_replace("%\.[^\.]*$%", $suffix, $video_url) .'">', $path, array('html' => TRUE, 'alias' => TRUE));
}
function theme_cdn2_formatter_default($element) {
      $node = $element['#node'];
      $field = $element['#item'];
      $node = node_load($node->nid);
      $db_info = _cdn2_get_field_by_content_type($node->type);
      $field_name = $db_info['field_name'];
      $items = $node->{$field_name};
      if ($field['processing']) {
        $output = t('Your video is either still processing or you have not uploaded any videos.  Once it is complete it will be displayed here.');
      }
      else {
        $output = theme('cdn2_video', $node, $field, $items, $teaser, $page);
      }
      $cdn2_default_preset = variable_get('cdn2_default_preset', $field['assets'][0]['preset_name']);
      $js = 'var cdn2_default_preset = \''. $cdn2_default_preset .'\';'."\n";
      $js .='var aPresetList = new Array();'."\n";
      for ($i = 0; $i < count($items[0]['assets']); $i++) {
        $js .= sprintf('aPresetList[\'%s\'] = \'%s\';'."\n", $field['assets'][$i]['preset_name'], $i);
      }
      drupal_add_js($js, 'inline');
      drupal_add_css(drupal_get_path('module', 'cdn2') .'/cdn2.css');
      drupal_add_js(drupal_get_path('module', 'cdn2') .'/js/ui.core.js');
      drupal_add_js(drupal_get_path('module', 'cdn2') .'/js/ui.tabs.js');
      drupal_add_js(drupal_get_path('module', 'cdn2') .'/js/cdn2.js', 'module', 'footer');
  return $output;  

}


function cdn2_elements() {
  return array(
    'cdn2' => array(
      '#input' => FALSE,
      '#columns' => array('video_token'),
      '#delta' => 0,
      '#process' => array('video_token_process'),
      '#submit' => array('cdn2_submit_new'),
    ),
  );
}

function _cdn2_has_video_token($nid) {
    $SQL = "SELECT * from cdn2_video_node WHERE nid=%d";
    $resource = db_query($SQL, $nid);
    $result = db_fetch_object($resource);
    if($result->nid) {
        return true;
    }
    else {
        return false;
    }
}

/**
 * implementation of hook_widget (shown when the edit/submit forms are called)
 */
function cdn2_widget(&$form, &$form_state, $field, $items, $delta = 0) {
  $formats = array();
  $node = $form['#node'];
  
  $enabled_presets = variable_get('cdn2_allowed_presets', array());
  foreach ($enabled_presets as $preset) {
    if ($preset && user_access('transcode cdn2 video to '. $preset)) {
      $formats[] = $preset;
    }
  }
  // refetch the video token for this node if it exists
  if ($node->nid) {
    $video_token = db_result(db_query("SELECT video_token from {cdn2_video_node} WHERE nid=%d", $node->nid));
    if(!$video_token) {
        $cdn2 = _cdn2_get_soap_client();
        $video_token = $cdn2->getVideoToken($formats);
        $form_state[0]['video_token'] = $video_token;
    }
  }
  else if (!count($_POST) && !isset($_POST[$field['field_name']])) {
    $cdn2 = _cdn2_get_soap_client();
    $video_token = $cdn2->getVideoToken($formats);

    $form_state[0]['video_token'] = $video_token;
  } else {
    $video_token = $_POST[$field['field_name']][0]['video_token']; 
  }
  if (!$video_token && strpos('admin', $_REQUEST['q']) !== 0) {
    drupal_set_message(t('Unable to contact CDN2 service. Video Uploads are currently disabled.'), 'error');
    return NULL;
  } 
      
  drupal_add_css(drupal_get_path('module', 'cdn2') .'/cdn2.css');
  $element['video_token'] = array(
    '#type' => 'hidden',
    '#default_value' => $video_token,
    '#required' => TRUE,
    '#weight' => $field['widget']['weight'],
  );
  $query = "SELECT is_submitted FROM {cdn2_video_node} WHERE video_token='%s'";
  $is_submitted = db_result(db_query($query, $video_token));
  $form['cdn2_is_submitted'] = array(
    '#type' => 'hidden',
    '#value' => $is_submitted ? 'true' : 'false'
  );
  
  // TODO: Here we need to show the uploader form if there's no video associated with this node, or show the list of assets associated with it and their status otherwise
  //
  $show_form = TRUE;
  
  if ($node->nid && _cdn2_has_video_token($node->nid)) {
    $show_form = FALSE;
    $assets = $items[0]['assets'];
    $asset_output = '';
    $form[$field['field_name'] .'_fieldset'] = array(
      '#type' => 'fieldset',
      '#title' => t('CDN2 Video Assets'),
    );
    
    if (count($assets)) {
      $asset_items = array();
      $presets = _cdn2_get_available_presets();
      $source = array();
      $source['asset_fetch_url'] = 'http://us.cloudfront.cdn2.net/'. $node->{$field['field_name']}[0]['video_token'];
      $source_preset = new stdClass();
      
      $source_preset->friendlyName = 'Original Video';
      $asset_items[] = theme('cdn2_video_asset_generic', $node, $source, $source_preset);
      foreach ($assets as $asset) {
        $asset_items[] = theme('cdn2_video_asset_generic', $node, $asset, $presets[$asset['preset_name']]);
      }
      $video_url = $asset['asset_fetch_url']; 
      $output = theme('item_list', $asset_items);
    } else {
      $output = t('Your video is either still transcoding or no formats were selected.');
    }
    $asset_drop_url = variable_get('clean_url', 0) ? "/cdn2/drop-asset/" : "?q=/cdn2/drop-asset/";
    $output .= 'You can reset the assets for this node by dropping them <a href="'. $asset_drop_url. $node->nid .'">here.</a>';

    $form[$field['field_name'] .'_fieldset'][$field['field_name'] .'_markup'] = array(
      '#type' => 'markup',
      '#value' => $output
    );
    $form[$field['field_name'] .'_images'] = array(
      '#type' => 'fieldset',
      '#title' => t('CDN2 Video Splash Image'),
    );
    $images = theme('cdn2_video_asset_image_generate', $node, $assets, $field['field_name']);
    $disclaimer = '<strong>NOTE: These sizes do not reflect the sizes that will show. Image sizes will be the same size as the Video Format. They are shown here at width="200" for space reasons.</strong>';
    $form[$field['field_name'] .'_images'][$field['field_name'] .'_image'] = array(
      '#type' => 'radios',
      '#default_value' => _cdn2_get_splash_image_number($node->nid),
      '#options'=> $images,
      '#prefix' => '<div class="cdn2_images_form">'.$disclaimer .'<br>',
      '#suffix' => '</div>',
    );

  }
  if ($show_form) {
    // TODO: handle resizing of the iframe to its child content's scroll height, e.g.:
    // document.getElementById('cdn2_iframe').height = document.getElementById('cdn2_iframe').contentWindow.document.body.scrollHeight;
    
    drupal_add_js(drupal_get_path('module', 'cdn2') .'/js/progress_update.js', 'module');
    drupal_add_js(drupal_get_path('module', 'cdn2') .'/js/jquery.progressbar.min.js', 'module');
    theme('cdn2_progress_image_settings');
    if ($node->nid) {
      $form['uploader'] = array(
        '#type' => 'markup',
        '#value' => '<iframe id="cdn2_iframe" class="cdn2" height="350" width="100%" src="'. url('cdn2/uploadform') .'/'. $video_token .'/'. $node->nid .'">CDN2 Video requires iframe support</iframe>',
        '#weight' => $field['widget']['weight'],
      );
    }else {
      $form['uploader'] = array(
        '#type' => 'markup',
        '#value' => '<iframe id="cdn2_iframe" class="cdn2" height="350" width="100%" src="'. url('cdn2/uploadform') .'/'. $video_token .'">CDN2 Video requires iframe support</iframe>',
        '#weight' => $field['widget']['weight'],
      );
    }
  }
return $element;
}
/*    case 'validate':
      if ($node->cdn2_is_submitted == 'false') {
        form_set_error('uploader', t('You must upload a video to continue.'));
      }
      break;
    case 'process form values':
      //$node->cdn2 = array('value'=>$node->cdn2);
      break;
    }
}
*/

