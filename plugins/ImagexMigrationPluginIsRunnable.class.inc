<?php
/**
 * @file
 */

/**
 * Allows for set time intervals to be used to determine if the migration is runnable.
 *
 * @author Aaron McGowan <aaron@imagexmedia.com.
 * @package Imagex.Migration.Plugins
 * @version 1.0.0
 */
class ImagexMigrationPluginIsRunnable extends ImagexMigrationPluginBase {
  /**
   * @var int The number of seconds that must elapse before running again.
   */
  private $interval = 0;

  /**
   * Creates a new instance of the ImagexMigrationPluginIsRunnable class.
   *
   * @access public
   * @param int $seconds
   *   The time interval to determine whether or not the migration is runnable.
   * @param ImagexMigrationBase $migration
   *   The migration object that this plugin is attached to.
   */
  public function __construct($seconds = 0, ImagexMigrationBase $migration = NULL) {
    parent::__construct($migration);
    $this->setInterval($seconds);
  }

  /**
   * Primary event callback for all ImagexMigration events.
   *
   * @access public
   * @param string $event
   *   The event identifier (eg. ImagexMigration::PREIMPORT).
   * @param array $args
   *   An array of arguments to be passed at time of event.
   */
  public function execute($event, array $args = array()) {
    switch ($event) {
      // Use of INITIALIZE event as the migrate's module checks the enabled
      // state of the migration object while building a list of migrations to
      // run, etc. This occurs at time of instantiation.
      case ImagexMigration::INITIALIZE:
        if (!$this->isMigrationRunnable()) {
          $this->getMigration()->setEnabled(FALSE);
        } else {
          $this->getMigration()->setEnabled(TRUE);
        }
        break;
      // Once the import is complete, update the last ran timestamp variable.
      case ImagexMigration::POSTIMPORT:
        $this->updateLastRunTimestamp();
        break;
    }
  }

  /**
   * Sets the number of seconds that must elapse between runs.
   *
   * @access public
   * @param int $seconds
   *   The number of seconds (eg. one day is equal to 86400).
   * @param ImagexMigrationPluginInterface
   *   Returns this plugin instance.
   */
  public function setInterval($seconds) {
    if (0 > $seconds) {
      throw new ImagexException(t('The seconds parameter must be greater then zero.'));
    }

    $this->interval = $seconds;
    return $this;
  }

  /**
   * Returns a boolean indicating whether the migration is runnable.
   *
   * @access public
   * @return bool
   *   Returns TRUE if the migration is runnable, otherwise FALSE.
   */
  public function isMigrationRunnable() {
    $allowed = (REQUEST_TIME <= ($this->getLastMigrationRun() + $this->interval)) ? FALSE : TRUE;
    if (!$allowed && $this->getMigration()->getStatus() !== MigrationBase::STATUS_DISABLED) {
      switch ($this->getMigration()->getStatus()) {
        case MigrationBase::STATUS_IMPORTING:
        case MigrationBase::STATUS_ROLLING_BACK:
        case MigrationBase::STATUS_STOPPING:
          $allowed = TRUE;
      }
    }

    return $allowed ;
  }

  /**
   * Returns the timestamp of the last migration run.
   *
   * The timestamp value for the last run is stored in the Drupal variables
   * table with the name
   * `Imagex_migration__migration_{migration_machine_name}_last_run`.
   *
   * @access public
   * @return int
   *   Returns the timestamp of the last run.
   */
  public function getLastMigrationRun() {
    return variable_get($this->getMigrationLastRunVariableName(), 0);
  }

  /**
   * Updates the last run timestamp in the var table with the current time.
   *
   * @access public
   */
  public function updateLastRunTimestamp() {
    variable_set($this->getMigrationLastRunVariableName(), REQUEST_TIME);
  }

  /**
   * Returns a the variable name for this migration's last run variable.
   *
   * @access protected
   * @return string
   *   Returns the variable name for last run timestamp.
   */
  protected function getMigrationLastRunVariableName() {
    return 'Imagex_migration__migration_' . drupal_strtolower($this->getMigration()->getMachineName()) . '_last_run';
  }
}
