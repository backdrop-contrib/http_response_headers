(function ($) {

    /**
     * Provide the summary information for the header rule settings vertical tabs.
     */
    Backdrop.behaviors.HttpHeaderRulesSettingsSummary = {
        attach: function (context) {
            // The backdropSetSummary method required for this behavior is not
            // available on the Blocks administration page, so we need to make
            // sure this behavior is processed only if backdropSetSummary is
            // defined.
            if (typeof jQuery.fn.backdropSetSummary == 'undefined') {
                return;
            }

            $('fieldset#edit-path', context).backdropSetSummary(function (context) {
                if (!$('textarea[name="pages"]', context).val()) {
                    return Backdrop.t('Not restricted');
                }
                else {
                    return Backdrop.t('Restricted to certain pages');
                }
            });

            $('fieldset#edit-node-type', context).backdropSetSummary(function (context) {
                var vals = [];
                $('input[type="checkbox"]:checked', context).each(function () {
                    vals.push($.trim($(this).next('label').text()));
                });
                if (!vals.length) {
                    vals.push(Backdrop.t('Not restricted'));
                }
                return vals.join(', ');
            });

            $('fieldset#edit-role', context).backdropSetSummary(function (context) {
                var vals = [];
                $('input[type="checkbox"]:checked', context).each(function () {
                    vals.push($.trim($(this).next('label').text()));
                });
                if (!vals.length) {
                    vals.push(Backdrop.t('Not restricted'));
                }
                return vals.join(', ');
            });

            $('fieldset#edit-user', context).backdropSetSummary(function (context) {
                var $radio = $('input[name="custom"]:checked', context);
                if ($radio.val() == 0) {
                    return Backdrop.t('Not customizable');
                }
                else {
                    return $radio.next('label').text();
                }
            });
        }
    };
})(jQuery);
