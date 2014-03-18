<?php
/**
 * Zend Framework (http://framework.zend.com/)
 *
 * @link      http://github.com/zendframework/ZendSkeletonApplication for the canonical source repository
 * @copyright Copyright (c) 2005-2014 Zend Technologies USA Inc. (http://www.zend.com)
 * @license   http://framework.zend.com/license/new-bsd New BSD License
 */

namespace Joduino\Controller;

use Zend\Mvc\Controller\AbstractActionController;
use Zend\View\Model\ViewModel;

class IndexController extends AbstractActionController
{
    public function indexAction()
    {
	$foam 		= $this->getRequest()->getQuery('foam');
	$fan 		= $this->getRequest()->getQuery('fan');
	$histo_nb_elt 	= $this->getRequest()->getQuery('nbelt', 10);

	$arduinoIcc = $this->getServiceLocator()->get('Joduino\Model\ArduinoIcc');
	$environmentTable = $this->getServiceLocator()->get('Joduino\Model\EnvironmentTable');

	$history = $environmentTable->getHistory($histo_nb_elt);


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

        return new ViewModel(array(
			'data' => $json, 
			'fan' => $fan, 
			'foam' => $foam,
			'history' => $history,
			'histo_nb_elt' => $histo_nb_elt,
			));
    }

}
