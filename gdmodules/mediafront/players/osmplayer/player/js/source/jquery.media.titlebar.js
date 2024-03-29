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
(function($) { 
   jQuery.media.ids = jQuery.extend( jQuery.media.ids, {
      titleLinks:"#mediatitlelinks"                 
   });     
   
   jQuery.fn.mediatitlebar = function( settings ) { 
      if( this.length === 0 ) {
         return null;
      }
      return new (function( titleBar, settings ) {        
         // Save the jQuery display.
         var _this = this;
         this.display = titleBar;
         
         this.titleLinks = this.display.find( settings.ids.titleLinks );
         
         this.display.find("a").each( function() {
            var linkId = $(this).attr("href");
            
            $(this).medialink( settings, function( event ) {
               event.preventDefault(); 
               _this.display.trigger( event.data.id );               
            }, {
               id:linkId.substr(1),
               obj:$(this)
            } );
         });
      })( this, settings );
   };
})(jQuery);
