// $Id: cdn2_admin.js,v 1.3.2.4 2009/03/22 21:46:58 ocyrus Exp $

$(document).ready(
    function() {
        $('form#cdn2-format-settings input:checkbox').click(cdn2_allowed_presets_change);
    }
);

function cdn2_allowed_presets_change() {

    // clear out the select box first
    //
    var oSelect = $('select#edit-cdn2-default-preset');
    // grab currently selected item
    //
    var sCurrentIndex = oSelect.val();
    oSelect.removeOption(/./);
    oSelect.addOption('-1', "Select...");
    var aOptions = $('form#cdn2-format-settings input:checkbox:checked');
    for (var i = 0; i < aOptions.length; i++) {
        var sVal = $(aOptions[i]).val();
        var presetCaption = aPresetList[sVal];
        if (presetCaption) {
            oSelect.addOption(sVal, presetCaption);
        }
    }

    if (oSelect.containsOption(sCurrentIndex) && sCurrentIndex != 'Select...') {
        oSelect.selectOptions(sCurrentIndex);
    }
}