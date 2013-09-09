<?php
/**
 * @file
 */

/**
 * Abstract base class for creating re-usable & maintainable migration objects.
 *
 * @author Aaron McGowan <aaron@imagexmedia.com>
 * @package Imagex.Migration
 * @version 1.0.0 
 */
abstract class ImagexMigrationBase extends Migration {
  /**
   * @var array
   *   An array of arguments passed during instantiation.
   */
  protected $arguments = array();

  /**
   * @var array
   *   An array of plugins to be used and ran during running of this migraiton.
   */
  private $plugins = array();

  /**
   * Creates an instance of the SoeMigration class.
   *
   * @access public
   * @param array $arguments
   *   An associative array of arguments specified in hook_migrate_api().
   */
  public function __construct(array $arguments = array()) {
    if (!isset($arguments['group'])) {
      // $arguments['group'] = self::getGroupInstance();
    }

    parent::__construct($arguments);

    // Initialize the migration's arguments.
    $arguments = array_merge($this->getDefaultArguments(), $arguments);
    $this->setArguments($arguments);

    // Set the description of this migration, if it exits in arguments.
    if (isset($this->arguments['description'])) {
      $this->setDescription($this->arguments['description']);
    }

    // Perform remaining initialization.
    $this->init();
  }

  /**
   * Handles the initializing of the migration instance.
   *
   * @access protected
   */
  protected function init() {
    // Method should be overridden so that derived implementations perform
    // any required initializing within, instead of within the constructor.
  }

  /**
   * Sets the description of this migration.
   *
   * @access public
   * @param string $description
   *   The description text to 
   */
  final public function setDescription($description) {
    parent::setDescription($description);
    return $this;
  }

  /**
   * Sets the migration source object instance.
   *
   * @access protected
   */
  final public function setSource(MigrateSource $source) {
    parent::setSource($source);
    return $this;
  }

  /**
   * Returns the source object instance.
   *
   * If the source object is not yet assigned, the assignment will occur 
   * prior to the source object being returned.
   *
   * @access public
   * @return object
   *   Returns the source object instance.
   */
  final public function getSource() {
    if (!($source = parent::getSource())) {
      parent::setSource($this->getSourceObject());
    }

    return parent::getSource();
  }

  /**
   * Sets the destination for this migration.
   *
   * @access public
   * @param string $destination
   *   The destination object to set.
   */
  final public function setDestination(MigrateDestination $destination) {
    parent::setDestination($destination);
    return $this;
  }

  /**
   * Returns the destination object instance.
   *
   * If the destination object is not yet assigned, the assignment will occur 
   * prior to the destination object being returned.
   *
   * @access public
   * @return object
   *   Returns the destination object instance.
   */
  final public function getDestination() {
    if (!($destination = parent::getDestination())) {
      $this->setDestination($this->getDestinationObject());
    }

    return parent::getDestination();
  }

  /**
   * Sets the map for this migration.
   *
   * @access protected
   * @param MigrateMap $map
   *   The migrate map object to set.
   */
  final public function setMap(MigrateMap $map) {
    parent::setMap($map);
    return $this;
  }

  /**
   * Sets the dependencies for this migration.
   *
   * @access protected
   * @param array $dependencies
   *   An array of migration machine names this migration depends on.
   */
  final protected function setDependencies(array $dependencies) {
    $this->setHardDependencies($dependencies);
    return $this;
  }

  /**
   * Sets the "soft" dependencies for this migration.
   *
   * @access protected
   * @param array $dependencies
   *   An array of migration machine names this migration depends on.
   */
  /* final protected function setSoftDependencies(array $dependencies) {
    $this->softDependencies = $dependencies;
    return $this;
  } */

  /**
   * Sets the enabled flag which is determined whether or not this migration is enabled.
   * 
   * @access public
   * @param bool $enabled
   *   A boolean value to set.
   */
  final public function setEnabled($enabled) {
    $this->enabled = (bool) $enabled;
    return $this;
  }

  /**
   * Returns an array of arguments passed to the migration at instantiation.
   *
   * @access public
   * @return array
   *   Returns an associative array of arguments.
   */
  final public function getArguments() {
    return $this->arguments;
  }

  /**
   * Returns the value of a single argument $name.
   *
   * @access public
   * @param string $name
   *   The name of the argument to retrieve.
   *
   * @return mixed
   *   Returns the argument's value if it exists, otherwise NULL.
   */
  final public function getArgument($name) {
    return isset($this->arguments[$name]) ? $this->arguments[$name] : NULL;
  }

