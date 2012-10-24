<?php
// $Id: page.tpl.php 6635 2010-03-01 00:39:49Z chris $
?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="<?php print $language->language; ?>" xml:lang="<?php print $language->language; ?>">

<head>
  <title><?php print $head_title; ?></title>
  <?php print $head; ?>
  <?php print $styles; ?>
  <?php print $setting_styles; ?>
  <!--[if IE 8]>
  <?php print $ie8_styles; ?>
  <![endif]-->
  <!--[if IE 7]>
  <?php print $ie7_styles; ?>
  <![endif]-->
  <!--[if lte IE 6]>
  <?php print $ie6_styles; ?>
  <![endif]-->
  <?php print $local_styles; ?>
  <?php print $scripts;?>
  
		 <script src="AC_OETags.js" type="text/javascript"></script>
		 <script type="text/javascript"> 
		var mw_flashMovieId = "SparkWeb"; // the id/name of your flash app's HTML DOM element     
		var mw_flashContainerId = "container"; // the id/name of the flash element's surrounding div element  
		</script>
		<script type="text/javascript">
		<!--
		// -----------------------------------------------------------------------------
		// Globals
		// Major version of Flash required
		var requiredMajorVersion = 9;
		// Minor version of Flash required
		var requiredMinorVersion = 0;
		// Minor version of Flash required
		var requiredRevision = 0;
		// -----------------------------------------------------------------------------
		// -->
		</script>

</head>

