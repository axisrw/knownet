// $Id: cdn2.js,v 1.1.2.1 2009/03/22 21:46:57 ocyrus Exp $

$(document).ready(
function() {
    var oDom = $('div.cdn2_tabs > ul');
    if (oDom.length > 0) {
	oDom.tabs();
	var nIndex = parseInt(aPresetList[cdn2_default_preset]);  // this is '1'
	oDom.tabs('select', nIndex);
    }
}
);