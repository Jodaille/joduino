<?php
/**
 * ArduinoJsonLog.php
 *
 * Joduino (https://github.com/Jodaille)
 *
 * @link      https://github.com/Jodaille/joduino for the canonical source repository
 * @copyright Copyright (c) 2014 Jodaille (http://jodaille.org)
 * @license   New BSD License
*/

namespace Joduino\Model;

use Zend\ServiceManager\ServiceLocatorAwareInterface;
use Zend\ServiceManager\ServiceLocatorInterface;
use Joduino\Model\Environment;

class ArduinoJsonLog implements ServiceLocatorAwareInterface
{
  protected 	$service_manager;
  protected 	$response;
  protected 	$environmentTable;
  protected 	$filepath;


  public function importFile($filepath, $sensor_id = false)
  {
    $this->setFilepath($filepath);
    $handle = fopen($filepath, 'r');
    $count = 0;
    if ($handle) {
        while (!feof($handle)) {
            $buffer = fgets($handle, 4096);
            $oJson = $this->decodeJson($buffer);
            if($oJson)
            {
              $this->_saveJson($oJson, $sensor_id);
              $count++;
            }
        }
        fclose($handle);
    }
  }

  private function _saveJson($oJson, $sensor_id = 1)
  {

    $oEnvironment = new Environment($sensor_id);

    $oEnvironment->setJson($oJson);

    $env = $this->getEnvironmentTable()->saveEnvironment($oEnvironment);
    return $env;
  }

  private function _saveResponse($oJson,$sentcode,$error)
  {
    $sensor_id = 1;

    $oEnvironment = new Environment($sensor_id);

    $oEnvironment->setJson($oJson);

    $oEnvironment->setMsgReceived($error);

    $oEnvironment->setMsgSent($sentcode);

    $env = $this->getEnvironmentTable()->saveEnvironment($oEnvironment);
    return $env;
  }

  public function decodeJson($sJson)
  {

    $error  = false;
    $brutJson = trim($sJson);

    $json = json_decode($brutJson);

    switch (json_last_error()) {

      case JSON_ERROR_DEPTH:
        $error = ' - Profondeur maximale atteinte';
      break;
      case JSON_ERROR_STATE_MISMATCH:
        $error = ' - Inadequation des modes ou underflow';
      break;
      case JSON_ERROR_CTRL_CHAR:
        $error = $brutJson;
      break;
      case JSON_ERROR_SYNTAX:
        $error = ' - Erreur de syntaxe ; JSON malforme';
      break;
      case JSON_ERROR_UTF8:
        $error = ' - BAD UTF-8, probablement une erreur d\'encodage';
      break;
      default:

      break;
    }
    if(!$error)
      return $json;
    else
      return false;
  }

  public function getEnvironmentTable()
  {
    if (!$this->environmentTable) {
      $sm = $this->getServiceLocator();
      $this->environmentTable = $sm->get('Joduino\Model\EnvironmentTable');
    }
    return $this->environmentTable;
  }

  public function setFilepath($filepath)
  {
    $this->filepath = $filepath;
  }

  public function setServiceLocator(ServiceLocatorInterface $serviceLocator)
  {
    $this->service_manager = $serviceLocator;
  }

  public function getServiceLocator()
  {
    return $this->service_manager;
  }
}
