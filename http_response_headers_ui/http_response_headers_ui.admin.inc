<?php
/**
 * @file
 * Administration pages and functions for the HTTP Response Headers module.
 */

/**
 * Menu callback for header rule list page.
 */
function http_response_headers_ui_list_form($form, &$form_state) {

  $rules = config_get('http_response_headers.settings', 'rules');

  $header = array(
    array('data' => t('Description')),
    array('data' => t('Header')),
    array('data' => t('Value')),
    array('data' => ''),
  );
  $rows = array();
  foreach ($rules as $machine_name => $rule) {
    $rows[] = array(
      array('data' => filter_xss($rule['description'])),
      array('data' => filter_xss($rule['header'])),
      array('data' => filter_xss($rule['header_value'])),
      array('data' => array(
        '#type' => 'dropbutton',
        '#links' => array(
          'edit' => array(
            'title' => t('Edit'),
            'href' => 'admin/config/system/http-response-headers/edit/' . $machine_name,
          ),
          'delete' => array(
            'title' => t('Delete'),
            'href' => 'admin/config/system/http-response-headers/delete/' . $machine_name,
          ),
        ),
      )),
    );
  }
  $form['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('There are no rules to display.'),
    '#attributes' => array('class' => array('http-response-headers-table')),
  );

  return $form;
}

/**
 * Menu callback for header rule add/edit page.
 */
function http_response_headers_ui_rule_form($form, &$form_state, $machine_name = '') {

  $rules = config_get('http_response_headers.settings', 'rules');
  if (empty($rules[$machine_name])) {
    $machine_name = '';
    $rule = NULL;
  }
  else {
    $rule = $rules[$machine_name];
  }

  $form['description'] = array(
    '#title' => t('Description'),
    '#type' => 'textfield',
    '#description' => t('The human-readable name of this rule. It is recommended that this name begin with a capital letter and contain only letters, numbers, and spaces. This name must be unique.'),
    '#default_value' => $machine_name ? $rule['description'] : '',
    '#disabled' => $machine_name ? TRUE : FALSE,
    '#required' => TRUE,
    '#size' => 30,
  );
  $form['machine_name'] = array(
    '#type' => 'machine_name',
    '#description' => t('A unique machine-readable name for this rule. It must only contain lowercase letters, numbers, and underscores.'),
    '#maxlength' => 32,
    '#machine_name' => array(
      'exists' => 'http_response_headers_ui_rule_exists',
      'source' => array('description'),
      'label' => t('Machine name'),
      'replace_pattern' => '[^a-z0-9_]+',
      'replace' => '_',
    ),
    '#disabled' => $machine_name ? TRUE : FALSE,
    '#default_value' => $machine_name,
  );
  $form['settings']['header'] = array(
    '#type' => 'select',
    '#title' => t('HTTP header'),
    '#description' => '',
    '#default_value' => $rule['header'],
    '#options' => config_get('http_response_headers.settings', 'allowed_headers'),
    '#empty_option' => t('Select a header'),
    '#required' => TRUE,
  );
  $form['settings']['header_value'] = array(
    '#type' => 'textarea',
    '#title' => t('HTTP header value'),
    '#description' => '',
    '#default_value' => $rule['header_value'],
    '#required' => TRUE,
  );

  // Visibility settings.
  $form['visibility_title'] = array(
    '#type' => 'item',
    '#title' => t('Visibility settings'),
  );
  $form['visibility'] = array(
    '#type' => 'vertical_tabs',
    '#attached' => array(
      'js' => array(backdrop_get_path('module', 'http_response_headers_ui') . '/js/http_response_headers_ui.js'),
    ),
  );

  // Per-path visibility.
  $form['visibility']['path'] = array(
    '#type' => 'fieldset',
    '#title' => t('Pages'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'visibility',
    '#weight' => 0,
  );

  $options = array(
    HTTP_RESPONSE_HEADERS_VISIBILITY_NOTLISTED => t('All pages except those listed'),
    HTTP_RESPONSE_HEADERS_VISIBILITY_LISTED => t('Only the listed pages'),
  );
  $description = t("Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array(
    '%blog' => 'blog',
    '%blog-wildcard' => 'blog/*',
    '%front' => '<front>',
  ));

  $form['visibility']['path']['visibility'] = array(
    '#type' => 'radios',
    '#title' => t('Set header on specific pages'),
    '#options' => $options,
    '#default_value' => $rule['visibility'],
  );

  $form['visibility']['path']['pages'] = array(
    '#type' => 'textarea',
    '#title' => '<span class="element-invisible">' . t('Pages') . '</span>',
    '#description' => $description,
    '#default_value' => $rule['pages'],
  );

  // Per-node-type visibility.
  $default_type_options = array();
  $types = $rule['types'];
  if (!empty($types)) {
    $default_type_options = explode(',', $types);
  }
  $form['visibility']['node_type'] = array(
    '#type' => 'fieldset',
    '#title' => t('Content types'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'visibility',
    '#weight' => 5,
  );
  $form['visibility']['node_type']['types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Set header for specific content types'),
    '#default_value' => $default_type_options,
    '#options' => node_type_get_names(),
    '#description' => t('Set this header only on pages that display content of the given type(s). If you select no types, there will be no type-specific limitation.'),
  );

  // Per-role visibility.
  $default_role_options = array();
  $roles = $rule['roles'];
  if (!empty($roles)) {
    $default_role_options = explode(',', $roles);
  }
  $role_options = array_map('check_plain', user_roles());
  $form['visibility']['role'] = array(
    '#type' => 'fieldset',
    '#title' => t('Roles'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'visibility',
    '#weight' => 10,
  );
  $form['visibility']['role']['roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Set header for specific roles'),
    '#default_value' => $default_role_options,
    '#options' => $role_options,
    '#description' => t('Set this header only for the selected role(s). If you select no roles, the rule will be visible to all users.'),
  );

  $form['actions'] = array(
    '#type' => 'actions',
    'save' => array(
      '#type' => 'submit',
      '#value' => t('Save rule'),
      '#submit' => array('http_response_headers_ui_rule_form_submit'),
    ),
    'delete' => array(
      '#type' => 'submit',
      '#value' => t('Delete rule'),
      '#submit' => array('http_response_headers_ui_rule_form_delete'),
    ),
    'cancel' => array(
      '#type' => 'link',
      '#title' => t('Cancel'),
      '#href' => 'admin/config/system/http-response-headers/list',
    ),
  );

  return $form;
}

