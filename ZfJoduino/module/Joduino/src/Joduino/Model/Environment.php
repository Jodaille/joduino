<?php
/**
* Joduino (https://github.com/Jodaille)
*
* @link      https://github.com/Jodaille/joduino for the canonical source repository
* @copyright Copyright (c) 2014 Jodaille (http://jodaille.org)
* @license   New BSD License
*/
namespace Joduino\Model;

class Environment
{
  public $id;
  public $sensor_id;
  public $humidity;
  public $temperature;
  public $photoresistance;
  public $msgsent;
  public $msgrcved;
  public $msgdate;

  public function __construct($sensor_id = false)
  {
    if($sensor_id)
    $this->sensor_id = $sensor_id;
  }

  public function exchangeArray($data)
  {
    $this->id     		= (!empty($data['id'])) ? $data['id'] : null;
    $this->sensor_id     	= (!empty($data['sensor_id'])) ? $data['sensor_id'] : null;
    $this->humidity     	= (!empty($data['humidity'])) ? $data['humidity'] : null;
    $this->temperature 	= (!empty($data['temperature'])) ? $data['temperature'] : null;
    $this->photoresistance 	= (!empty($data['photoresistance'])) ? $data['photoresistance'] : null;
    $this->waterlevel 		= (!empty($data['waterlevel'])) ? $data['waterlevel'] : null;

    $this->msgsent 		= (!empty($data['msgsent'])) ? $data['msgsent'] : null;
    $this->msgrcved 	= (!empty($data['msgrcved'])) ? $data['msgrcved'] : null;
    $this->msgdate 		= (!empty($data['msgdate'])) ? $data['msgdate'] : null;
  }

  public function setJson($oJsonDecode)
  {
    $this->id     		= (!empty($oJsonDecode->id)) ? $oJsonDecode->id : null;
    $this->sensor_id     	= $this->sensor_id;
    $this->humidity     	= (!empty($oJsonDecode->humidity)) ? $oJsonDecode->humidity : null;;
    $this->temperature 		= (!empty($oJsonDecode->temperature)) ? $oJsonDecode->temperature : null;
    $this->photoresistance = (!empty($oJsonDecode->light)) ? $oJsonDecode->light : null;
    $this->waterlevel = (!empty($oJsonDecode->waterlevel)) ? $oJsonDecode->waterlevel : null;
    $this->msgsent	 	= (!empty($oJsonDecode->msgsent)) ? $oJsonDecode->msgsent : null;
    $this->msgrcved	 	= (!empty($oJsonDecode->alarm)) ? $oJsonDecode->alarm : null;
    $this->msgdate 		= (!empty($oJsonDecode->date)) ? $oJsonDecode->date : $this->_getDate();
  }

  public function setMsgReceived($msg)
  {
    $this->msgrcved = $msg;
  }

  public function setMsgSent($code)
  {
    $this->msgsent = $code;
  }

  private function _getDate()
  {
    $datetime = strftime('%Y-%m-%d %H:%M:%S');

    return $datetime ;
  }
}