<script type="text/javascript">
function jive_sparkweb_getConfig()
{
	return {
		server: "knownet2",
		connectionType: "socket",
		port: "5222",
		autoLogin: "false"
                 policyFileURL: "xmlsocket://knownet2:5229"
	};
}
</script>
<script type="text/javascript">
    // Copyright 2007 www.flexcapacitor.com, www.drumbeatinsight.com 
    // Version 1.0.0
    
    // global array of html elements
    htmlControls = new Array();
    
    var timer;
    var titleStr;
    var nicknameStr;
    var bodyStr;
    var flashPlayer;
    
    
	// add HTML element to the page
    function addChild(o) {
    	if (o.type=="division") {
    		addChildDivision(o);
    	}
    	else if (o.type=="iframe") {
    		addChildIFrame(o);
    	}
    }
    
    function detectFlashPlayer(){
    try {
    	if(navigator.appName.indexOf("Microsoft") != -1){
    		flashPlayer = window.SparkWeb;
    	}
    	else {
    		flashPlayer = window.document.SparkWeb;
    	}
    }
    	catch(e){
    		alert(e);
    	}
    }
    
    window.onblur = function() {
    	if(flashPlayer != null && flashPlayer.setFocused){
			flashPlayer.setFocused(false);    	
		}
	};
	
	window.onfocus = function() {
		if(flashPlayer != null && flashPlayer.setFocused){
			flashPlayer.setFocused(true);
		}
	};
	
	
	function isReady(){
		return true;
	}
	
    
    // add iframe to the page
    function addChildIFrame(o) {
    
    	if (getElement(o.id)) {  
			// do nothing for now
    	}
    	
		var newElement = document.createElement("iframe");
		newElement.id = o.id;
		newElement.name = o.name;
		newElement.movieId = o.movieId;
		newElement.width = o.width;
		newElement.height = o.height;		
		newElement.frameBorder = o.frameborder;
		newElement.style.position = o.position;
		setSize(newElement,o.width,o.height);
		moveElementTo(newElement,o.x,o.y);
		//newElement.style.backgroundColor = "transparent";
		// always 0px - do not add a border to the iframe itself
		// add a child div and add a border to that or add border in mxml
		newElement.style.border = o.border;
		newElement.src = o.source;
		
		//  use innerHTML or DOM element creation methods to put content into body
		document.body.appendChild(newElement);
		
		newElement.onload = new function() {
			// set a flag so the application knows the page is loaded
			// looking for a reliable method that works cross browser
		}
    }
    
    // add division to the page
    function addChildDivision(o) {
		var newElement = document.createElement("div");
		newElement.id = o.id;
		newElement.name = o.name;
		newElement.movieId = o.movieId;
		
		newElement.style.position = o.position;
		setSize(newElement,o.width,o.height);
		moveElementTo(newElement,o.x,o.y);
		newElement.style.backgroundColor = "#" + o.backgroundColor;
		newElement.style.padding = "0px";
		newElement.style.margin = "0px";
		// always 0px - do not add a border to the container div tag
		// add a border in mxml or add a child div tag in the htmlText property and add a border to that
		newElement.style.border = o.border;
		newElement.innerHTML = o.htmlText;
		
		document.body.appendChild(newElement);
		setScrollPolicyById(o.id, o.htmlScrollPolicy);
		
		addToGlobalArray(newElement, o.type);
    }
    
	// add to associative array
	function addToGlobalArray(el, elementType) {
		var newElement = new Object();
		newElement.element = el;
		newElement.id = el.id;
		newElement.loaded = false;
		newElement.type = elementType;
		htmlControls[el.id] = newElement;
	}
    
	// gets the element by name
	function getElement(id) {
		if (htmlControls[id]) {
			return htmlControls[id].element;
		}
		return document.getElementById(id);
	}
	
	function appendMessage(id, message){
		try {
			var f = getElement(id);
			var p = f.contentWindow.document;
	  	    window[id].appendMessage(message);
		}
		catch(err){
			setTimeout('appendMessage("+id+", "+message+")', 3000);
		}
	}
	
	function isReady(id){
		try {
			var f = getElement(id);
			var p = f.contentWindow.document;
	  	    return window[id].isReady();
		}
		catch(err){
		}
		return false;
	}
	
	function changeTitle(title){
		window.document.title = title;
		clearTimeout(timer);
	}
	
	function alertTitle(title, username){
		clearTimeout(timer);
		window.document.title = title;
		titleStr = title;
		nicknameStr = username;
		timer = setTimeout("showNickname()", 1000);
	}
	
	function showNickname(){
		window.document.title = nicknameStr;
		timer = setTimeout("showTitle()", 1000);
	}
	
	function showTitle(){
		window.document.title = titleStr;
		timer = setTimeout("showNickname()", 1000);
	}
	
	
	function showBody(){
		window.document.title = bodyStr;
		timer = setTimeout("showTitle()", 1000);
	}
	
	function refreshPage(){
		window.location.reload( false );
	}
	
	
	
	// cannot get height of pages loaded from different domains
	// ie, page is hosted at www.yoursite.com and you load www.google.com will fail with return value of -1
	// works in ff and ie. not tested in mac browsers - 
	function getElementHeight(id){
		var el = getElement(id);
		moz = (document.getElementById && !document.all);
		
		if (el){
		
			// check the height value
			try {
			
				/*** return div height ***/
				if (el.nodeName.toLowerCase()=="div") {
					var scrollHeight = el.scrollHeight;
					var divHeight = el.style.height;
					divHeight = (scrollHeight > parseInt(divHeight)) ? scrollHeight : divHeight;
					return divHeight;
				}
				
				/*** return iframe height ***/
				//moz
				if (moz) {
					return el.contentDocument.body.scrollHeight;
				}
				else if (el.Document) {
					return el.Document.body.scrollHeight;
				}
			}
			catch(e)
			{
				//An error is raised if the IFrame domain != its container's domain
				//alert('Error: ' + e.number + '; ' + e.description+'\nPossibly - Cannot access because page domain does not equal container domain');
				return -1;
			}
		}
	}

	// get property value
	function getElementValue(id, elProperty){
		
		// if periods are in the name assume absolute path 
		// otherwise assume element id
		if (id.indexOf('.')!=-1) {
			var newArr = id.split('.');
			var elValue = "";
			
			try {
				el = window;
				for (var i=0;i < newArr.length;i++) {
					el = el[newArr[i]];
				}
				return el;
			}
			catch (e) {
				//alert("Whoooops!! Cant find " + elId);
				// should return null or undefined here
				return -1;
			}
		}
		else {
			// try and get property value
			try {
				var el = getElement(id);
				var elValue = el[elProperty];
				return elValue;
			}
			catch(e) {
				//alert("Error: Can't find " + elId + "." + elProperty);
				// should return null or undefined here
				return -1;
			}
		}
	}
	
	// get HTML content
	function getHTML(id, elementType) {
		var el = getElement(id);
		if (el!=null) {
			
			if (elementType =="division") {
				return el.innerHTML;
			}
		}
		return "";
	}

	// get reference to the flash movie	
	function getMovieById(id) {
		if (navigator.appName.indexOf ("Microsoft") !=-1) {
			return window[id];
		} else {
			return window.document[id];
		}
	}
	
	// hide element by name
	// set height of content to 0px so the 
	// flash context menu appears in the right location
	function hide(id, hideOffscreen, offscreenOffset) {
		var el = getElement(id);
		if (hideOffscreen) {
			el.style.width = "0px";
			el.style.height = "0px";
		}
		el.style.visibility = "hidden";
	}
	
	// move element to new location
	function moveElementTo(el,x,y) {
		el.style.left = parseInt(x) + "px";
		el.style.top = parseInt(y) + "px";
	}
	
	// forces redraw
	function refresh(id) {
		var el = getElement(id);
		el.style.cssText = el.style.cssText;
	}
	
	function removeEle(id){
		var olddiv = getElement(id);
  		document.body.removeChild(olddiv);  		
	}
	
	// remove
	function remove(id) {
		hide(id, true);
		var el = getElement(id);
		//el = undefined; // not sure how to delete
	}
    
	// set document title
    function setDocumentTitle(title) {
        window.document.title = title;
        return 1;
    }
	
	// set HTML content
	function setHTML(id, htmlText, elementType) {
		var el = getElement(id);
		if (el!=null) {
		
			if (elementType =="division") {
				el.innerHTML = htmlText;
			}
		}
	}
	
	// set position
	function setPosition(id,x,y) {
		var el = getElement(id);

		// LEFT
		if (x != undefined) {
			el.style.left = x + "px";
		}
		// TOP
		if (y != undefined) {
			//alert(y)
			el.style.top = y + "px";
		}
	}
	
	// set scroll policy of element
	function setScrollPolicy(el, overflow) {
		if (overflow != "resize") {
			el.style.overflow = overflow;
		}
	}
	
	// set scroll policy of element by id
	function setScrollPolicyById(id, overflow) {
		var el = getElement(id);
		
		// setting this to anything other than auto in ff fails it
		if (overflow != "resize") {
			el.style.overflow = overflow;
		}
	}
	
	// set size
	function setSize(el,w,h) {
		// WIDTH
		if (w != undefined) {
			// if width is a percentage pass in the string as is
			if (String(w).indexOf("%")!=-1) {
				el.style.width = w;
			}
			else {
				el.style.width = parseInt(w) + "px";
			}
		}
		
		// HEIGHT
		if (h!=undefined) {
			if (String(h).indexOf("%")!=-1) {
				el.style.height = h;
			}
			else {
				el.style.height = parseInt(h) + "px"; 
			}
		}
	}
	
	// set size called by id
	function setSizeByValue(id,w,h) {
		var el = getElement(id);
		if (el.style.visibility=="hidden") { return; }
		
		// WIDTH
		if (w != undefined) {
			// if width is a percentage pass in the string as is
			if (String(w).indexOf("%")!=-1) {
				el.style.width = w;
			}
			else {
				el.style.width = parseInt(w) + "px";
			}
		}
		
		// HEIGHT
		if (h!=undefined) {
			if (String(h).indexOf("%")!=-1) {
				el.style.height = h;
			}
			else {
				el.style.height = parseInt(h) + "px"; 
			}
		}
	}
	
	// set iframe location
	function setSource(id,source){
		var el = getElement(id);
		el.src = source;
	}
	
	// show element by name
	function show(id, hideOffscreen, left, width, height) {
		var el = getElement(id);
		el.style.visibility = "visible";
		if (hideOffscreen) {
			el.style.width = parseInt(width) + "px";
			el.style.height = parseInt(height) + "px";
		}
	}
	
	function showIt(id, height){
		var el = getElement(id);
		el.style.visibility = "visible";
	    el.style.height = parseInt(height - 5) + "px";
	}
		