  /**
   * Sets all arguments for this migration instance.
   *
   * @access public
   * @param array $arguments
   *  An array of arguments to set to this migration.
   */
  final public function setArguments(array $arguments) {
    $this->arguments = $arguments;
    return $this;
  }

  /**
   * Sets the value for an individual argument.
   *
   * @access protected
   * @param string $name
   *   The name of the argument to set its value.
   * @param mixed $value
   *   The value for the argument named $name.
   *
   * @return ImagexMigration
   *   Returns this instance of the ImagexMigration class.
   */
  final public function setArgument($name, $value) {
    $this->arguments[$name] = $value;
    return $this;
  }

  /**
   * Returns a boolean to indicate whether or not an argument exists.
   *
   * @access public
   * @param string $name
   *   The name of the argument to check if it exists.
   *
   * @return bool
   *   Returns TRUE if the argument exists, otherwise false.
   */
  final public function hasArgument($name) {
    return isset($this->arguments[$name]);
  }

  /**
   * Returns an array of default arguments and values.
   *
   * @access protected
   * @return array
   *   Returns an associative array of default arguments.
   */
  public function getDefaultArguments() {
    return array();
  }

  /**
   * Adds a plugin to the current migration's plugin stack.
   *
   * Upon a successful attachment, the plugin's migration object is set in
   * addition to the ImagexMigration::INITIALIZE event is fired.
   *
   * @access public
   * @param string $name
   *   A name used to identify this plugin with this migration.
   * @param ImagexMigrationPluginInterface $plugin
   *   The actual migration plugin.
   * 
   * @return bool
   *   Returns TRUE if the plugin was added, otherwise FALSE.
   */
  final public function addPlugin($name, ImagexMigrationPluginInterface $plugin) {
    if (isset($this->plugins[$name])) {
      return FALSE;
    }

    $this->plugins[$name] = $plugin;
    $this->plugins[$name]->setMigration($this);
    $this->plugins[$name]->execute(ImagexMigration::INITIALIZE);

    return TRUE;
  }

  /**
   * Removes a plugin given its name.
   *
   * @access public
   * @param string $name
   *   The name of the plugin to remove.
   *
   * @return bool
   *   Returns TRUE if the plugin exists and was removed, otherwise false.
   */
  final public function removePlugin($name) {
    if (isset($this->plugins[$name])) {
      $this->plugins[$name]->setMigration(NULL);
      unset($this->plugins[$name]);
      return TRUE;
    }

    return FALSE;
  }

  /**
   * Returns a single plugin instance by its given name.
   *
   * @access public
   * @param string $name
   *   The name of the plugin to retrieve.
   * 
   * @return ImagexMigrationPluginInterface
   *   Returns the plugin instance if it exists, otherwise NULL.
   */
  final public function getPlugin($name) {
    return isset($this->plugins[$name]) ? $this->plugins[$name] : NULL;
  }

  /**
   * Returns an associative array of all plugins for this migration.
   *
   * @access public
   * @return array
   *   Returns all the plugins attached to this instance.
   */
  final public function getPlugins() {
    return $this->plugins;
  }

  /**
   * Returns a boolean to indicate whether or not a plugin exists.
   *
   * @access public
   * @param string $name
   *   The name of the plugin to check if it exists.
   *
   * @return bool
   *   Returns TRUE if a plugin exists with $name, otherwise returns FALSE.
   */
  final public function hasPlugin($name) {
    return isset($this->plugins[$name]);
  }

  /**
   * Executes all plugins attached to this migration.
   *
   * @access protected
   * @param string $event
   *   The name of the event that is being fired.
   * @param array $args
   *   An array of arguments to be passed to the plugin upon execution.
   */
  final protected function executePlugins($event, array $args = array()) {
    if (!in_array($event, ImagexMigration::getArrayOfEvents())) {
      return;
    }

    foreach ($this->plugins as $plugin_name => $plugin) {
      $plugin->execute($event, $args);
    }
  }

  /**
   * Returns the migration group instance.
   *
   * @access public
   * @return MigrateGroup
   *   Returns the MigrateGroup instance for the Imagex Migration classes.
   */
  public static function getGroupInstance() {
    return MigrateGroup::getInstance('Imagex');
  }

