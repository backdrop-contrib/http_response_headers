<?php
/**
 * @file
 * Admin page callbacks for the HTTP response headers module.
 */

/**
 * Callback for configuration form.
 */
function http_response_headers_settings_form($form, &$from_state) {
  $config = config('http_response_headers.settings');
  $form['#config'] = 'http_response_headers.settings';

  $form['label'] = array(
    '#type' => 'markup',
    '#markup' => theme('item_list', array(
      'title' => t('Currently enabled headers'),
      'items' => $config->get('allowed_headers'),
    )),
  );

  if (!module_exists('http_response_headers_ui')) {
    $form['info'] = array(
      '#markup' => t('To add, remove, or edit headers, enable the HTTP Response Headers UI module.'),
      '#prefix' => '<p>',
      '#suffix' => '</p>',
    );
  }

  $form['allowed_headers'] = array(
    '#type' => 'select',
    '#title' => t('Allowed headers'),
    '#default_value' => $config->get('allowed_headers'),
    '#options' => http_response_headers_get_all_headers(),
    '#multiple' => TRUE,
    '#size' => 15,
  );
  $form['additional_headers'] = array(
    '#type' => 'textarea',
    '#title' => t('Additional headers'),
    '#description' => t('Additional HTTP headers, one per line.'),
    '#rows' => '5',
    '#default_value' => http_response_headers_get_additional_headers(),
  );
  $form['exclude_pages'] = array(
    '#type' => 'textarea',
    '#title' => t('Exclude header rule pages'),
    '#description' => t('Excludes header rule check on these pages.'),
    '#rows' => '5',
    '#default_value' => $config->get('exclude_pages'),
  );
  $form['#submit'][] = 'http_response_headers_additional_headers_submit';

  return system_settings_form($form);
}

/**
 * Submit handler for additional_headers.
 */
function http_response_headers_additional_headers_submit($form, &$form_state) {
  $headers = &$form_state['values']['additional_headers'];
  $headers = explode("\n", $headers);
  $headers = array_map('trim', $headers);
  $headers = array_filter($headers);
}

/**
 * Provides any additional headers.
 *
 * @return string
 *   List of headers separated by newlines.
 */
function http_response_headers_get_additional_headers() {
  $headers = config_get('http_response_headers.settings', 'additional_headers');
  if (!is_array($headers)) {
    $headers = array($headers);
  }
  $headers = trim(implode("\n", $headers));
  return $headers;
}

/**
 * Provides response headers.
 *
 * @return array
 *   An array of response headers.
 */
function http_response_headers_get_all_headers() {

  // Standard response headers.
  $headers['Standard response headers'] = array(
    'Access-Control-Allow-Origin',
    'Accept-Ranges',
    'Age',
    'Allow',
    'Cache-Control',
    'Connection',
    'Content-Encoding',
    'Content-Language',
    'Content-Length',
    'Content-Location',
    'Content-MD5',
    'Content-Disposition',
    'Content-Range',
    'Content-Type',
    'Date',
    'ETag',
    'Expires',
    'Last-Modified',
    'Link',
    'Location',
    'P3P',
    'Pragma',
    'Proxy-Authenticate',
    'Refresh',
    'Retry-After',
    'Server',
    'Set-Cookie',
    'Status',
    'Strict-Transport-Security',
    'Trailer',
    'Transfer-Encoding',
    'Vary',
    'Via',
    'Warning',
    'WWW-Authenticate',
  );

  // Non-standard response headers.
  $headers['Common non-standard response headers'] = array(
    'X-Frame-Options',
    'X-XSS-Protection',
    'Content-Security-Policy',
    'X-Content-Security-Policy',
    'X-WebKit-CSP',
    'X-Content-Type-Options',
    'X-Powered-By',
    'X-UA-Compatible',
    'X-Permitted-Cross-Domain-Policies',
    'Surrogate-Control',
  );

  $additional_headers = config_get('http_response_headers.settings', 'additional_headers');
  if (count($additional_headers)) {
    $headers['Additional headers'] = $additional_headers;
  }

  // Map key.
  foreach ($headers as $title => $header_set) {
    $return_list[$title] = array_combine($header_set, $header_set);
  }

  return $return_list;
}
