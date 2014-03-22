<?php
/**
* ArduinoIcc.php
*
*/
namespace Joduino\Model;

use Zend\ServiceManager\ServiceLocatorAwareInterface;
use Zend\ServiceManager\ServiceLocatorInterface;
use Joduino\Model\Environment;

class ArduinoIcc implements ServiceLocatorAwareInterface
{
  protected 	$service_manager;
  private 	$sendIDeuxCcmd;
  protected 	$response;
  protected 	$environmentTable;

  const FOAM_ON = 14;
  const FOAM_OFF = 15;
  const FAN_ON = 11;
  const FAN_OFF = 12;

  public function __construct($service_manager)
  {
      $this->service_manager = $service_manager;
      $config = $this->getServiceLocator()->get('config');
      $sendIDeuxCcmd = $config['Joduino']['sendIDeuxCcmd'];

      if(!isset($sendIDeuxCcmd) OR is_null($sendIDeuxCcmd))
      {
	      $e = new \Exception('Please add Joduino sendIDeuxCcmd to config !');
	      throw $e;
      }

      $this->sendIDeuxCcmd = $sendIDeuxCcmd;
  }

  public function getResponse()
  {
      $json = json_decode($this->response);
      return $json;
  }

  public function changeFanState($state)
  {
    $json = null;

    if($state)
    {
      if($state == 'on' || $state == '1')
		      $send = self::FAN_ON;

      elseif($state == 'off')
		      $send = self::FAN_OFF;

      else
		      throw new \Exception('Bad state specified');

      $json = $this->sendMsgToArduino($send, false);
    }
    return $json;
  }

  public function changeFoamState($state)
  {
    $json = null;

    if($state)
    {
      if($state == 'on' || $state == '1')

		      $send = self::FOAM_ON;

      elseif($state == 'off')

		      $send = self::FOAM_OFF;
      else

		      throw new \Exception('Bad state specified');

      $json = $this->sendMsgToArduino($send, false);
    }
    return $json;
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

  public function sendMsgToArduino($code, $decode = true)
  {
    $json = false;
    $error = 'OK';
    $sentcode = $code;

    $cmd = $this->sendIDeuxCcmd . ' -m ';

    exec($cmd . " $code", $output);

    $brutJson = trim(current($output));

    $this->response = $brutJson;

    if($decode)
      $json = json_decode($brutJson);
    else
      $json = $brutJson;

    switch (json_last_error()) {

	    case JSON_ERROR_DEPTH:
		    $error = ' - Profondeur maximale atteinte';
	    break;
	    case JSON_ERROR_STATE_MISMATCH:
		    $error = ' - Inadéquation des modes ou underflow';
	    break;
	    case JSON_ERROR_CTRL_CHAR:
		    $error = ' - Erreur lors du contrôle des caractères';
	    break;
	    case JSON_ERROR_SYNTAX:
		    $error = ' - Erreur de syntaxe ; JSON malformé';
	    break;
	    case JSON_ERROR_UTF8:
		    $error = ' - Caractères UTF-8 malformés, probablement une erreur d\'encodage';
	    break;

	    default:
	    break;
    }
    $this->_saveResponse(json_decode($brutJson),$sentcode,$error);
    return $json;
  }

  public function getEnvironmentTable()
  {
    if (!$this->environmentTable) {
      $sm = $this->getServiceLocator();
      $this->environmentTable = $sm->get('Joduino\Model\EnvironmentTable');
    }
    return $this->environmentTable;
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