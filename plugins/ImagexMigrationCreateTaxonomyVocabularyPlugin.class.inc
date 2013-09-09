<?php
/**
 * @file
 */

/**
 * Allows for a taxonomy vocabulary to be created at initialization.
 *
 * @author Aaron McGowan <aaron@imagexmedia.com>
 * @package Imagex.Migration.Plugins
 * @version 1.0.0
 */
class ImagexMigrationCreateTaxonomyVocabularyPlugin extends ImagexMigrationPluginBase {
  /**
   * @var array An array of vocabulary information.
   */
  private $vocabulary = array();

  /**
   * Creates a new instance of the ImagexMigrationCreateTaxonomyVocabularyPlugin class.
   *
   * @access public
   * @param array $vocabulary
   *   An array of vocabulary information.
   * @param ImagexMigrationBase $migration
   *   The migration object that this plugin is attached too.
   */
  public function __construct(array $vocabulary = array(), ImagexMigrationBase $migration = NULL) {
    parent::__construct($migration);
    $this->vocabulary = $vocabulary;
  }

  /**
   * Primary event callback for the ImagexMigration events.
   *
   * @access public
   * @param string $event
   *   The event identifier (eg. ImagexMigration::PREIMPORT).
   * @param array $args
   *   An array of arguments to be passed at time of event.
   */
  public function execute($event, array $args = array()) {
    switch ($event) {
      case ImagexMigration::INITIALIZE:
        // If the vocabulary's machine_name is set, attempt to load the vocab.
        // If the vocabulary is not successfully loaded, therefore does not
        // exist, create a new vocabulary using the `$this->vocabulary` info.
        if (isset($this->vocabulary['machine_name'])) {
          $vocabulary = taxonomy_vocabulary_machine_name_load($this->vocabulary['machine_name']);
          if (!$vocabulary) {
            taxonomy_vocabulary_save((object) $this->vocabulary);
          }
        }
        break;
    }
  }
}
