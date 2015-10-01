<?php
/**
* Joduino (https://github.com/Jodaille)
*
* @link      https://github.com/Jodaille/joduino for the canonical source repository
* @copyright Copyright (c) 2014 Jodaille (http://jodaille.org)
* @license   New BSD License
*/

namespace Joduino\Controller;

use Zend\Mvc\Controller\AbstractActionController;
use Zend\View\Model\ViewModel;
use Zend\View\Model\JsonModel;

class IndexController extends AbstractActionController
{

  public function indexAction()
  {
    $foam 		= $this->getRequest()->getQuery('foam');
    $fan 		= $this->getRequest()->getQuery('fan');
    $histo_nb_elt 	= $this->getRequest()->getQuery('nbelt', 10);

    $arduinoIcc = $this->getServiceLocator()->get('Joduino\Model\ArduinoIcc');
    $environmentTable = $this->getServiceLocator()->get('Joduino\Model\EnvironmentTable');

    if($fan == 'fan_on')
    {
      $json = $arduinoIcc->sendMsgToArduino(11);
    }
    else
    {
      $json = $arduinoIcc->sendMsgToArduino(12);
    }

    if($foam == 'foam_on')
    {
      $json = $arduinoIcc->sendMsgToArduino(14);
    }
    else
    {
      $json = $arduinoIcc->sendMsgToArduino(15);
    }

    $history = $environmentTable->getHistory($histo_nb_elt);

    return new ViewModel(array(
      'data' => $json,
      'fan' => $fan,
      'foam' => $foam,
      'history' => $history,
      'histo_nb_elt' => $histo_nb_elt,
    ));
  }

  public function histoAction()
  {
    $environmentTable = $this->getServiceLocator()->get('Joduino\Model\EnvironmentTable');
    $historyT = $environmentTable->getHistoBar('temperature');
    $historyH = $environmentTable->getHistoBar('humidity');
    $historyW = $environmentTable->getHistoBar('waterlevel');
    $historyL = $environmentTable->getHistoBar('photoresistance');


    return new ViewModel(array(
      'historyT' => $historyT,
      'historyH' => $historyH,
      'historyW' => $historyW,
      'historyL' => $historyL,

    ));
  }

  public function detecti2cAction()
  {
    $arduinoIcc = $this->getServiceLocator()->get('Joduino\Model\ArduinoIcc');
    $detectInfos = $arduinoIcc->detect($address = 12);

    return new ViewModel(array(
      'detectInfos' => $detectInfos,
    ));
  }

  public function detecthttpAction()
  {
    $action 	= $this->getRequest()->getQuery('action', 'json');
    $urlArduino = "http://192.168.0.177/$action/";
    $page = file_get_contents($urlArduino);

    $page = trim($page);

    return new ViewModel(array(
      'page' => $page,
    ));
  }
}
