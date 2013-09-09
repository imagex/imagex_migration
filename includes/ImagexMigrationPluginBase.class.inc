<?php
/**
 * @file
 */

/**
 * Base migration plugin class that allows for re-usable trait like behavior.
 *
 * @author Aaron McGowan <aaron@imagexmedia.com>
 * @package Imagex.Migration
 * @version 1.0.0
 */
abstract class ImagexMigrationPluginBase implements ImagexMigrationPluginInterface {
  /**
   * @var ImagexMigration
   *   Contains the migration object instance, default value NULL.
   */
  private $migration = NULL;

  /**
   * Creates a new OpenMigrationPluginInterface instance.
   *
   * @access public
   * @param ImagexMigrationBase $migration
   *   The migration object instance using this plugin instance.
   */
  public function __construct(ImagexMigrationBase $migration = NULL) {
    if ($migration) {
      $this->setMigration($migration);
    }
  }

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
  final public function setMigration(ImagexMigrationBase $migration) {
    $this->migration = $migration;
    return $this;
  }

  /**
   * Returns the migration object instance using this plugin instance.
   *
   * @access public
   * @return ImagexMigrationBase
   *   Returns the set migration object instance.
   */
  final public function getMigration() {
    return $this->migration;
  }
}