/**
 * Submission function for the save button on the header rule add/edit form.
 */
function http_response_headers_ui_rule_form_submit($form, &$form_state) {
  $rules = config_get('http_response_headers.settings', 'rules');
  $rules[$form_state['values']['machine_name']] = array(
    'description' => $form_state['values']['description'],
    'header' => $form_state['values']['header'],
    'header_value' => $form_state['values']['header_value'],
    'visibility' => $form_state['values']['header'],
    'pages' => $form_state['values']['pages'],
    'types' => implode(',', array_filter($form_state['values']['types'])),
    'roles' => implode(',', array_filter($form_state['values']['roles'])),
  );
  config_set('http_response_headers.settings', 'rules', $rules);
  http_response_headers_cache_reset();
  backdrop_set_message(t('Rule %description has been saved.', array('%description' => $form_state['values']['description'])));
  $form_state['redirect'] = 'admin/config/system/http-response-headers/list';
}

/**
 * Submission function for the delete button on the header rule add/edit form.
 */
function http_response_headers_ui_rule_form_delete($form, &$form_state) {
  $machine_name =  $form_state['values']['machine_name'];
  $form_state['redirect'] = 'admin/config/system/http-response-headers/delete/' . $machine_name;
}

/**
 * Confirmation page for deleting a header rule.
 */
function http_response_headers_ui_rule_delete_confirm_form($form, &$form_state, $machine_name) {
  $rules = config_get('http_response_headers.settings', 'rules');
  if (isset($rules[$machine_name])) {
    $description = $rules[$machine_name]['description'];
    return confirm_form(array(
        'machine_name' => array(
          '#type' => 'value',
          '#value' => $machine_name,
        ),
      ),
      t('Are you sure you want to delete the rule %rule?', array('%rule' => $description)),
      'admin/config/system/http-response-headers/list');
  }
  else {
    backdrop_set_message(t('There is no rule with machine name %machine_name.', array('%machine_name' => $machine_name)), 'error');
    backdrop_goto('admin/config/system/http-response-headers/list');
  }
}

/**
 * Submission handler for confirmation page for deleting a header rule.
 */
function http_response_headers_ui_rule_delete_confirm_form_submit($form, &$form_state) {
  $machine_name = $form_state['values']['machine_name'];
  $rules = config_get('http_response_headers.settings', 'rules');
  $description = $rules[$machine_name]['description'];
  unset($rules[$machine_name]);
  config_set('http_response_headers.settings', 'rules', $rules);
  http_response_headers_cache_reset();
  backdrop_set_message(t('Rule %description has been deleted.', array('%description' => $description)));
  $form_state['redirect'] = 'admin/config/system/http-response-headers/list';
}

/**
 * Returns whether a rule with given machine name exists.
 *
 * @see http_response_headers_ui_rule_form()
 */
function http_response_headers_ui_rule_exists($machine_name) {
  $rules = config_get('http_response_headers.settings', 'rules');
  return isset($rules[$machine_name]);
}
