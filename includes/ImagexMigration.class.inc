<?php
/**
 * @file
 */

/**
 * Non-instantiable class with helper methods and constants.
 * 
 * @author Aaron McGowan <aaron@imagexmedia.com>
 * @package Imagex.Migration
 * @version 1.0.0
 */
abstract class ImagexMigration {
  const INITIALIZE  = 'initialize';
  const PREIMPORT   = 'preimport';
  const POSTIMPORT  = 'postimport';

  /**
   * Returns an array of all the event constants.
   * 
   * An example of an event constant would be ImagexMigration::PREIMPORT.
   *
   * @access public
   * @return array
   *   Returns an array of event constant values.
   */
  public static function getArrayOfEvents() {
    return array(
      self::INITIALIZE,
      self::PREIMPORT,
      self::POSTIMPORT,
    );
  }
}
