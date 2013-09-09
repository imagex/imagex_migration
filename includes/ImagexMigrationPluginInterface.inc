<?php
/**
 * @file
 */

/**
 * Defines the base behaviors of migration plugins.
 *
 * @author Aaron McGowan <aaron@imagexmedia.com>
 * @package Imagex.Migration
 * @version 1.0.0
 */
interface ImagexMigrationPluginInterface {
  /**
   * Sets the migration object instance.
   *
   * @access public
   * @param ImagexMigrationBase $migration
   *   The migration object instance using this plugin instance.
   *
   * @return ImagexMigrationPluginInterface
   *   Returns this plugin instance.
   */
  public function setMigration(ImagexMigrationBase $migration);

  /**
   * Returns the migration object instance using this plugin instance.
   *
   * @access public
   * @return ImagexMigrationBase
   *   Returns the set migration object instance.
   */
  public function getMigration();

  /**
   * Primary event callback for all ImagexMigration events.
   *
   * @access public
   * @param string $event
   *   The event identifier (eg. ImagexMigration::PREIMPORT).
   * @param array $args
   *   An array of arguments to be passed at time of event.
   */
  public function execute($event, array $args = array());
}
