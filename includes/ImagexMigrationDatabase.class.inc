<?php
/**
 * @file
 */

/**
 *
 * @author Aaron McGowan <aaron@imagexmedia.com>
 * @package Imagex.Migration
 * @version 1.0.0
 */
abstract class ImagexMigrationDatabase extends ImagexDatabase {
  /**
   * @var array An array of database connection information for migrations.
   */
  private static $migrationConnectionInfo = array();

  /**
   * Returns a database connection object for a specific migration.
   *
   * @access public
   * @param string $name
   *   The name of the connection to use.
   * @param string $target
   *   The connection's target database retrieve.
   *
   * @return Database
   *   Returns a database object instance representing this connection.
   */
  final public static function getMigrationConnection($name, $target = 'default') {
    $key = self::createMigrationConnectionKey($name, $target);
    if (!isset(self::$connections[$key][$target])) {
      self::$connections[$key][$target] = self::openMigrationConnection($key, $target);
    }

    return self::$connections[$key][$target];
  }

  /**
   * Creates a machine name key for connections using the name and target info.
   *  
   * @access protected
   * @param string $name
   *   The name of the database connection.
   * @param string $target
   *   The connection's target name.
   *
   * @return string
   *   Returns a usable key to represent the database name and target.
   */
  final protected static function createMigrationConnectionKey($name, $target) {
    return drupal_strtolower(str_replace(array(' '), array('_'), (trim($name) . ' ' . trim($target))));
  }

  /**
   * Loads and parses all of the connection information related to migrations.
   *
   * The connection information is retrieved from the migration settings data
   * that is defined in an external file and loaded using the
   * Imagex_migration_settings function. The SQL connection settings should
   * be structured similarly to that of a Drupal site's settings.php database
   * connection information.
   * 
   * @access protected
   * @param boolean $reset
   *   A boolean value to indicate if the current info loaded should be reset.
   */
  final protected static function parseMigrationConnectionInfo($reset = FALSE) {
    if (!empty(self::$migrationConnectionInfo) && !$reset) {
      return;
    }

    $settings = Imagex_migration_settings();
    if (!isset($settings['source']['sql']['connections'])) {
      return;
    }

    $information = &$settings['source']['sql']['connections'];

    self::$migrationConnectionInfo = array();
    foreach ($information as $name => $values) {
      foreach ($information[$name] as $target => $target_values) {
        self::$migrationConnectionInfo[self::createMigrationConnectionKey($name, $target)][$target] = $target_values;
      }
    }

    self::$migrationConnectionInfo = self::parseConnectionInformation(self::$migrationConnectionInfo, TRUE);
  }

  /**
   * 
   * @access protected
   * @param string $key
   *   The database connection information key.
   * @param string $target
   *   The connection's target.
   *
   * @return Database
   */
  final protected static function openMigrationConnection($key, $target) {
    if (empty(self::$migrationConnectionInfo)) {
      self::parseMigrationConnectionInfo();
    }

    return self::openConnection($key, $target);
  }
}