</script>

<body id="<?php print $body_id; ?>" class="<?php print $body_classes; ?>">
  <div id="page" class="page">
    <div id="page-inner" class="page-inner">
      <div id="skip">
        <a href="#main-content-area"><?php print t('Skip to Main Content Area'); ?></a>
      </div>

      <!-- header-top row: width = grid_width -->
      <?php print theme('grid_row', $header_top, 'header-top', 'full-width', $grid_width); ?>

      <!-- header-group row: width = grid_width -->
      <div id="header-group-wrapper" class="header-group-wrapper full-width">
        <div id="header-group" class="header-group row <?php print $grid_width; ?>">
          <div id="header-group-inner" class="header-group-inner inner clearfix">
            <?php print theme('grid_block', theme('links', $secondary_links), 'secondary-menu'); ?>
            

            <?php if ($logo || $site_name || $site_slogan): ?>
            <div id="header-site-info" class="header-site-info block">
              <div id="header-site-info-inner" class="header-site-info-inner inner">
                <?php if ($logo): ?>
                <div id="logo">
                  <a href="<?php print check_url($front_page); ?>" title="<?php print t('Home'); ?>"><img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" /></a>
                </div>
                <?php endif; ?>
                <?php if ($site_name || $site_slogan): ?>
                <div id="site-name-wrapper" class="clearfix">
                  <?php if ($site_name): ?>
                  <span id="site-name"><a href="<?php print check_url($front_page); ?>" title="<?php print t('Home'); ?>"><?php print $site_name; ?></a></span>
                  <?php endif; ?>
                  <?php if ($site_slogan): ?>
                  <span id="slogan"><?php print $site_slogan; ?></span>
                  <?php endif; ?>
                </div><!-- /site-name-wrapper -->
                <?php endif; ?>
              </div><!-- /header-site-info-inner -->
            </div><!-- /header-site-info -->
            <?php endif; ?>
                       <div id="header-region" class="header-region block">
              <div id="header-region-inner" class="header-region-inner inner">
            <?php print $header; ?>
              </div><!-- /header-region-inner -->
            </div><!-- /header-region -->
          </div><!-- /header-group-inner -->
        </div><!-- /header-group -->
      </div><!-- /header-group-wrapper -->
   
   <div id="nav-group-wrapper" class="nav-group-wrapper full-width">
        <div id="nav-group" class="nav-group row <?php print $grid_width; ?>">
          <div id="nav-group-inner" class="nav-group-inner inner clearfix">
   <?php print theme('grid_block', $primary_links_tree, 'primary-menu'); ?> 
