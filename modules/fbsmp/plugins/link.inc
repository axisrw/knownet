<?php
// $Id: link.inc,v 1.1.2.14 2010/08/17 20:29:52 publicmind Exp $

/**
 * @file
 *   Allows users to attach links with their FBSS status.
 */

/**
 * Implementation of hook_fbsmp_admin_settings_form().
 */
function fbsmp_link_fbsmp_admin_settings_form(&$form_state) {
  $prev_settings = fbsmp_load_plugin_settings('link');
  $yesno = array(1 => t('Yes'), 0 => t('No'));
  $len_opts = drupal_map_assoc(array(10, 15, 20, 30, 40, 50, 60, 70, 80, 90, 100, 200, 300, 400, 500, 750, 1000));
  
  $form = array();
  
  $form['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
    '#collapsible' => FALSE,
  );
  $form['general']['max_title_length'] = array(
    '#type' => 'select',
    '#options' => $len_opts,
    '#title' => t('Maximum link title length'),
    '#default_value' => $prev_settings['max_title_length'],
    '#description' => t('This controls how many characters of the link title will be stored.'),
  );  
  $form['general']['max_description_length'] = array(
    '#type' => 'select',
    '#options' => $len_opts,
    '#title' => t('Maximum link description length'),
    '#default_value' => $prev_settings['max_description_length'],
    '#description' => t('This controls how many characters of the link description will be stored.'),
  );  
  $form['general']['class'] = array(
    '#type' => 'textfield',
    '#title' => t("Additional link classes"),
    '#default_value' => $prev_settings['class'],
    '#description' => t("These are extra classes that are added to the link. Separate them with spaces (as you would in HTML)"),
  );  
  $form['general']['width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width of the thumbnail'),
    '#default_value' => $prev_settings['width'],
    '#size' => 15,
    '#maxlength' => 10,
    '#description' => t('The width attribute of the img tag for thumbnail. Set to 0 for no restriction.'),
  );  
  $form['general']['height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height of the thumbnail'),
    '#default_value' => $prev_settings['height'],
    '#size' => 15,
    '#maxlength' => 10,
    '#description' => t('The height attribute of the img tag for thumbnail. Set to 0 for no restriction.'),
  );
  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['advanced']['max_imagesize'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum image size'),
    '#default_value' => $prev_settings['max_imagesize'],
    '#description' => t('Specify the size limit that applies to each image thumbnail. Enter a value like "512" (bytes), "80K" (kilobytes) or "50M" (megabytes) in order to restrict the allowed image size. Leave empty for no restrictions.'),
  );
  $form['advanced']['min_imagesize'] = array(
    '#type' => 'textfield',
    '#title' => t('Minimum image size'),
    '#default_value' => $prev_settings['min_imagesize'],
    '#description' => t('Specify the minimum size that applies to each image thumbnail. Enter a value like "512" (bytes), "80K" (kilobytes) or "50M" (megabytes) in order to restrict the allowed image size. Leave empty for no restrictions.'),
  );    
  $form['advanced']['external'] = array(
    '#type' => 'radios',
    '#options' => $yesno,
    '#prefix' => '<div class="fbsmp-radios">',
    '#suffix' => '</div>',
    '#title' => t('Open links in a new window'),
    '#default_value' => $prev_settings['external'],
    '#description' => t('Recommended to keep your vistors on your site. Note that this will cause HTML "Strict" validation errors if used.'),
  );
  // Add "rel=nofollow."
  $form['advanced']['nofollow'] = array(
    '#type' => 'radios',
    '#options' => $yesno,
    '#prefix' => '<div class="fbsmp-radios">',
    '#suffix' => '</div>',
    '#title' => t('Spam link deterrent'),
    '#default_value' => $prev_settings['nofollow'],
    '#description' => t('If enabled, Drupal will add rel="nofollow" to all links, as a measure to reduce the effectiveness of spam links. Note: this will also prevent valid links from being followed by search engines, therefore it is likely most effective when enabled for anonymous users.'),
  );
  $form['advanced']['url_stub'] = array(
    '#type' => 'textfield',
    '#title' => t('Default for URL'),
    '#default_value' => $prev_settings['url_stub'],
    '#description' => t('This will be used as a stub in the URL field.'),
  );
  $form['advanced']['cache_duration'] = array(
    '#type' => 'select',
    '#title' => t('Cache duration'),
    '#options' => drupal_map_assoc(array(0, 3600, 10800, 21600, 32400, 43200, 86400, 172800, 259200, 604800, 1209600, 2419200, 4838400, 9676800, 15724800, 31449600), 'format_interval'),
    '#default_value' => $prev_settings['cache_duration'],
    '#description' => t('The data retrieved from the link (title, description and images) will be cached for the set time period to improve performance. Set Zero (0) to disable caching.'),
  );
  $form['advanced']['timeout'] = array(
    '#type' => 'select',
    '#title' => t('Maximum execution time (in seconds)'),
    '#options' => drupal_map_assoc(array(3, 5, 10, 15, 20, 25, 30, 40, 50, 60)),
    '#default_value' => $prev_settings['timeout'],
    '#description' => t('Select the maximum amount of time to take while attaching a link.'),
  );
  
  if (module_exists('emvideo')) {
    module_load_include('inc', 'fbsmp', 'plugins/link.emvideo');
    $form['emvideo'] = array(
      '#type' => 'fieldset',
      '#title' => t('Emvideo integration'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['emvideo']['enable_emvideo'] = array(
      '#type' => 'checkbox',
      '#default_value' => $prev_settings['enable_emvideo'],
      '#title' => t('Enable emvideo integration'),
      '#description' => t('Enabling this will show the embedded video along with the status, if the shared link is from the list of the supported providers.')
    );
    $form['emvideo'] += _fbsmp_link_emvideo_settings_form($form_state, $prev_settings);
  }

  return $form;
}

/**
 * Implementation of hook_fbsmp_admin_settings_form_validate().
 */
function fbsmp_link_fbsmp_admin_settings_form_validate(&$form, &$form_state) {
  $form_values = $form_state['values'];
  //Check that CSS class is valid.
  if (!empty($form_values['class']) && !preg_match('/^[a-zA-Z]+[_a-zA-Z0-9- ]*$/', $form_values['class'])) {
    form_set_error('class', t('The HTML "class" attribute can only include spaces, underscores, hyphens, and alphanumeric characters.'));
  }
  
  //Check that set height and width are valid.
  foreach (array('height', 'width') as $attr) {
    if (!empty($form_values[$attr]) && (!is_numeric($form_values[$attr]) || $form_values[$attr] != round($form_values[$attr]) || $form_values[$attr] < 0)) {
      form_set_error($attr, t('The @attribute must be a non-negative integer.', array('@attribute' => $attr)));
    }
  }
  
  //Check that set imagesize is valid
  if (!empty($form_values['max_imagesize']) && !is_numeric(parse_size($form_values['max_imagesize']))) {
    form_set_error('max_imagesize', t('The "Maximum image size" option must contain a valid value. You can either leave the text field empty or enter a string like "512" (bytes), "80K" (kilobytes) or "50M" (megabytes).'));
  }
  
  if (!empty($form_values['min_imagesize']) && !is_numeric(parse_size($form_values['min_imagesize']))) {
    form_set_error('min_imagesize', t('The "Minimum image size" option must contain a valid value. You can either leave the text field empty or enter a string like "512" (bytes), "80K" (kilobytes) or "50M" (megabytes).'));
  }
  
  if (module_exists('emvideo')) {
    module_load_include('inc', 'fbsmp', 'plugins/link.emvideo');
    _fbsmp_link_emvideo_settings_form_validate($form, $form_state);
  }
}

/**
 * Implementation of hook_fbsmp_admin_settings_form_submit().
 */
function fbsmp_link_fbsmp_admin_settings_form_submit(&$form, &$form_state) {
  $form_values = $form_state['values'];
  $settings = array();
  $settings['max_title_length'] = $form_values['max_title_length'];
  $settings['max_description_length'] = $form_values['max_description_length'];
  $settings['class'] = $form_values['class'];
  $settings['width'] = $form_values['width'];
  $settings['height'] = $form_values['height'];
  $settings['max_imagesize'] = $form_values['max_imagesize'];
  $settings['min_imagesize'] = $form_values['min_imagesize'];
  $settings['external'] = $form_values['external'];
  $settings['nofollow'] = $form_values['nofollow'];
  $settings['url_stub'] = $form_values['url_stub'];
  $settings['cache_duration'] = $form_values['cache_duration'];
  $settings['timeout'] = $form_values['timeout'];
  
  if (module_exists('emvideo')) {
    $settings['enable_emvideo'] = $form_values['enable_emvideo'];
    module_load_include('inc', 'fbsmp', 'plugins/link.emvideo');
    $settings += _fbsmp_link_emvideo_settings_form_submit($form, $form_state);
  }
  
  fbsmp_save_plugin_settings($settings, 'link');
}

/**
 * Implementation of hook_fbsmp_widget_form().
 */
function fbsmp_link_fbsmp_widget_form(&$form_state) {
  $form = array();
  $settings = fbsmp_load_plugin_settings('link');
  if (isset($form_state['fbsmp_link']['step']) && $form_state['fbsmp_link']['step'] == 2) {
    $data = $form_state['fbsmp_link']['data'];
    if (isset($data['images']) && is_array($data['images']) && count($data['images']) > 0) {
      $delta = 0;
      foreach ($data['images'] as $image_url => $size) {
        $delta++;
        $form['fbsmp_link_thumbnail_'. $delta] = array(
          '#type' => 'hidden',
          '#access' => TRUE,
          '#attributes' => array('class' => 'fbsmp-link-thumbnail-'. $delta),
          '#value' => $image_url,
        );
      }
      $form['fbsmp_link_thumbnail_count'] = array(
        '#type' => 'hidden',
        '#access' => TRUE,
        '#value' => $delta,
      );
      $current_thumbnail = isset($form_state['fbsmp_link']['current_thumbnail']) ? $form_state['fbsmp_link']['current_thumbnail'] : 1;
      $form['fbsmp_link_current_thumbnail'] = array(
        '#type' => 'hidden',
        '#access' => TRUE,
        '#default_value' => $current_thumbnail,
      );
      $form['fbsmp_link_thumbnail_select'] = array(
        '#value' => fbsmp_link_render_thumbnail_select($form['fbsmp_link_thumbnail_'. $current_thumbnail]['#value'], $current_thumbnail, $delta, $settings),
      );
      $form['fbsmp_link_no_thumbnail'] = array(
        '#type' => 'checkbox',
        '#title' => t('No thumbnail'),
        '#default_value' => 0,
      );
    }
    $title = $form_state['fbsmp_link']['data']['title'];
    $description = $form_state['fbsmp_link']['data']['description'];
    
    //"\xE2\x80\xA6" is the UTF8 character sequence for the ellipsis, which must be enclosed in double quotes.
    //Neither the literal binary character (?) nor the HTML entity (&hellip;) work on all operating systems. 
    if (drupal_strlen($title) > $settings['max_title_length']) {
      $title = drupal_substr($title, 0, $settings['max_title_length'] - 3) ."\xE2\x80\xA6";
    }
    if (drupal_strlen($description) > $settings['max_description_length']) {
      $description = drupal_substr($description, 0, $settings['max_description_length'] - 3) ."\xE2\x80\xA6";
    }
    $form['fbsmp_link_title'] = array(
      '#title' => t('Title'),
      '#type' => 'textfield',
      '#maxlength' => $settings['max_title_length'],
      '#default_value' => $title,
    );
    $form['fbsmp_link_description'] = array(
      '#title' => t('Description'),
      '#type' => 'textarea',
      '#maxlength' => $settings['max_description_length'],
      '#default_value' => $description,
      '#resizable' => FALSE,
    );    
    $form['fbsmp_link_data'] = array(
      '#type' => 'value',
      '#value' => $form_state['fbsmp_link'],
    );
  }
  else {
    $form['fbsmp_link_url'] = array(
      '#type' => 'textfield',
      '#maxlength' => 255,
      '#default_value' => $settings['url_stub'],
      '#settings' => $settings,
    );

    $form['fbsmp_link_attach'] = array(
      '#type' => 'submit',
      '#value' => t('Attach'),
      '#validate' => array('fbsmp_link_attach_validate'),
      '#submit' => array('fbsmp_link_attach_submit'),
      '#attributes' => array('class' => 'fbsmp-link-attach-button'),
      '#ahah' => array(
        'path' => 'fbsmp/js',
        'wrapper' => 'fbsmp-wrapper-replace',
      ),
    );
  }
  return $form;
}

/**
 * Implementation of hook_fbsmp_widget_form_validate().
 */
function fbsmp_link_fbsmp_widget_form_validate(&$form, &$form_state) {
  //Set 'fbsmp_link' in case validation error happens,
  //we would need to rebuild the form and will need these values.
  $form_state['fbsmp_link'] = $form_state['values']['fbsmp_link_data'];
  $form_state['fbsmp_link']['current_thumbnail'] = $form_state['values']['fbsmp_link_current_thumbnail'];
}

/**
 * Implementation of hook_fbsmp_widget_form_save().
 */
function fbsmp_link_fbsmp_widget_form_save($form ,$form_state) {
  $record = array();
  $record['page_url'] = $form_state['values']['fbsmp_link_data']['page_url'];
  $delta = $form_state['values']['fbsmp_link_current_thumbnail'];
  if (!$form_state['values']['fbsmp_link_no_thumbnail']) {
    $record['thumbnail'] = $form_state['values']['fbsmp_link_thumbnail_'. $delta];
    $record['emvideo_valid'] = $form_state['values']['fbsmp_link_data']['data']['emvideo_valid'];
    $record['emvideo_data'] = $form_state['values']['fbsmp_link_data']['data']['emvideo_data'];
  }
  $record['title'] = trim($form_state['values']['fbsmp_link_title']);
  $record['description'] = trim($form_state['values']['fbsmp_link_description']);
  return $record;
}

/**
 * Implementation of hook_fbsmp_subtheme().
 */
function fbsmp_link_fbsmp_subtheme() {
  return array(
    'fbsmp_link_attachment'  => array(
      'arguments' => array('attachment' => NULL, 'settings' => NULL),
      'file' => 'plugins/link.inc',
    ),
    'fbsmp_link_thumbnail'  => array(
      'arguments' => array('attachment' => NULL, 'settings' => NULL),
      'file' => 'plugins/link.inc',
    ),
    'fbsmp_link_url'  => array(
      'arguments' => array('attachment' => NULL, 'settings' => NULL),
      'file' => 'plugins/link.inc',
    ),
    'fbsmp_link_emvideo' => array(
      'arguments' => array('item' => NULL, 'settings' => NULL),
      'file' => 'plugins/link.emvideo.inc',
    ),
  );
}

/**
 * Implementation of hook_fbsmp_themed_attachment().
 */
function fbsmp_link_fbsmp_themed_attachment($attachment) {
  $settings = fbsmp_load_plugin_settings('link');
  return theme('fbsmp_link_attachment', $attachment, $settings);
}

/**
 * Themes a link.
 *
 * @param $attachment
 *   The attacment object.
 * @param $settings
 * (optional) An array which can have one or more of following keys:
 *   - width
 *       An integer specifying the width attribute of the img tag.
 *   - height
 *       An integer specifying the height attribute of the img tag.
 *   - class
 *       A string specifying the class attribute of the link.
 *   - external
 *       A boolean specifying whether to add target="_blank" 
 *       attribute to the link.
 *   - nofollow
 *       A boolean specifying whether to add rel="nofollow" 
 *       attribute to the link.
 *   - emvideo_valid
 *       A boolean specifying whether emvideo data was collected for
 *       for the URL.
 *   - emvideo_width
 *       An integer specifying the height of the embedded video.
 *   - emvideo_height
 *       An integer specifying the width of the embedded video.
 *   - emvideo_autoplay
 *       A boolean specifying whether to autoplay the video on page load.
 *   - emvideo_providers
 *       An array specifying the available providers.
 *
 * @return
 *   Return a link themed with title, description and thumbnail or embedded video.
 */
function theme_fbsmp_link_attachment($attachment, $settings = array()) {
  $attachment_data = $attachment->data;
  $output = theme('fbsmp_link_url', $attachment, $settings);
  if (isset($attachment_data['description']) && $attachment_data['description']) {
    $output .= '<div class="fbsmp-link-description">'. check_plain($attachment_data['description']) .'</div>';
  }
  return $output;
}

/**
 * Themes an image thumbnail.
 *
 * @param $attachment
 *   The attacment object.
 * @param $settings
 * (optional) An array which can have one or more of following keys:
 *   - width
 *       An integer specifying the width attribute of the img tag.
 *   - height
 *       An integer specifying the height attribute of the img tag.
 * @return
 *   Return a themed thumbnail.
 */
function theme_fbsmp_link_thumbnail($attachment, $settings = array()) {
  $attachment_data = $attachment->data;
  $output = '';
  if (isset($attachment_data['thumbnail']) && $attachment_data['thumbnail']) {
    $attributes = array();
    foreach (array('height', 'width') as $attr) {
      if (isset($settings[$attr]) && $settings[$attr]) {
        $attributes[$attr] = $settings[$attr];
      }
    }
    $attributes['class'] = "fbsmp-link-thumbnail";
    $output .= theme('image', $attachment_data['thumbnail'], $attachment_data['title'], $attachment_data['title'], $attributes, FALSE);
  }
  return $output;
}

/**
 * Themes a URL with title and thumbnail or embedded video.
 *
 * @param $attachment
 *   The attacment object.
 * @param $settings
 * (optional) An array which can have one or more of following keys:
 *   - width
 *       An integer specifying the width attribute of the img tag.
 *   - height
 *       An integer specifying the height attribute of the img tag.
 *   - class
 *       A string specifying the class attribute of the img tag.
 *   - external
 *       A boolean specifying whether to add target="_blank" 
 *       attribute to the link.
 *   - nofollow
 *       A boolean specifying whether to add rel="nofollow" 
 *       attribute to the link.
 *   - emvideo_valid
 *       A boolean specifying whether emvideo data was collected for
 *       for the URL.
 *   - emvideo_width
 *       An integer specifying the height of the embedded video.
 *   - emvideo_height
 *       An integer specifying the width of the embedded video.
 *   - emvideo_autoplay
 *       A boolean specifying whether to autoplay the video on page load.
 *   - emvideo_providers
 *       An array specifying the available providers.
 * @return
 *   Return a URL themed with title and thumbnail or embedded video.
 */
function theme_fbsmp_link_url($attachment, $settings = array()) {
  $attachment_data = $attachment->data;
  $output = '';
  $emvideo_output = '';
  if ($attachment_data['emvideo_valid']) {
    $emvideo_output = theme('fbsmp_link_emvideo', $attachment_data['emvideo_data'], $settings);
  }
  else {
    $output = theme('fbsmp_link_thumbnail', $attachment, $settings);
  }
  $output .= '<div class="fbsmp-link-title">'. check_plain($attachment_data['title']) .'</div>';
  
  $options = array(
    'attributes' => array(),
    'html' => TRUE,
  );
  if (isset($settings['class']) && $settings['class']) {
    $options['attributes']['class'] = $settings['class'];
  }
  if (isset($settings['external']) && $settings['external']) {
    $options['attributes']['target'] = '_blank';
  }
  if (isset($settings['nofollow']) && $settings['nofollow']) {
    $options['attributes']['rel'] = 'nofollow';
  }
  
  $output = l($output, $attachment_data['page_url'], $options);
  return $emvideo_output . $output;
}

/**
 * Returns the themed form element for thumbnail selection.
 */
function fbsmp_link_render_thumbnail_select($image_url, $current, $images_count, $settings = array()) {
  $output = '';
  $output .= '<div class="fbsmp-link-images">';
  
  $attributes = array();
  foreach (array('height', 'width') as $attr) {
    if (isset($settings[$attr]) && $settings[$attr]) {
      $attributes[$attr] = $settings[$attr];
    }
  }
  $attributes['class'] = "fbsmp-link-thumbnail-select";
  $output .= theme('image', $image_url, '', '', $attributes, FALSE);  
  $images_path = drupal_get_path('module', 'fbsmp') . '/plugins/images';
  
  $output .= theme('image', $images_path .'/previous.png', t('Previous'), '', array('class' => 'fbsmp-link-previous-button'), FALSE);
  $output .= theme('image', $images_path .'/next.png', t('Next'), '', array('class' => 'fbsmp-link-next-button'), FALSE);
  
  $output .= '<span class="fbsmp-link-chars">'. t('%current of %max', array('%current' => $current, '%max' => $images_count)) .'</span>';
  $output .= '</div>';
  return $output;
}

/**
 * Implementation of hook_fbsmp_token_list().
 */
function fbsmp_link_fbsmp_token_list() {
  return array(
    'link-title' => t('The page title of the shared link (No HTML allowed).'),
    'link-description' => t('The page summary of the shared link (No HTML allowed).'),
    'link-url' => t('The shared URL.'),
    'link-thumbnail' => t('The URL of the selected thumbnail.'),
    'link-themed-url' => t('The themed URL which was shared, with thumbnail (or embedded video) and title.'),
    'link-themed-thumbnail' => t('The themed thumbnail for the shared link.'),
  );
}

/**
 * Implementation of hook_fbsmp_token_values().
 */
function fbsmp_link_fbsmp_token_values($attachment) {
  $attachment_data = $attachment->data;
  $settings = fbsmp_load_plugin_settings('link');
  return array(
    'link-title' => check_plain($attachment_data['title']),
    'link-description' => check_plain($attachment_data['description']),
    'link-thumbnail' => check_url($attachment_data['thumbnail']),
    'link-url' => check_url($attachment_data['page_url']),
    'link-themed-url' => theme('fbsmp_link_url', $attachment, $settings),
    'link-themed-thumbnail' => theme('fbsmp_link_thumbnail', $attachment, $settings),
  );
}

//===================
// HELPER FUNCTIONS.
//===================

/**
 * Retrieves title, description, and images for a given url (web page).
 *
 * @param $url
 *   the url for the request
 * @param $options
 *   (optional) An array which can have one or more of following keys:
 *   - cached
 *       if TRUE, the result of this request will be cached. Defaults to TRUE.
 *   - max_redirects
 *       An integer representing how many times a redirect may be followed.
 *       Defaults to 3.
 *   - timeout
 *       A float representing the maximum number of seconds the function call
 *       may take. The default is 30 seconds.
 *   - max_imagesize
 *       The maximum size of the image that must be considered. The default is 
 *       512 K. Set 0 for no restriction.
 *   - min_imagesize
 *       The minimum size of the image that must be considered. The default is 
 *       0 bytes.
 *   - hash_extra
 *       The key for caching is created from the url. If you want to add extra
 *       information to the key, use 'hash_extra'
 *   - cache_duration
 *       if cached is TRUE, set this to the time period after which the cache entry 
 *       should expire (in seconds). Default is 9676800 seconds (16 weeks).
 *
 * @return
 *   An array which can have the following parameters:
 *   - title
 *       A string containing the title of the requested web page.
 *   - description
 *       A string containing the description of the requested web page.
 *   - images
 *       An array containing the image_url/image_size as key/value pairs.
 */
function fbsmp_link_request_contents($url, $options = array()) {
  //Merge the default options.
  $options += array(
    'cached' => TRUE,
    'max_redirects' => 3,
    'timeout' => 30,
    'max_imagesize' => '512K',
    'min_imagesize' => 0,
    'hash_extra' => FALSE,
    'cache_duration' => 9676800,
  );
  
  //Create the cid for the cache table.
  $arghash = md5($url);
  if ($options['hash_extra']) {
    $arghash .= ':'. $options['hash_extra'];
  }

  $data = array(
    'images' => array(), 
    'title' => t('Title'),
    'description' => '',
  );
  $emvideo = array(
    'emvideo_valid' => FALSE,
    'emvideo_data' => array(),
    'images' => array(),
  );
  
  //Check if the URL is from the supported video providers. We do not cache this content.
  if (module_exists('emvideo') && $options['enable_emvideo']) {
    module_load_include('inc', 'fbsmp', 'plugins/link.emvideo');
    $item = _fbsmp_link_emvideo_data($url, $options);
    if (is_array($item) && $item) {
      $emvideo['emvideo_valid'] = TRUE;
      $emvideo['images'] = empty($item['image']) ? array() : array($item['image'] => 0);
      $emvideo['emvideo_data'] = $item;
    }
  }
  
  //If it is a cachable request, try to load a cached value.
  if ($options['cached'] && $cache = cache_get($arghash, 'cache')) {
    return $emvideo['emvideo_valid'] ? array_merge($cache->data, $emvideo) : $cache->data;
  }

  timer_start(__FUNCTION__);
  if (!fbsmp_include_library('http_request.inc', 'http_request')) {
    fbsmp_log_error(t("Could not find http_request.inc file in the libraries folder"));
    return $data;
  }
  
  $result = fbsmp_http_request($url, array('timeout' => $options['timeout']));
  
  if ($result->code != 200) {
    fbsmp_log_error(t("Could not connect to @url, HTTP error @error", array('@error' => $result->code, '@url' => $url)));
    return $data;
  }
  
  $page_url = $url;
  if (in_array($result->redirect_code, array(301, 302, 307))) {
    $page_url = $result->redirect_url;
  }
  
  $document = new DOMDocument();
  if (@$document->loadHTML($result->data) === FALSE) {
    fbsmp_log_error(t("Could not parse the content on @url", array('@url' => $url)));
    return $data;
  }
  
  $xpath         = new DOMXPath($document);
  
  //Get title of the page.
  $titles        = @$xpath->evaluate("//title");
  $data['title'] = trim($titles->length > 0 ? $titles->item(0)->nodeValue : $data['title']);
  //Get description of the page.
  $descriptions  = @$xpath->evaluate("//meta[@name='description']");
  $data['description'] = trim($descriptions->length > 0 ? $descriptions->item(0)->getAttribute('content') : $data['description']);
  
  if (!$emvideo['emvideo_valid']) {
    $hrefs         = @$xpath->evaluate("/html/body//img");
    $images = array();
    
    if (!fbsmp_include_library('url_to_absolute.inc', 'url_to_absolute')) {
      fbsmp_log_error(t("Could not find url_to_absolute.inc file in the libraries folder"));
      return $data;
    }
  
    for ($i = 0; $i < $hrefs->length; $i++) {
      $image_url = $hrefs->item($i)->getAttribute('src');
      if (!isset($image_url) || empty($image_url)) {
        continue;
      }
      $abs_url = url_to_absolute($page_url, $image_url);
      if ($abs_url && ($imagesize = _fbsmp_link_validate_filesize($abs_url, $options['max_imagesize'], $options['min_imagesize'], ($options['timeout'] - timer_read(__FUNCTION__) / 1000))) != -1) {
        $images[$abs_url] = $imagesize;
      }   
      if (($options['timeout'] - timer_read(__FUNCTION__) / 1000) <= 0) {
        fbsmp_log_error(t("Request timed out for @url", array('@url' => $url)));
        break;
      }
    }
    asort($images);
    $images = array_reverse($images, TRUE);
    $data['images'] = $images;
  }
  //Cache the results.
  if ($options['cached']) {
    cache_set($arghash, $data, 'cache', time() + $options['cache_duration']);
  }
  
  return $emvideo['emvideo_valid'] ? array_merge($data, $emvideo): $data;  
}

/**
 * Validates the filesize of a remote URL.
 *
 * @param $file_url
 *   The file url.
 * @param $max_size
 *   The maximum size of the file. Set 0 for no restriction.
 * @param $min_size
 *   The minimum size of the file. Set 0 for no restriction.
 * @param $timeout
 *   A float representing the maximum number of seconds the function call
 *   may take. The default is 10 seconds.
 * @param $max_redirects
 *   An integer representing how many times a redirect may be followed.
 *   Defaults to 3.
 * 
 * @return
 *   An integer representing the filesize or -1 in case of error.
 */
function _fbsmp_link_validate_filesize($file_url, $max_size = 0, $min_size = 0, $timeout = 10, $max_redirects = 3) {
  $options = array(
    'method' => 'HEAD',
    'max_redirects' => $max_redirects,
    'timeout' => $timeout,
  );
  fbsmp_include_library('http_request.inc', 'http_request');
  $result = fbsmp_http_request($file_url, $options);
  $max_size = parse_size($max_size);
  $min_size = parse_size($min_size);
  if ($result->code == 200 && (!$max_size || (isset($result->headers['Content-Length']) && $result->headers['Content-Length'] < $max_size)) && (!$min_size || (isset($result->headers['Content-Length']) && $result->headers['Content-Length'] > $min_size))) {
    return isset($result->headers['Content-Length']) ? $result->headers['Content-Length'] : 0;
  }
  return -1;
}