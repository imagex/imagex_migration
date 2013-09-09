<?php
/**
 * @file
 */

/**
 * Base SQL source migrate class that provides a wrapper to MigrateSourceSQL.
 *
 * @author Aaron McGowan <aaron@imagexmedia.com>
 * @package Imagex.Migration
 * @version 1.0.0
 */
abstract class ImagexMigrateSourceSql extends MigrateSourceSQL {
  /**
   * @var ImagexMigrationBase
   *   The instance of the ImagexMigration in which this source is used in.
   */
  private $migration = NULL;

  /**
   * @var Database_mysql
   */
  private $connection = NULL;

  /**
   * @var array
   *   An array of arguments/options passed at time of instantiation.
   */
  private $options = array();

  /**
   * Creates a new instance of the SQL migrate.
   *
   * @access public
   * @param ImagexMigrationBase $migration
   *   The migration object instance that is using this migrate source type.
   * @param array $options
   *   An array of options that are specified and used for this migrate.
   */
  final public function __construct(ImagexMigrationBase $migration, array $options = array()) {
    $this->migration = $migration;
    $this->options = $options;

    if (isset($options['map_joinable'])) {
      $this->mapJoinable = $options['map_joinable'];
    }
  }

  /**
   * Performs initialization of the migrate source.
   *
   * This is method is invoked prior to the actual query being used. This allows
   * for less overhead at time of instantiation in the event that this migrate
   * source is never really used or queried.
   *
   * @access protected
   */
  protected function init() {
    $this->initSourceConnection();
    $this->originalQuery = $this->getSourceQuery();
    $this->query = clone $this->originalQuery;
    $this->countQuery = clone $this->originalQuery->countQuery();
  }

  /**
   * Returns the source connection object.
   *
   * @access protected
   * @return Database_driver
   *   Returns the database driver instance for this migrate source.
   */
  final public function getSourceConnection() {
    $this->initSourceConnection();
    return $this->connection;
  }

  /**
   * Returns the ImagexMigration class instance.
   *
   * @access protected
   * @return ImagexMigration
   *   Returns the ImagexMigration class instance for this source.
   */
  final protected function getMigration() {
    $this->initSourceConnection();
    return $this->migration;
  }

  /**
   * Returns the source query object required for this migrate.
   * 
   * @example
   *    return $this->getSourceConnection()->select('nodes', 'n');
   *
   * @access protected
   * @return SelectQueryInterface
   *   Returns the select query for retrieving the data at the source.
   */
  abstract protected function getSourceQuery();

  /**
   * Dynamically creates the database connection object if needed.
   *
   * @access private
   * @throws ImagexException If source connection details do not exist in Migration arguments.
   */
  final private function initSourceConnection() {
    if (NULL !== $this->connection) {
      return;
    }

    $arguments = $this->migration->getArguments();
    if (!isset($arguments['source']['connection']['name']) || !isset($arguments['source']['connection']['target'])) {
      throw new ImagexException(t('Source SQL connection details do not exist in Migration arguments.'));
    }

    $this->connection = ImagexMigrationDatabase::getMigrationConnection(
      $arguments['source']['connection']['name'],
      $arguments['source']['connection']['target']
    );
  }

  /**
   * {@inheritDoc}
   */
  final public function performRewind() {
    parent::performRewind();
  }

  /**
   * {@inheritDoc}
   */
  final public function rewind() {
    $this->init();
    parent::rewind();
  }

  /**
   * {@inheritDoc}
   */
  final public function computeCount() {
    $this->init();
    return parent::computeCount();
  }

  public function fields() {
    $this->init();
    return parent::fields();
  }
}