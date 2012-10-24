<?php
/**
 * A Views attachment that when attached to a view that has a node
 * argument and a vocabulary filter set to one vocabulary will append a tagging form
 * that is wired up to the view. To have checkboxes prepended to the tag links in
 * the view, the tag links must have a CSS class of "ct-tag". In Views 2 on the
 * term field setting check "Output this field as a link", add "ct-tag" to
 * "Link class", and check "Link this field to it's taxonomy term page".
 *
 * Any suggestions as to how this can be improved are very welcome. Please raise an issue!
 *
 * With a view to keeping this loosely coupled, the attachment adds the following JS settings
 * to the JS settings array:
 *  - 'source' => 'views_tagform_attachment' this is used to identify an ajax
 *    tagging form submission as having come from a form provided by this attachment.
 *    The alter hook in community_tags_views.module rebuilds the view and adds
 *    is to the ajax response.
 *  - 'extra' => Any settings that are needed to rebuild the view are put here
 *    and are available in the form POST.
 *  - 'links_container_class' => this helps identify the tag links to the community tags JS
 *    that e.g. prepends checkboxes to each link.
 */
class community_tags_views_plugin_attachment_tagform extends views_plugin_display_attachment {
  function execute() {
    // Test that the view we're attached to has built. If is hasn't it might be because the node argument didn't validate.
    if(!empty($this->view->old_view[1]) && $this->view->old_view[1]->built) {
      return $this->render();
    }
  }

  function render() {
    global $user;

    $nid = $this->_get_node_argument();
    $vid = $this->_get_vocabulary();

    if($nid && $vid) {
      $node = node_load($nid);
      $tags = community_tags_get_user_node_tags($user, $node, $vid);

      $output = '';

      module_load_include('inc', 'community_tags', 'community_tags.pages');

      // set a container class to identify the whole view
      $views_container_class = 'ct-container-class';
      $extra = array();

      $old_view = $this->view->old_view[1];
      $css_class = $old_view->display_handler->get_option('css_class');
      $css_class .= !empty($css_class) ? ' '.$views_container_class : $views_container_class;
      $old_view->display_handler->set_option('css_class', $css_class);

      $extra['view_name'] = $old_view->name;
      $extra['view_display_id'] = $old_view->current_display;
      $extra['view_args'] = check_plain(implode('/', $old_view->args));
      $extra['view_path'] = check_plain($_GET['q']);
      // Pass through URL to ensure we get e.g. language prefixes.
      $extra['view_base_path'] = $old_view->get_path();

      static $dom_id = 5;
      $this->view->old_view[1]->dom_id = !empty($old_view->dom_id) ? $old_view->dom_id : 'ct'.$dom_id++;
      $vars['classes_array'][] = 'view-dom-id-' . $vars['dom_id'];

      $extra['view_dom_id'] = $this->view->old_view[1]->dom_id;
      // To fit multiple views on a page, the programmer may have
      // overridden the display's pager_element.
      $extra['pager_element'] = $old_view->pager['element'];

      if (!count($tags) || user_access('edit own tags')) {
        if(user_access('edit own tags')) {
          // User has already tagged this node, but can edit their tags. Show form
          // with the user's tags pre-populated.
          $names = community_tags_flatten($tags);
          $tags = taxonomy_implode_tags($tags);
        }
        else {
          $tags = NULL;
        }

        // User has not yet added tags to this node yet. Show form.
        if(!isset($_REQUEST['js']) && $this->view->use_ajax) {
          _community_tags_views_add_js();
          $output .= drupal_get_form('community_tags_form', array('node' => $node, 'cloud' => $cloud, 'nid' => $node->nid, 'vid' => $vid, 'tags' => $tags, 'inline' => $inline, 'links_container_class' => $views_container_class, 'extra' => $extra, 'source' => 'views_tagform_attachment'));
        }
        else {
          $output .= drupal_get_form('community_tags_form', array('node' => $node, 'cloud' => $cloud, 'nid' => $node->nid, 'vid' => $vid, 'tags' => $tags, 'inline' => $inline, 'links_container_class' => $views_container_class, 'extra' => $extra, 'source' => 'views_tagform_attachment'));
        }
      }
    }

    return $output;
  }

  function _get_node_argument() {
    if($handlers = $this->get_handlers('argument')) {
      $keys = array_keys($handlers);
      $index = array_search('nid', $keys);
      if ($index !== FALSE) {
        if(!empty($this->view->args[$index])) {
          return $this->view->args[$index];
        }
      }
    }
  }

  function _get_vocabulary() {
    // no idea how reliable this is - thee must be an easier way...
    if(!empty($this->view->old_view[1])) {
      $old_view = $this->view->old_view[1];
      if(isset($old_view->filter['vid'])) {
        $vid_filter = $old_view->filter['vid'];
        if(!is_array($vid_filter->value)) {
          return $vid_filter->value;
        } elseif (count($vid_filter->value) == 1) {
          return reset($vid_filter->value);
        }
      }
    }
  }
}
