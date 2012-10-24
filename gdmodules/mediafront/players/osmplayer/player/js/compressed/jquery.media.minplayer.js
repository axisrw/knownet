/**
 *  Copyright (c) 2010 Alethia Inc,
 *  http://www.alethia-inc.com
 *  Developed by Travis Tidwell | travist at alethia-inc.com 
 *
 *  License:  GPL version 3.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *  
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.

 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 */
(function(a){jQuery.media=jQuery.media?jQuery.media:{};jQuery.media.defaults=jQuery.extend(jQuery.media.defaults,{logo:"logo.png",logoWidth:49,logoHeight:15,logopos:"sw",logox:5,logoy:5,link:"http://www.mediafront.org",file:"",image:"",timeout:2000,autoLoad:true});jQuery.media.ids=jQuery.extend(jQuery.media.ids,{busy:"#mediabusy",preview:"#mediapreview",play:"#mediaplay",media:"#mediadisplay",control:"#mediacontrol"});jQuery.fn.minplayer=function(b){if(this.length===0){return null;}return new (function(d,e){e=jQuery.media.utils.getSettings(e);this.display=d;var g=this;this.autoLoad=e.autoLoad;this.controller=null;this.activeController=null;this.busy=d.find(e.ids.busy);this.busyImg=this.busy.find("img");this.busyWidth=this.busyImg.width();this.busyHeight=this.busyImg.height();this.play=d.find(e.ids.play);this.play.bind("click",function(){g.togglePlayPause();});this.playImg=this.play.find("img");this.playWidth=this.playImg.width();this.playHeight=this.playImg.height();this.preview=null;this.usePlayerControls=false;this.busyFlags=0;this.busyVisible=false;this.playVisible=false;this.previewVisible=false;this.controllerVisible=true;this.hasMedia=false;this.playing=false;this.width=this.display.width();this.height=this.display.height();this.showElement=function(j,h,i){if(j&&!this.usePlayerControls){if(h){j.show(i);}else{j.hide(i);}}};this.showPlay=function(h,i){this.playVisible=h;this.showElement(this.play,h,i);};this.showBusy=function(j,h,i){if(h){this.busyFlags|=(1<<j);}else{this.busyFlags&=~(1<<j);}this.busyVisible=(this.busyFlags>0);this.showElement(this.busy,this.busyVisible,i);};this.showPreview=function(h,i){this.previewVisible=h;if(this.preview){this.showElement(this.preview.display,h,i);}};this.showController=function(h,i){this.controllerVisible=h;if(this.controller){this.showElement(this.controller.display,h,i);}};this.onControlUpdate=function(h){if(this.media){if(this.media.playerReady){switch(h.type){case"play":this.media.player.playMedia();break;case"pause":this.media.player.pauseMedia();break;case"seek":this.media.player.seekMedia(h.value);break;case"volume":this.media.player.setVolume(h.value);break;case"mute":this.media.mute(h.value);break;}}else{if((this.media.playQueue.length>0)&&!this.media.mediaFile){this.autoLoad=true;this.playNext();}}if(e.template&&e.template.onControlUpdate){e.template.onControlUpdate(h);}}};this.fullScreen=function(h){if(e.template.onFullScreen){e.template.onFullScreen(h);}};this.onPreviewLoaded=function(){this.previewVisible=true;};this.onMediaUpdate=function(h){switch(h.type){case"paused":this.playing=false;this.showPlay(true);this.showBusy(1,false);break;case"update":case"playing":this.playing=true;this.showPlay(false);this.showBusy(1,false);this.showPreview((this.media.mediaFile.type=="audio"));break;case"initialize":this.playing=false;this.showPlay(true);this.showBusy(1,this.autoLoad);this.showPreview(true);break;case"buffering":this.showPlay(true);this.showBusy(1,true);this.showPreview((this.media.mediaFile.type=="audio"));break;}if(this.controller){this.controller.onMediaUpdate(h);}if(this.activeController){this.activeController.onMediaUpdate(h);}if(e.template&&e.template.onMediaUpdate){e.template.onMediaUpdate(h);}};this.addController=function(i,h){if(i){i.display.bind("controlupdate",i,function(j,k){g.activeController=j.data;g.onControlUpdate(k);});if(h&&!this.activeController){this.activeController=i;}}return i;};this.media=this.display.find(e.ids.media).mediadisplay(e);if(this.media){this.media.display.bind("mediaupdate",function(h,i){g.onMediaUpdate(i);});}this.controller=this.addController(this.display.find(e.ids.control).mediacontrol(e),false);if(jQuery.media.controllers[e.id]){var f=jQuery.media.controllers[e.id];var c=f.length;while(c--){this.addController(f[c],true);}}this.setSize=function(l,j){this.width=l?l:this.width;this.height=j?j:this.height;if((this.width>0)&&(this.height>0)){this.setLogoPos();if(this.preview){this.preview.resize(this.width,this.height);}var i=Math.ceil((this.width-this.busyWidth)/2);var m=Math.ceil((this.height-this.busyHeight)/2);this.busy.css({width:this.width,height:this.height});this.busyImg.css({marginLeft:i,marginTop:m});var h=Math.ceil((this.width-this.playWidth)/2);var k=Math.ceil((this.height-this.playHeight)/2);this.play.css({width:this.width,height:this.height});this.playImg.css({marginLeft:h,marginTop:k});if(this.media){this.media.display.css({width:this.width,height:this.height});this.media.setSize(this.width,this.height);}}};this.showPlayerController=function(h){if(this.media&&this.media.hasControls()){this.usePlayerControls=h;if(h){this.busy.hide();this.play.hide();if(this.preview){this.preview.display.hide();}if(this.controller){this.controller.display.hide();}}else{this.showBusy(1,((this.busyFlags&2)==2));this.showPlay(this.playVisible);this.showPreview(this.previewVisible);this.showController(this.controllerVisible);}this.media.showControls(h);}};if(this.media){this.display.prepend('<div class="medialogo"></div>');this.logo=this.display.find(".medialogo").mediaimage(e.link);this.logo.display.css({position:"absolute",zIndex:490});this.logo.width=e.logoWidth;this.logo.height=e.logoHeight;this.logo.loadImage(e.logo);}this.setLogoPos=function(){if(this.logo){var i=parseInt(this.media.display.css("marginTop"),0);var h=parseInt(this.media.display.css("marginLeft"),0);var k=(e.logopos=="se"||e.logopos=="sw")?(i+this.height-this.logo.height-e.logoy):i+e.logoy;var j=(e.logopos=="ne"||e.logopos=="se")?(h+this.width-this.logo.width-e.logox):h+e.logox;this.logo.display.css({marginTop:k,marginLeft:j});}};this.onResize=function(i,h){if(this.controller){this.controller.onResize(i,h);}this.setSize(this.width+i,this.height+h);};this.reset=function(){this.hasMedia=false;this.playing=false;if(this.controller){this.controller.reset();}if(this.activeController){this.activeController.reset();}this.showBusy(1,this.autoLoad);if(this.media){this.media.reset();}};this.togglePlayPause=function(){if(this.media){if(this.media.playerReady){if(this.playing){this.showPlay(true);this.media.player.pauseMedia();}else{this.showPlay(false);this.media.player.playMedia();}}else{if((this.media.playQueue.length>0)&&!this.media.mediaFile){this.autoLoad=true;this.playNext();}}}};this.loadImage=function(h){this.preview=d.find(e.ids.preview).mediaimage();if(this.preview){this.preview.resize(this.width,this.height);this.preview.display.bind("imageLoaded",function(){g.onPreviewLoaded();});this.preview.loadImage(h);if(this.media){this.media.preview=h;}}};this.clearImage=function(){if(this.preview){this.preview.clear();}};this.loadFiles=function(h){this.reset();if(this.media&&this.media.loadFiles(h)&&this.autoLoad){this.media.playNext();}};this.playNext=function(){if(this.media){this.media.playNext();}};this.loadMedia=function(h){this.reset();if(this.media){this.media.loadMedia(h);}};if(e.file){this.loadMedia(e.file);}if(e.image){this.loadImage(e.image);}})(this,b);};})(jQuery);