<?php
/**
 * @file
 */

/**
 * Base interface for retrieving the media folder for files.
 *
 * This interface should be implemented by the Migration classes, the concrete
 * implementations of the ImagexMigration.
 *
 * @author Aaron McGowan <aaron@imagexmedia.com>
 * @package Imagex.Migration.Media
 * @version 1.0.0
 */
interface ImagexMigrateMediaFolderDestinationInterface {
  /**
   * Returns the media folder name that files should be categorized as.
   * 
   * @access public
   * @return string
   *   The term name of the media folder to categorize each item in.
   */
  public function getMediaFolder();
}