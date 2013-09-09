<?php
/**
 * @file
 */

/**
 * Base class for handling file migrations by their URI.
 * 
 * @author Aaron McGowand <aaron@imagexmedia.com>
 * @package Imagex.Migration
 * @version 1.0.0
 */
class ImagexMigrateFileUri extends MigrateFileUri {
  /**
   * @var string
   *   The default file path if it is being used (determined by static::getUseDefaultFile()).
   */
  private $default_file_path = NULL;

  /**
   * @var boolean
   *   A boolean value used to indicate whether or not a default should be copied.
   */
  private $use_default_file = FALSE;

  /**
   * Returns the current migration object instance.
   *
   * @access public
   * @return MigrationBase
   *   Returns the current migration instance.
   */
  final public function getMigration() {
    return Migration::currentMigration();
  }

  /**
   * Implementation of MigrateFileInterface::processFile().
   *
   * @access public
   * @param string $value
   *   The URI or local filespec of a file to be imported.
   * @param int $owner
   *   The User ID (uid) to be the owner.
   * 
   * @return object
   *   The file entity being created or referenced, FALSE on failure.
   */
  public function processFile($value, $owner) {
    // Call our initializer for the default file information.
    $this->initDefaultFileInfo();

    // Continue with regular file processing.
    return parent::processFile($value, $owner);
  }

  /**
   * Implementation of MigrateFile::copyFile().
   *
   * @access protected
   * @param string $destination
   *   The destination path within Drupal.
   * @return bool
   *   Returns TRUE if the file was successfully copied, otherwise FALSE.
   */
  protected function copyFile($destination) {
    $this->sourcePath = static::urlencode($this->sourcePath);

    $copied = FALSE;
    if (file_exists($this->sourcePath)) {
      if (file_unmanaged_copy($this->sourcePath, $destination, FILE_EXISTS_RENAME)) {
        $copied = TRUE;
      }
    }

    if (!$copied && $this->use_default_file && !empty($this->default_file_path) && is_file($this->default_file_path)) {
      if (!file_unmanaged_copy($this->default_file_path, $destination, FILE_EXISTS_RENAME)) {
        $migration = Migration::currentMigration();
        $migration->saveMessage(t('The specified default file, %file, to use when the source file does not exist could not be copied.', array(
          '%file' => $this->default_file_path
        )));
      }
      else {
        $copied = TRUE;
      }
    }

    return $copied;
  }

  /**
   * Initializes the default file properties for this instance.
   *
   * @access protected
   */
  protected function initDefaultFileInfo() {
    // Determine if default files are being used.
    $this->use_default_file = static::getUseDefaultFile();
    if ($this->use_default_file) {
      // Set the default file path if default files are being used.
      $this->default_file_path = static::getDefaultFilePath();
    }
  }

  /**
   * Returns a boolean indicating whether or not a default files should be used.
   *
   * @access protected
   * @return bool
   *   Returns FALSE by default, method should be overridden.
   */
  protected static function getUseDefaultFile() {
    return FALSE;
  }

  /**
   * Returns an absolute file path string to use as a default file.
   *
   * @access protected
   * @return string
   *   Returns NULL by default, method should be overridden.
   */
  protected static function getDefaultFilePath() {
    return NULL;
  }
}