<?php
/**
 * @file
 */

/**
 * Non-instantiatable base class for all SQL migration oriented instances.
 *
 * Features include providing the default arguments with source and destination
 * connection details, resulting in less code to maintain throughout the
 * building of SQL migrations.
 * 
 * @author Aaron McGowand <aaron@imagexmedia.com>
 * @package Imagex.Migration
 * @version 1.0.0
 */
abstract class ImagexMigrationSqlMigration extends ImagexMigrationBase {
  /**
   * Returns default arguments and values including connection information base.
   *
   * @access public
   * @return array
   *   Returns an associative array containing default arguments.
   */
  public function getDefaultArguments() {
    return array_merge(parent::getDefaultArguments(), array(
      'source' => array(
        'connection' => array(
          'name' => '',
          'target' => '',
        ),
      ),
    ));
  }
}