  /**
   * Returns the MigrateMap instance for this migration.
   *
   * @access public
   * @return MigrateMap
   *   Returns the MigrateMap that is created in concrete getMapObject method.
   */
  public function getMap() {
    if (empty($this->map)) {
      $this->setMap($this->getMapObject());
    }

    return parent::getMap();
  }

  /**
   * Handles the initializing and setting of all field mappings.
   *
   * This method should be overridde by concrete class instances as this method
   * is invoked after the map has been set for field mappings to occur correctly
   * and at an efficient time.
   * 
   * @access protected
   */
  protected function setFieldMappings() {
    // Placeholder, empty.
  }

  /**
   * Returns the source object instance for this migration.
   * 
   * @access protected
   * @return MigrateSource
   *   Returns a newly created source object instance.
   */
  protected abstract function getSourceObject();


  /**
   * Returns the destination object instances for this migration.
   *  
   * @access protected
   * @return MigrateDestination
   *   Returns a newly created destination object instance.
   */
  protected abstract function getDestinationObject();

  /**
   * Returns the migrate map object instance for this migration.
   * 
   * @access protected
   * @return MigrateMap
   *   Returns a newly created map object instance.
   */
  protected abstract function getMapObject();

  public function processImport(array $options = array()) {
    $return = parent::processImport($options);
    var_dump($return); exit;
  }

  /**
   * {@inheritDoc}
   */
  protected function beginProcess($new_status) {
    parent::beginProcess($new_status);

    switch ($this->status) {
      case Migration::STATUS_IMPORTING:
      case Migration::STATUS_ROLLING_BACK:
        $this->setSource($this->getSourceObject());
        $this->setDestination($this->getDestinationObject());
        $this->setMap($this->getMapObject());
        $this->setFieldMappings();
        break;
    }
  }

  /**
   * {@inheritDoc}
   */
  final public function preImport() {
    $this->executePlugins(ImagexMigration::PREIMPORT);
  }

  /**
   * {@inheritDoc}
   */
  final public function postImport() {
    $this->executePlugins(ImagexMigration::POSTIMPORT);
  }

  /**
   * {@inheritDoc}
   */
  public function isComplete() {
    if (NULL === $this->getSource()) {
      $this->setSource($this->getSourceObject());
    }
    return parent::isComplete();
  }

  /**
   * {@inheritDoc}
   */
  public function sourceCount($refresh = FALSE) {
    if (NULL === $this->getSource()) {
      $this->setSource($this->getSourceObject());
    }
    return parent::sourceCount($refresh);
  }

  /**
   * {@inheritDoc}
   */
  public function processedCount() {
    if (NULL === $this->getMap()) {
      $this->setMap($this->getMapObject());
    }
    return parent::processedCount();
  }

  /**
   * {@inheritDoc}
   */
  public function importedCount() {
    if (NULL === $this->getMap()) {
      $this->setMap($this->getMapObject());
    }
    return parent::importedCount();
  }

  /**
   * {@inheritDoc}
   */
  public function updateCount() {
    if (NULL === $this->getMap()) {
      $this->setMap($this->getMapObject());
    }
    return parent::updateCount();
  }

  /**
   * {@inheritDoc}
   */
  public function errorCount() {
    if (NULL === $this->getMap()) {
      $this->setMap($this->getMapObject());
    }
    return parent::errorCount();
  }

  /**
   * {@inheritDoc}
   */
  public function messageCount() {
    if (NULL === $this->getMap()) {
      $this->setMap($this->getMapObject());
    }
    return parent::messageCount();
  }

  /**
   * {@inheritDoc}
   */
  public function prepareUpdate() {
    if (NULL === $this->getMap()) {
      $this->setMap($this->getMapObject());
    }
    return parent::prepareUpdate();
  }

  /**
   * {@inheritDoc}
   */
  public function saveMessage($message, $level = MigrationBase::MESSAGE_ERROR) {
    if (NULL === $this->getMap()) {
      $this->setMap($this->getMapObject());
    }
    return parent::saveMessage($message, $level);
  }

  /**
   * {@inheritDoc}
   */
  public function setUpdate(array $source_key = NULL) {
    if (NULL === $this->getMap()) {
      $this->setMap($this->getMapObject());
    }
    return parent::setUpdate($source_key);
  }

  /**
   * {@inheritDoc}
   */
  protected function currentSourceKey() {
    if (NULL === $this->getSource()) {
      $this->setSource($this->getSourceObject());
    }
    return parent::currentSourceKey();
  }
}
