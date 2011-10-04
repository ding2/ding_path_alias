<?php

/**
 * @file
 * Integration with pathauto alias generation
 */

/**
 * Batch processing callback; Generate aliases for products..
 */
function ding_path_alias_library_news_page_pathauto_bulk_update_batch_process(&$context) {
  if (!isset($context['sandbox']['current'])) {
    $context['sandbox']['count'] = 0;
    $context['sandbox']['current'] = 0;
  }

  $query = db_select('node', 'n');
  $query->leftJoin('og', 'o', 'o.etid = n.nid');
  $query->leftJoin('url_alias', 'ua', "CONCAT(CONCAT('library/', o.gid), '/news') = ua.source");
  $query->addField('o', 'gid');
  $query->isNull('ua.source');
  $query->condition('o.gid', $context['sandbox']['current'], '>');
  $query->condition('o.entity_type', 'node', '=');
  $query->condition('n.type', 'ding_library', '=');
  $query->orderBy('o.gid');
  $query->addTag('pathauto_bulk_update');

  // Get the total amount of items to process.
  if (!isset($context['sandbox']['total'])) {
    $context['sandbox']['total'] = $query->countQuery()->execute()->fetchField();

    // If there are no nodes to update, the stop immediately.
    if (!$context['sandbox']['total']) {
      $context['finished'] = 1;
      return;
    }
  }

  $query->range(0, 25);
  $gids = $query->execute()->fetchCol();
  ding_path_alias_library_news_page_pathauto_bulk_update_alias_multiple($gids, 'bulkupdate');

  $context['sandbox']['count'] += count($gids);
  $context['sandbox']['current'] = max($gids);
  $context['message'] = t('Updated alias for news page for library node with gid @gid.', array('@gid' => end($gids)));

  if ($context['sandbox']['count'] != $context['sandbox']['total']) {
    $context['finished'] = $context['sandbox']['count'] / $context['sandbox']['total'];
  }
  elseif (!empty($gids)) {
    $context['finished'] = 1;
  }
}

/**
 * Update the URL aliases for multiple library news pages.
 *
 * @param $gids
 *   An array of group IDs.
 * @param $op
 *   Operation being performed on the nodes ('insert', 'update' or
 *   'bulkupdate').
 * @param $options
 *   An optional array of additional options.
 */
function ding_path_alias_library_news_page_pathauto_bulk_update_alias_multiple($gids, $op, array $options = array()) {
  $options += array('message' => FALSE);
  
  $groups = og_load_multiple($gids);
  foreach ($groups as $group) {
    ding_path_alias_library_news_page_pathauto_update_alias($group, $op, $options);
  }

  if (!empty($options['message'])) {
    drupal_set_message(format_plural(count($gids), 'Updated URL alias for 1 library news page.', 'Updated URL aliases for @count library news pages.'));
  }
}

/**
 * Update the URL aliases for an individual node.
 *
 * @param $group
 *   A fully loaded node.
 * @param $op
 *   Operation being performed on the node ('insert', 'update' or 'bulkupdate').
 * @param $options
 *   An optional array of additional options.
 */
function ding_path_alias_library_news_page_pathauto_update_alias(OgGroup $group, $op, array $options = array()) {
  module_load_include('inc', 'pathauto');
  $node = node_load($group->etid);
  if ($node) {
    $path = 'library/' . $group->gid . '/news';
    pathauto_create_alias('ding_path_alias_library_news_page', $op, $path, array('node' => $node));
  }
}

/**
 * Batch processing callback; Generate aliases for products..
 */
function ding_path_alias_library_event_page_pathauto_bulk_update_batch_process(&$context) {
  if (!isset($context['sandbox']['current'])) {
    $context['sandbox']['count'] = 0;
    $context['sandbox']['current'] = 0;
  }

  $query = db_select('node', 'n');
  $query->leftJoin('og', 'o', 'o.etid = n.nid');
  $query->leftJoin('url_alias', 'ua', "CONCAT(CONCAT('library/', o.gid), '/events') = ua.source");
  $query->addField('o', 'gid');
  $query->isNull('ua.source');
  $query->condition('o.gid', $context['sandbox']['current'], '>');
  $query->condition('o.entity_type', 'node', '=');
  $query->condition('n.type', 'ding_library', '=');
  $query->orderBy('o.gid');
  $query->addTag('pathauto_bulk_update');

  // Get the total amount of items to process.
  if (!isset($context['sandbox']['total'])) {
    $context['sandbox']['total'] = $query->countQuery()->execute()->fetchField();

    // If there are no nodes to update, the stop immediately.
    if (!$context['sandbox']['total']) {
      $context['finished'] = 1;
      return;
    }
  }

  $query->range(0, 25);
  $nids = $query->execute()->fetchCol();
  ding_path_alias_library_event_page_pathauto_bulk_update_alias_multiple($nids, 'bulkupdate');

  $context['sandbox']['count'] += count($nids);
  $context['sandbox']['current'] = max($nids);
  $context['message'] = t('Updated alias for event page for library node with nid @nid.', array('@nid' => end($nids)));

  if ($context['sandbox']['count'] != $context['sandbox']['total']) {
    $context['finished'] = $context['sandbox']['count'] / $context['sandbox']['total'];
  }
  elseif (!empty($nids)) {
    $context['finished'] = 1;
  }
}

/**
 * Update the URL aliases for multiple library event pages.
 *
 * @param $nids
 *   An array of node IDs.
 * @param $op
 *   Operation being performed on the nodes ('insert', 'update' or
 *   'bulkupdate').
 * @param $options
 *   An optional array of additional options.
 */
function ding_path_alias_library_event_page_pathauto_bulk_update_alias_multiple($nids, $op, array $options = array()) {
  $options += array('message' => FALSE);
  
  $nodes = node_load_multiple($nids);
  foreach ($nodes as $node) {
    ding_path_alias_library_event_page_pathauto_update_alias($node, $op, $options);
  }

  if (!empty($options['message'])) {
    drupal_set_message(format_plural(count($nids), 'Updated URL alias for 1 library event page.', 'Updated URL aliases for @count library event pages.'));
  }
}

/**
 * Update the URL aliases for an individual node.
 *
 * @param $node
 *   A fully loaded node.
 * @param $op
 *   Operation being performed on the node ('insert', 'update' or 'bulkupdate').
 * @param $options
 *   An optional array of additional options.
 */
function ding_path_alias_library_event_page_pathauto_update_alias(stdClass $node, $op, array $options = array()) {
  module_load_include('inc', 'pathauto');
  $group = og_load_multiple(FALSE, array('entity_type' => 'node', 'etid' => $node->nid), FALSE);
  if ($group) {
    $group = reset($group);
    $path = 'library/' . $group->gid . '/events';
    pathauto_create_alias('ding_path_alias_library_event_page', $op, $path, array('node' => $node));
  }
}

