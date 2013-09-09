<?php
/**
 * @file
 */

/**
 * Base File URI for handling media folder destination lookup.
 *
 * When used, the concrete migration class must implement the interface
 * ImagexMigrateMediaFolderDestinationInterface.
 * 
 * @author Aaron McGowan <aaron@imagexmedia.com>
 * @package Imagex.Migration.Media
 * @version 1.0.0
 */
abstract class ImagexMigrateMediaFolderFileUri extends ImagexMigrateFileUri {
  /**
   * @var string
   *   The base destination directory within Drupal.
   */
  protected $destinationDir = NULL;

  public function __construct($arguments = array(), $default_file = NULL){
    $this->destinationDir = file_default_scheme() . '://';
    parent::__construct($arguments, $default_file);
  }

  /**
   * {@inheritDoc}
   */
  protected function initDefaultFileInfo() {
    parent::initDefaultFileInfo();

    // Attempt to resolve the destination directory if the media folder
    // destination interface is implemented, additionally resolve the path and
    // assign the Drupal stream path to the `destinationDir` property.
    if ($this->getMigration() instanceof ImagexMigrateMediaFolderDestinationInterface) {
      $media_folder_name = $this->getMigration()->getMediaFolder();
      if (!empty($media_folder_name) && ($media_folder_vocab = taxonomy_vocabulary_machine_name_load('media_folders'))) {
        $media_folder_term = taxonomy_get_term_by_name($media_folder_name, $media_folder_vocab->machine_name);
        if (NULL !== $media_folder_term) {
          $media_folder_term = array_shift($media_folder_term);
          $media_folder_path = media_browser_plus_construct_dir_path($media_folder_term);
          $this->destinationDir = file_default_scheme() . '://' . $media_folder_path;
        }
      }
    }
  }
}