<?php print theme('grid_block', $search_box, 'search-box'); ?>
        </div><!-- /nav-group-inner -->
        </div><!-- /nav-group -->
      </div><!-- /nav-group-wrapper -->
       <?php print theme('grid_row','', $grid_width); ?>  

      <!-- preface-top row: width = grid_width -->
      <?php print theme('grid_row', $preface_top, 'preface-top', 'full-width', $grid_width); ?>

      <!-- main row: width = grid_width -->
      <div id="main-wrapper" class="main-wrapper full-width">
        <div id="main" class="main row <?php print $grid_width; ?>">
          <div id="main-inner" class="main-inner inner clearfix">
            <?php print theme('grid_row', $sidebar_first, 'sidebar-first', 'nested', $sidebar_first_width); ?>

            <!-- main group: width = grid_width - sidebar_first_width -->
            <div id="main-group" class="main-group row nested <?php print $main_group_width; ?>">
              <div id="main-group-inner" class="main-group-inner inner">
                <?php print theme('grid_row', $preface_bottom, 'preface-bottom', 'nested'); ?>

                <div id="main-content" class="main-content row nested">
                  <div id="main-content-inner" class="main-content-inner inner">
                    <!-- content group: width = grid_width - (sidebar_first_width + sidebar_last_width) -->
                    <div id="content-group" class="content-group row nested <?php print $content_group_width; ?>">
                      <div id="content-group-inner" class="content-group-inner inner">
                    

                        <?php if ($content_top || $help || $messages): ?>
                        <div id="content-top" class="content-top row nested">
                          <div id="content-top-inner" class="content-top-inner inner">
                            <?php print theme('grid_block', $help, 'content-help'); ?>
                            <?php print theme('grid_block', $messages, 'content-messages'); ?>
                            <?php print $content_top; ?>
                          </div><!-- /content-top-inner -->
                        </div><!-- /content-top -->
                        <?php endif; ?>

                        <div id="content-region" class="content-region row nested">
                          <div id="content-region-inner" class="content-region-inner inner">
                            <a name="main-content-area" id="main-content-area"></a>

                            <div id="content-inner" class="content-inner block">
                              <div id="content-inner-inner" class="content-inner-inner inner">
                            <?php if ($title): ?>
                                <h1 class="title"><?php print $title; ?></h1>
                                <?php endif; ?>
                            <?php print theme('grid_block', $tabs, 'content-tabs'); ?>
                                <?php if ($content): ?>
                                <div id="content-content" class="content-content">
                                  <?php print $content; ?>
                                  <?php print $feed_icons; ?>
                                </div><!-- /content-content -->
                                <?php endif; ?>
                              </div><!-- /content-inner-inner -->
                            </div><!-- /content-inner -->
                          </div><!-- /content-region-inner -->
                        </div><!-- /content-region -->

                        <?php print theme('grid_row', $content_bottom, 'content-bottom', 'nested'); ?>
                      </div><!-- /content-group-inner -->
                    </div><!-- /content-group -->

                    <?php print theme('grid_row', $sidebar_last, 'sidebar-last', 'nested', $sidebar_last_width); ?>
                  </div><!-- /main-content-inner -->
                </div><!-- /main-content -->

                <?php print theme('grid_row', $postscript_top, 'postscript-top', 'nested'); ?>
              </div><!-- /main-group-inner -->
            </div><!-- /main-group -->
          </div><!-- /main-inner -->
        </div><!-- /main -->
      </div><!-- /main-wrapper -->

      <!-- postscript-bottom row: width = grid_width -->
      <?php print theme('grid_row', $postscript_bottom, 'postscript-bottom', 'full-width', $grid_width); ?>

      <!-- footer row: width = grid_width -->
      <?php print theme('grid_row', $footer, 'footer', 'full-width', $grid_width); ?>

      <!-- footer-message row: width = grid_width -->
      <div id="footer-message-wrapper" class="footer-message-wrapper full-width">
        <div id="footer-message" class="footer-message row <?php print $grid_width; ?>">
          <div id="footer-message-inner" class="footer-message-inner inner clearfix">
            <?php print theme('grid_block', $footer_message, 'footer-message-text'); ?>

         </div><!-- /footer-message-inner -->
       </div><!-- /footer-message -->
      </div><!-- /footer-message-wrapper -->

    </div><!-- /page-inner -->
  </div><!-- /page -->
  <?php print $closure; ?>
</body>
</html>
