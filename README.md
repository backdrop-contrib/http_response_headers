HTTP Response Headers
=====================

The HTTP Response Headers module allows to set HTTP response headers (both standard and non-standard) on pages by various visibility rule settings. Headers can be restricted by path, content type and user role.

This module contains a sub-module, HTTP Response Headers UI, that lets you add, remove, and edit headers. Typically you would enable this module, perform your configuration, then disable it for day-to-day use.

Installation
------------

- Install this module using [the official Backdrop CMS instructions](https://backdropcms.org/guide/modules).

- Visit the configuration page under Administration > Configuration > System >
  HTTP Response Headers (`admin/config/system/http_response_headers`) to enable headers.
  
- To set specific headers and set rules for their appearance, install the HTTP Response Headers UI sub-module and visit its configuration page at Administration > Configuration > System >
  HTTP Response Headers > List Rules (`admin/config/system/http_response_headers/list`) to add, edit, and/or remove headers and to set their visibility conditions.

Typical use cases:

1. Set 'Cache-Control' or 'Expires' header to set/reset cache behavior
 of browser/cache servers.
2. Set 'X-Frame-Options' to restrict your pages rendering on a frame.
3. Set 'WWW-Authenticate' to set authentication to pages.

Note that if you set the Content-Security-Policy: frame-src header, you need to include your own site as an allowed host or it will break some functionality (e.g., AJAX file uploads).

Differences from Drupal 7
-------------------------

* All configuration information is now stored in config files rather than the db.
* Import/export functionality formerly provided by CTools is no longer supported. Import/export functionality is now provided by Config Management.
* Configuration page paths are different.
* Automated tests have not yet been updated for Backdrop.
* Drush support is not yet updated for Backdrop.

Issues
------

Bugs and feature requests should be reported in [the Issue Queue](https://github.com/backdrop-contrib/http_response_headers/issues).

Current Maintainers
-------------------

- [Robert J. Lang (bugfolder)](https://github.com/bugfolder)

Credits
-------

- Ported to Backdrop CMS by [Robert J. Lang (bugfolder)](https://github.com/bugfolder).
- Drupal 7 version by [Vijaya Chandran Mani (vijaycs85)](https://www.drupal.org/u/vijaycs85)
- Drupal maintainers:
    * [Vijaya Chandran Mani (vijaycs85)](https://www.drupal.org/u/vijaycs85)
    * [Minnur Yunusov (minnur)](https://www.drupal.org/u/minnur)
    * [Drew Webber (mcdruid)](https://www.drupal.org/u/mcdruid)
    * [Malachy McConnell (mmcconnell)](https://www.drupal.org/u/mmcconnell)
- Drupal supporting organization:
     * [Eurostar](https://www.drupal.org/eurostar)

License
-------

This project is GPL v2 software.
See the LICENSE.txt file in this directory for complete text.

